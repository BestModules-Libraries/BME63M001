/*****************************************************************
File:        readTDS
Description: power on to obtain the current TDS1/NTC1 values directly.
             After successful reception, print the current TDS1/NTC1 
             value every second in the serial port,Please ensure that 
			       the baud rate of the Serial Monitor is 115200.
******************************************************************/
#include <BME63M001.h>
BME63M001 myTDS(6,7);//SoftwareSerial RX TX pin of arduino UNO and BMduino
//BME63M001 myTDS(&Serial1);//HardwareSerial of BMduino BMCOM1
//BME63M001 myTDS(&Serial2);//HardwareSerial of BMduino BMCOM2
//BME63M001 myTDS(&Serial3);//HardwareSerial of BMduino BMCOM3
uint8_t channel;/*TDS chanel*/
uint16_t FWVer;
float TDSValue ;
float TempValue ;

void setup() 
{
    myTDS.begin();
    Serial.begin(115200);
    FWVer = myTDS.getFWVer();          
    Serial.print("F/W version is V");/*print module version*/
    Serial.print(FWVer/256);
    Serial.print(".");
    Serial.println(FWVer%256);      
          
}

void loop() 
{      
    channel = 1;   //CH1
    TDSValue  = myTDS.readTDS(channel) ;
    TempValue = myTDS.readTemperature(channel);
    Serial.print("TDS");
    Serial.print(channel);   
    Serial.print(" value is ");
    Serial.print(TDSValue,1);  
    Serial.print(" ppm");
    Serial.print("(");
    Serial.print(TDSValue*2,1);  
    Serial.println(" uS/cm)");       

    Serial.print("temperature");
    Serial.print(channel);
    Serial.print(" value is ");                    
    Serial.print(TempValue,1); 
    Serial.println(" ℃");  
    delay(1000);                                
}
 
