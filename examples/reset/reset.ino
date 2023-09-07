/*****************************************************************
File:         reset
Description:  Reset the modular,Please ensure that the baud rate
              of the Serial Monitor is 115200.
******************************************************************/

#include <BME63M001.h>
#include <SoftwareSerial.h>
BME63M001 myTDS(6,7);//SoftwareSerial RX TX pin of arduino UNO and BMduino
//BME63M001 myTDS(&Serial1);//HardwareSerial of BMduino BMCOM1
//BME63M001 myTDS(&Serial2);//HardwareSerial of BMduino BMCOM2
//BME63M001 myTDS(&Serial3);//HardwareSerial of BMduino BMCOM3
void setup() 
{
    myTDS.begin();
    Serial.begin(115200);
    delay(10);      
}

void loop() 
{               
    delay(5);
    if(myTDS.reset() == CHECK_OK)
    {
        Serial.println("modular has reset"); 
    }
    else
    {
        Serial.println("Fail to reset"); 
    } 
    while(1);                                                                   
}
