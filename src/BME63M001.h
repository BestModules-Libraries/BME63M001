/*****************************************************************
File:             BME63M001.h
Author:           BESTMODULES
Description:      Define classes and required variables
Version：  	  V1.0.2	 --2023-09-06
******************************************************************/

#ifndef BME63M001_h_
#define BME63M001_h_
#include "Arduino.h"
#include <SoftwareSerial.h>
 class BME63M001
 {
	public:
		BME63M001(HardwareSerial*theSerial=&Serial);
		BME63M001(uint8_t rxPin,uint8_t txPin);
		void begin(uint32_t baud = 9600);
		uint16_t getFWVer();
		float  	readTDS(uint8_t channel);
		float  	readTemperature(uint8_t channel);
		uint8_t	sleep();  
		uint8_t	reset();
	private:
    uint8_t readData(uint8_t channel,float getTDSValue[],float getNTCValue[]);
		uint8_t readBytes(uint8_t rbuf[],uint8_t command,uint16_t timeout);
    void writeBytes(uint8_t wbuf[], uint8_t wlen);

		uint8_t checkSum;
    unsigned char SWRxbuff[12]={0};
    float TDSValue[1] = {0};/*TDS value*/
    float NTCValue[1] = {0};/*NTC value*/
		uint8_t _rxPin;
		uint8_t _txPin;		
		HardwareSerial*_hardSerial = NULL;
		SoftwareSerial *_softSerial = NULL;		
		
 };
//Communication check code
#define  CHECK_FAIL  					(0X00)/*Error communication*/
#define  CHECK_OK  						(0X01)/*Correct communication*/
#define  TIMEOUT_ERROR  						(0X04)/*Communication timeout*/
#define  CHECK_ERROR  				(0X05)/*Receive checksum error*/
#define  COMMAND_ERROR  				(0X06)/*The receive command is incorrect*/
#define  CHANNEL_ERROR  				(0X07)/*the input channel is incorrect*/
//Protocol fixed code
#define  HEADER  						(0X55)/*Header code*/
#define  ADDRESSHIGH  					(0X63)/*Eight high bits of module address*/
#define  ADDRESSLOW  					(0X01)/*Eight low bits of module address*/
//Read data command

#define  COMMAND0						(0X00)
#define  COMMAND0_TX_LENGTH 			(0X01)/*the command&data byte length*/
#define  COMMAND0_RX_LENGTH 			(0X08)/*the protocol length*/
#define  COMMAND1						(0X01)
#define  COMMAND1_TX_LENGTH 			(0X02)
#define  COMMAND1_RX_LENGTH 			(0X0c)
#define  COMMAND2						(0X02)
#define  COMMAND2_TX_LENGTH 			(0X01)
#define  COMMAND2_RX_LENGTH 			(0X06)
#define  COMMAND3						(0X03)
#define  COMMAND3_TX_LENGTH 			(0X01)
#define  COMMAND3_RX_LENGTH 			(0X06)
#endif
