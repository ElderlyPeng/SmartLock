#include "..\sim800\bsp_sim800.h"
#include "..\systick\bsp_SysTick.h"
#include "..\usart\bsp_usart.h"
#include <string.h>

extern char rx_buf_SIM[255];
extern uint8_t num_SIM;
char temp_str[50];    // 临时子串

// 从母串中获取与子串长度相等的临时子串
static void ReadStrUnit(char * str,char *temp_str,int idx,int len)  
{
    uint16_t index = 0;
    for(index = 0; index < len; index++)
    {
        temp_str[index] = str[idx+index];
    }
    temp_str[index] = '\0';
}

// 查找子串的函数
static int GetSubStrPos(char *str1,char *str2)
{
    int idx = 0;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if( len1 < len2)
    {    
        return -1; // 子串比母串长
    }

    while(1)
    {
        ReadStrUnit(str1,temp_str,idx,len2);    // 不断获取的从 母串的 idx 位置处更新临时子串
        if(strcmp(str2	,temp_str)==0)break;      // 若临时子串和子串一致，结束循环
        idx++;                                  // 改变从母串中取临时子串的位置
        if(idx>=len1-len2)return -1;                 // 若 idx 已经超出母串长度，说明母串不包含该子串
    }

    return idx;    // 返回子串第一个字符在母串中的位置
}

// 发送命令，参数CMD为命令字符串，参数Return为sim800的返回值，只有等到返回值才会继续，参数timeOut为超时时间（单位1ms）
// 成功返回1， 超时返回0
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

// SIM800初始化，正常返回1，超时返回0
int SIM800_Init(void)
{
	uint16_t timeOut = 0;
	// 等待到初始化完成
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

// SIM800打电话,正常返回1，超时返回0
int SIM800_Call(void)
{	
	SysTick_Delay_Ms(100);
	memset(rx_buf_SIM,0,255);
	num_SIM = 0;
	// 等待到挂断电话
	return SIM800_SendCMD("ATD17539589424", "\"CALL\",0", 60000);
}

// SIM800连接上服务器
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

// SIM800断开服务器
int SIM800_Break(void)
{
	return SIM800_SendCMD("AT+CIPCLOSE", "OK", 15000);		
}

// SIM800等待服务器下发开锁命令
int SIM800_WaitCMD(void)
{
	uint16_t timeOut = 0;
	memset(rx_buf_SIM,0,255);
	num_SIM = 0;
	// 等待到服务器下发开锁命令(233),两分钟后超时
	while(GetSubStrPos(rx_buf_SIM, "233") == -1)
	{
		SysTick_Delay_Ms(10); 
		timeOut++;
		if (timeOut >= 12000)
			return 0;
	}
	// 成功收到命令后返回字符串"OK"
	SIM800_SendCMD("AT+CIPSEND=5", ">", 10000);
	SIM800_SendCMD("DatOK", "SEND OK", 10000);	
	return 1;
}
