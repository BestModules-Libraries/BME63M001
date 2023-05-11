/*****************************************************************
File:         sleep
Description:  Put the module into sleep,Please ensure 
              that the baud rate of the Serial Monitor is 115200.
******************************************************************/

#include <BME63M001.h>
#include <SoftwareSerial.h>
BME63M001 myTDS(6,7);//SoftwareSerial RX TX pin of arduino UNO and BMduino
//BME63M001 myTDS(&Serial1);//HardwareSerial of BMduino BMCOM1
void setup() {
    myTDS.begin();
    Serial.begin(115200);
    delay(10);      
}

void loop() {               
    delay(5);
    if(myTDS.sleep() == CHECK_OK)
    {
        Serial.println("modular has shut off"); 
    }
    else
    {
        Serial.println("Fail to shut off"); 
    } 
    while(1);                                                                   
}
