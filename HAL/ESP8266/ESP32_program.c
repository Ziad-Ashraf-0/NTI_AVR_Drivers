/*
 * ESP32_program.c
 *
 * Created: 11/17/2023 11:29:19 PM
 *  Author: zashr
 */ 

#define  F_CPU 16000000UL
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/USART/USART_interface.h"
#include "ESP32_config.h"
#include "ESP32_interface.h"
#include "ESP32_private.h"
#include <util/delay.h>
#include <string.h>
#include <stdio.h>


u8 Response_Status;
volatile u16 Counter = 0, pointer = 0;
u32 TimeOut = 0;
char RESPONSE_BUFFER[DEFAULT_BUFFER_SIZE];

void ESP8266_Init(void)
{
	UART_receiveByteAsynchCallBack(receiveCallback);
}

void Read_Response(char* _Expected_Response)
{
	u8 EXPECTED_RESPONSE_LENGTH = strlen(_Expected_Response);
	u32 TimeCount = 0, ResponseBufferLength;
	char RECEIVED_CRLF_BUF[EXPECTED_RESPONSE_LENGTH];

	while(1)
	{
		if(TimeCount >= (DEFAULT_TIMEOUT+TimeOut))
		{
			TimeOut = 0;
			Response_Status = ESP8266_RESPONSE_TIMEOUT;
			return;
		}

		if(Response_Status == ESP8266_RESPONSE_STARTING)
		{
			Response_Status = ESP8266_RESPONSE_WAITING;
		}

		ResponseBufferLength = strlen(RESPONSE_BUFFER);
		if (ResponseBufferLength)
		{
			_delay_ms(1);
			TimeCount++;
			if (ResponseBufferLength==strlen(RESPONSE_BUFFER))
			{
				for (u16 i=0;i<ResponseBufferLength;i++)
				{
					memmove(RECEIVED_CRLF_BUF, RECEIVED_CRLF_BUF + 1, EXPECTED_RESPONSE_LENGTH-1);
					RECEIVED_CRLF_BUF[EXPECTED_RESPONSE_LENGTH-1] = RESPONSE_BUFFER[i];
					if(!strncmp(RECEIVED_CRLF_BUF, _Expected_Response, EXPECTED_RESPONSE_LENGTH))
					{
						TimeOut = 0;
						Response_Status = ESP8266_RESPONSE_FINISHED;
						return;
					}
				}
			}
		}
		_delay_ms(1);
		TimeCount++;
	}
}

void ESP8266_Clear()
{
	memset(RESPONSE_BUFFER,0,DEFAULT_BUFFER_SIZE);
	Counter = 0;	pointer = 0;
}

void Start_Read_Response(char* _ExpectedResponse)
{
	Response_Status = ESP8266_RESPONSE_STARTING;
	do {
		Read_Response(_ExpectedResponse);
	} while(Response_Status == ESP8266_RESPONSE_WAITING);

}

void GetResponseBody(char* Response, uint16_t ResponseLength)
{

	uint16_t i = 12;
	char buffer[5];
	while(Response[i] != '\r')
	++i;

	strncpy(buffer, Response + 12, (i - 12));
	ResponseLength = atoi(buffer);

	i += 2;
	uint16_t tmp = strlen(Response) - i;
	memcpy(Response, Response + i, tmp);

	if(!strncmp(Response + tmp - 6, "\r\nOK\r\n", 6))
	memset(Response + tmp - 6, 0, i + 6);
}

u8 WaitForExpectedResponse(char* ExpectedResponse)
{
	Start_Read_Response(ExpectedResponse);	/* First read response */
	if((Response_Status != ESP8266_RESPONSE_TIMEOUT))
	return true;				/* Return true for success */
	return false;				/* Else return false */
}

u8 SendATandExpectResponse(char* ATCommand, char* ExpectedResponse)
{
	ESP8266_Clear();
	UART_sendString(ATCommand);		/* Send AT command to ESP8266 */
	UART_sendString("\r\n");
	return WaitForExpectedResponse(ExpectedResponse);
}

u8 ESP8266_ApplicationMode(u8 Mode)
{
	char _atCommand[20];
	memset(_atCommand, 0, 20);
	sprintf(_atCommand, "AT+CIPMODE=%d", Mode);
	_atCommand[19] = 0;
	return SendATandExpectResponse(_atCommand, "\r\nOK\r\n");
}

u8 ESP8266_ConnectionMode(u8 Mode)
{
	char _atCommand[20];
	memset(_atCommand, 0, 20);
	sprintf(_atCommand, "AT+CIPMUX=%d", Mode);
	_atCommand[19] = 0;
	return SendATandExpectResponse(_atCommand, "\r\nOK\r\n");
}

u8 ESP8266_Begin()
{
	for (u8 i=0;i<5;i++)
	{
		if(SendATandExpectResponse("AT","\r\nOK\r\n"))
		return true;
	}
	return false;
}

