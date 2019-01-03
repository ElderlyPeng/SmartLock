#include "..\sim800\bsp_sim800.h"
#include "..\systick\bsp_SysTick.h"
#include "..\usart\bsp_usart.h"
#include <string.h>

extern char rx_buf_SIM[255];
extern uint8_t num_SIM;
char temp_str[50];    // ��ʱ�Ӵ�

// ��ĸ���л�ȡ���Ӵ�������ȵ���ʱ�Ӵ�
static void ReadStrUnit(char * str,char *temp_str,int idx,int len)  
{
    uint16_t index = 0;
    for(index = 0; index < len; index++)
    {
        temp_str[index] = str[idx+index];
    }
    temp_str[index] = '\0';
}

// �����Ӵ��ĺ���
static int GetSubStrPos(char *str1,char *str2)
{
    int idx = 0;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if( len1 < len2)
    {    
        return -1; // �Ӵ���ĸ����
    }

    while(1)
    {
        ReadStrUnit(str1,temp_str,idx,len2);    // ���ϻ�ȡ�Ĵ� ĸ���� idx λ�ô�������ʱ�Ӵ�
        if(strcmp(str2	,temp_str)==0)break;      // ����ʱ�Ӵ����Ӵ�һ�£�����ѭ��
        idx++;                                  // �ı��ĸ����ȡ��ʱ�Ӵ���λ��
        if(idx>=len1-len2)return -1;                 // �� idx �Ѿ�����ĸ�����ȣ�˵��ĸ�����������Ӵ�
    }

    return idx;    // �����Ӵ���һ���ַ���ĸ���е�λ��
}

// �����������CMDΪ�����ַ���������ReturnΪsim800�ķ���ֵ��ֻ�еȵ�����ֵ�Ż����������timeOutΪ��ʱʱ�䣨��λ1ms��
// �ɹ�����1�� ��ʱ����0
static int SIM800_SendCMD(char* CMD, char* Return, uint16_t timeOut)
{
	uint16_t i = 0;
	memset(rx_buf_SIM,0,255);
	num_SIM = 0;
	printf("%s\r\n",CMD);
	while(GetSubStrPos(rx_buf_SIM, Return) == -1)
	{
		SysTick_Delay_Ms(1);
		i++;
		if (i >= timeOut)
			return 0;
	}
	return 1;
}

// SIM800��ʼ������������1����ʱ����0
int SIM800_Init(void)
{
	uint16_t timeOut = 0;
	// �ȴ�����ʼ�����
	memset(rx_buf_SIM,0,255);
	num_SIM = 0;
	
	while(GetSubStrPos(rx_buf_SIM, "Call Ready") == -1)
	{
		SysTick_Delay_Ms(10);
		timeOut++;
		if (timeOut >= 6000)
			return 0;
	}
	return 1;
}

// SIM800��绰,��������1����ʱ����0
int SIM800_Call(void)
{	
	SysTick_Delay_Ms(100);
	memset(rx_buf_SIM,0,255);
	num_SIM = 0;
	// �ȴ����Ҷϵ绰
	return SIM800_SendCMD("ATD17539589424", "\"CALL\",0", 60000);
}

// SIM800�����Ϸ�����
int SIM800_Connect(void)
{
	uint8_t flag = 0;
	SysTick_Delay_Ms(100);	
	
	flag += SIM800_SendCMD("AT+CGATT=1", "OK", 15000);	
	flag += SIM800_SendCMD("AT+CGACT=1,1", "OK", 10000);
	flag += SIM800_SendCMD("AT+CIPSTART=TCP,120.79.86.255,80", "CONNECT OK", 10000);
	flag += SIM800_SendCMD("AT+CIPSEND=9", ">", 10000);
	flag += SIM800_SendCMD("Fromdoor1", "SEND OK", 10000);
	flag += SIM800_SendCMD("AT+CIPSEND=9", ">", 10000);
	flag += SIM800_SendCMD("Topeople1", "SEND OK", 10000);
	flag += SIM800_SendCMD("AT+CIPSEND=4", ">", 10000);
	flag += SIM800_SendCMD("DatA", "SEND OK", 10000);	
	if (flag == 9)
		return 1;
	else 
		return 0;
}

// SIM800�Ͽ�������
int SIM800_Break(void)
{
	return SIM800_SendCMD("AT+CIPCLOSE", "OK", 15000);		
}

// SIM800�ȴ��������·���������
int SIM800_WaitCMD(void)
{
	uint16_t timeOut = 0;
	memset(rx_buf_SIM,0,255);
	num_SIM = 0;
	// �ȴ����������·���������(233),�����Ӻ�ʱ
	while(GetSubStrPos(rx_buf_SIM, "233") == -1)
	{
		SysTick_Delay_Ms(10); 
		timeOut++;
		if (timeOut >= 12000)
			return 0;
	}
	// �ɹ��յ�����󷵻��ַ���"OK"
	SIM800_SendCMD("AT+CIPSEND=5", ">", 10000);
	SIM800_SendCMD("DatOK", "SEND OK", 10000);	
	return 1;
}
