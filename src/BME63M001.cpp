/*****************************************************************
File:             BME63M001->cpp
Author:           BESTMODULES
Description:      Define classes and required variables if this class
                 has parameters,input parameters when this class defined
                 the class(this function) can call its member in current
                 function,or by called outside else
History：
V1.0.1-- initial version；2022-12-06；Arduino IDE : ≥v1.8.13
******************************************************************/
#include "BME63M001.h"
#include <SoftwareSerial.h>
/**********************************************************
Description: Constructor
Parameters: *theSerial: Wire object if your board has more than one UART interface
                         parameter range:
                                         BMduino UNO: &Serial、&Serial1、&Serial2、&Serial3、&Serial4
                                         Arduino UNO：&Serial
Return:      None
Others:      None
**********************************************************/
BME63M001::BME63M001(HardwareSerial *theSerial)
{
    _hardSerial = theSerial;
    _softSerial = NULL;
}
/**********************************************************
Description: Constructor
Parameters:  rxPin: Receiver pin of the UART
             txPin: Send signal pin of UART
Return:      None
Others:   	 None
**********************************************************/
BME63M001::BME63M001(uint8_t rxPin, uint8_t txPin)
{
    _hardSerial = NULL;
    _rxPin = rxPin;
    _txPin = txPin;
    _softSerial = new SoftwareSerial(_rxPin, _txPin);
}
/**********************************************************
Description: Set the baud of the serial communicates
             with module

Parameters:  baud:(default 9600bps)
Output:      None
Return:      None
Others:      None
**********************************************************/
void BME63M001::begin(uint32_t baud)
{
    checkSum = 0;
    if (_softSerial != NULL)
    {
        _softSerial->begin(baud);
        _softSerial->setTimeout(25); // set softwareUart baud
    }
    else
    {
        _hardSerial->begin(baud);
        _hardSerial->setTimeout(25); // set hardwareUart timeout
    }
}