u8 ESP8266_Close()
{
	return SendATandExpectResponse("AT+CIPCLOSE=1", "\r\nOK\r\n");
}

u8 ESP8266_WIFIMode(u8 _mode)
{
	char _atCommand[20];
	memset(_atCommand, 0, 20);
	sprintf(_atCommand, "AT+CWMODE=%d", _mode);
	_atCommand[19] = 0;
	return SendATandExpectResponse(_atCommand, "\r\nOK\r\n");
}

u8 ESP8266_JoinAccessPoint(char* _SSID, char* _PASSWORD)
{
	char _atCommand[60];
	memset(_atCommand, 0, 60);
	sprintf(_atCommand, "AT+CWJAP=\"%s\",\"%s\"", _SSID, _PASSWORD);
	_atCommand[59] = 0;
	if(SendATandExpectResponse(_atCommand, "\r\nWIFI CONNECTED\r\n"))
	return ESP8266_WIFI_CONNECTED;
	else{
		if(strstr(RESPONSE_BUFFER, "+CWJAP:1"))
		return ESP8266_CONNECTION_TIMEOUT;
		else if(strstr(RESPONSE_BUFFER, "+CWJAP:2"))
		return ESP8266_WRONG_PASSWORD;
		else if(strstr(RESPONSE_BUFFER, "+CWJAP:3"))
		return ESP8266_NOT_FOUND_TARGET_AP;
		else if(strstr(RESPONSE_BUFFER, "+CWJAP:4"))
		return ESP8266_CONNECTION_FAILED;
		else
		return ESP8266_JOIN_UNKNOWN_ERROR;
	}
}

u8 ESP8266_connected()
{
	SendATandExpectResponse("AT+CIPSTATUS", "\r\nOK\r\n");
	if(strstr(RESPONSE_BUFFER, "STATUS:2"))
	return ESP8266_CONNECTED_TO_AP;
	else if(strstr(RESPONSE_BUFFER, "STATUS:3"))
	return ESP8266_CREATED_TRANSMISSION;
	else if(strstr(RESPONSE_BUFFER, "STATUS:4"))
	return ESP8266_TRANSMISSION_DISCONNECTED;
	else if(strstr(RESPONSE_BUFFER, "STATUS:5"))
	return ESP8266_NOT_CONNECTED_TO_AP;
	else
	return ESP8266_CONNECT_UNKNOWN_ERROR;
}

u8 ESP8266_Start(u8 _ConnectionNumber, char* Domain, char* Port)
{
	u8 _startResponse;
	char _atCommand[60];
	memset(_atCommand, 0, 60);
	_atCommand[59] = 0;

	if(SendATandExpectResponse("AT+CIPMUX?", "CIPMUX:0"))
	sprintf(_atCommand, "AT+CIPSTART=\"TCP\",\"%s\",%s", Domain, Port);
	else
	sprintf(_atCommand, "AT+CIPSTART=\"%d\",\"TCP\",\"%s\",%s", _ConnectionNumber, Domain, Port);

	_startResponse = SendATandExpectResponse(_atCommand, "CONNECT\r\n");
	if(!_startResponse)
	{
		if(Response_Status == ESP8266_RESPONSE_TIMEOUT)
		return ESP8266_RESPONSE_TIMEOUT;
		return ESP8266_RESPONSE_ERROR;
	}
	return ESP8266_RESPONSE_FINISHED;
}

u8 ESP8266_Send(char* Data)
{
	char _atCommand[20];
	memset(_atCommand, 0, 20);
	sprintf(_atCommand, "AT+CIPSEND=%d", (strlen(Data)+2));
	_atCommand[19] = 0;
	SendATandExpectResponse(_atCommand, "\r\nOK\r\n>");
	if(!SendATandExpectResponse(Data, "\r\nSEND OK\r\n"))
	{
		if(Response_Status == ESP8266_RESPONSE_TIMEOUT)
		return ESP8266_RESPONSE_TIMEOUT;
		return ESP8266_RESPONSE_ERROR;
	}
	return ESP8266_RESPONSE_FINISHED;
}

u16 ESP8266_DataAvailable()
{
	return (Counter - pointer);
}

u8 ESP8266_DataRead()
{
	if(pointer < Counter)
	return RESPONSE_BUFFER[pointer++];
	else{
		ESP8266_Clear();
		return 0;
	}
}

u16 Read_Data(char* _buffer)
{
	u16 len = 0;
	_delay_ms(100);
	while(ESP8266_DataAvailable() > 0)
	_buffer[len++] = ESP8266_DataRead();
	return len;
}

static void receiveCallback(u8 data)
{
	RESPONSE_BUFFER[Counter] = data;
	Counter++;
	if(Counter == DEFAULT_BUFFER_SIZE){
		Counter = 0; pointer = 0;
	}
}