/**********************************************************
Description: Get modular F/W version
Parameters:
Return:      FWVer
Others:      F/W version number = FWVer highByte.FWVer lowByte
**********************************************************/
uint16_t BME63M001::getFWVer()
{
    uint8_t check, i;
    int16_t FWVer = 0;
    checkSum = HEADER + ADDRESSHIGH + ADDRESSLOW + COMMAND0_TX_LENGTH + COMMAND0;
    checkSum = ~checkSum;
    uint8_t FWCMD[6] = {HEADER, ADDRESSHIGH, ADDRESSLOW, COMMAND0_TX_LENGTH, COMMAND0, checkSum};
    writeBytes(FWCMD, 6);
    checkSum = 0;
    check = readBytes(SWRxbuff, COMMAND0_RX_LENGTH, 25);
    if (check == CHECK_OK)
    {
        FWVer = SWRxbuff[5] * 256UL + SWRxbuff[6];
    }
    for (i = 0; i < 12; i++)
    {
        SWRxbuff[i] = 0;
    }
    return FWVer;
}
/**********************************************************
Description:Get the current single channel TDS and the temperature
Parameters: channel:Variable for storing Channels of TDS/NTC to be get
Return:     TDSValue:Water quality concentration
Others:     None
**********************************************************/
float BME63M001::readTDS(uint8_t channel)
{
    TDSValue[0] = {0};
    readData(channel, TDSValue, NTCValue);
    return TDSValue[0];
}
/**********************************************************
Description:Get the current single channel TDS and the temperature
Parameters: channel:Variable for storing Channels of TDS/NTC to be get
Return:     NTCValue:Water temperature
Others:     None
**********************************************************/
float BME63M001::readTemperature(uint8_t channel)
{
    NTCValue[0] = {0};
    readData(channel, TDSValue, NTCValue);
    return NTCValue[0];
}
/**********************************************************
Description:set dormant
Parameters:
Return:		0:CHECK_FAIL
            1:CHECK_OK
Others:		None
**********************************************************/
uint8_t BME63M001::sleep()
{
    uint8_t check, i;
    SWRxbuff[0] = 0;
    checkSum = HEADER + ADDRESSHIGH + ADDRESSLOW + COMMAND2_TX_LENGTH + COMMAND2;
    checkSum = ~checkSum;
    uint8_t sleepCMD[6] = {HEADER, ADDRESSHIGH, ADDRESSLOW, COMMAND2_TX_LENGTH, COMMAND2, checkSum};
    writeBytes(sleepCMD, 6);
    checkSum = 0;
    check = readBytes(SWRxbuff, COMMAND2_RX_LENGTH, 25);
    for (i = 0; i < 12; i++)
    {
        SWRxbuff[i] = 0;
    }
    if (check == CHECK_OK)
    {
        return CHECK_OK;
    }
    else
    {
        return CHECK_FAIL;
    }
}
/**********************************************************
Description: reset
Parameters: None
Return:		0:CHECK_FAIL
            1:CHECK_OK
Others:		None
**********************************************************/
uint8_t BME63M001::reset()
{
    uint8_t check, i;
    SWRxbuff[0] = 0;
    checkSum = HEADER + ADDRESSHIGH + ADDRESSLOW + COMMAND3_TX_LENGTH + COMMAND3;
    checkSum = ~checkSum;
    uint8_t resetCMD[6] = {HEADER, ADDRESSHIGH, ADDRESSLOW, COMMAND3_TX_LENGTH, COMMAND3, checkSum};
    writeBytes(resetCMD, 6);
    checkSum = 0;
    check = readBytes(SWRxbuff, COMMAND3_RX_LENGTH, 25);
    for (i = 0; i < 12; i++)
    {
        SWRxbuff[i] = 0;
    }
    if (check == CHECK_OK)
    {
        return CHECK_OK;
    }
    else
    {
        return CHECK_FAIL;
    }
}
/**********************************************************
Description:Get the current single channel TDS and the temperature
Parameters: channel:Variable for storing Channels of TDS/NTC to be get
            getTDSValue:Variable for storing current TDS value
            getNTCValue:Variable for storing current NTC value
Return:     1:CHECK_OK
      4:TIMEOUT_ERROR
      5:CHECK_ERROR
Others:     None
**********************************************************/
uint8_t BME63M001::readData(uint8_t channel, float getTDSValue[], float getNTCValue[])
{
    uint8_t check, i;
    if (channel > '0')
    {
        channel = channel - '0'; /*input ascii format*/
    }
    if ((channel != 1) && (channel != 2))
    {
        getNTCValue[0] = 0;
        getTDSValue[0] = 0;
        return CHANNEL_ERROR;
    }

    SWRxbuff[0] = 0;
    checkSum = HEADER + ADDRESSHIGH + ADDRESSLOW + COMMAND1_TX_LENGTH + COMMAND1 + channel;
    checkSum = ~checkSum;
    uint8_t dataCMD[7] = {HEADER, ADDRESSHIGH, ADDRESSLOW, COMMAND1_TX_LENGTH, COMMAND1, channel, checkSum};
    writeBytes(dataCMD, 7);
    checkSum = 0;
    check = readBytes(SWRxbuff, COMMAND1_RX_LENGTH, 25);
    if (check == CHECK_OK)
    {
        getTDSValue[0] = float((uint32_t(SWRxbuff[6]) << 16) + (uint16_t(SWRxbuff[7]) << 8) + SWRxbuff[8]);
        getNTCValue[0] = float((uint16_t(SWRxbuff[9]) << 8) + SWRxbuff[10]) / 10;
        getTDSValue[0] = getTDSValue[0] / 20;
    }
    else
    {
        getNTCValue[0] = 0;
        getTDSValue[0] = 0;
    }
    for (i = 0; i < 12; i++)
    {
        SWRxbuff[i] = 0;
    }
    return check;
}
/**********************************************************
Description:UART read Bytes
Parameters:  	rbuf:buff for storing uart receive data
                  command:Module command instructions
Return:     1:CHECK_OK
                  4:TIMEOUT_ERROR
                  5:CHECK_ERROR
Others:   	None
**********************************************************/
uint8_t BME63M001::readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeout)
{
    uint8_t i, delayCnt = 0, checkSum = 0;

    if (_softSerial != NULL)
    {
        for (i = 0; i < rlen; i++)
        {
            delayCnt = 0;
            while (_softSerial->available() == 0)
            {
                if (delayCnt > timeout)
                {
                    return TIMEOUT_ERROR; // Timeout error
                }
                delay(1);
                delayCnt++;
            }
            rbuf[i] = _softSerial->read();
        }
    }
    /* Select HardwareSerial Interface */
    else
    {
        for (i = 0; i < rlen; i++)
        {
            delayCnt = 0;
            while (_hardSerial->available() == 0)
            {
                if (delayCnt > timeout)
                {
                    return TIMEOUT_ERROR; // Timeout error
                }
                delay(1);
                delayCnt++;
            }
            rbuf[i] = _hardSerial->read();
        }
    }

    /* check Sum */
    for (i = 0; i < rlen; i++)
    {
        checkSum += rbuf[i];
    }
    checkSum++;
    if (checkSum == 0)
    {
        return CHECK_OK; // Check correct
    }
    else
    {
        return CHECK_ERROR; // Check error
    }
}
/**********************************************************
Description:UART write bytes
Parameters:    wbuff:wbuff for storing uart receive data
               wlen: data length
Return:
Others:     None
**********************************************************/
void BME63M001::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
    /* Select SoftwareSerial Interface */
    if (_softSerial != NULL)
    {
        while (_softSerial->available() > 0)
            ;
        {
            _softSerial->read();
        }
        _softSerial->write(wbuf, wlen);
    }
    /* Select HardwareSerial Interface */
    else
    {
        while (_hardSerial->available() > 0)
            ;
        {
            _hardSerial->read();
        }
        _hardSerial->write(wbuf, wlen);
    }
}
