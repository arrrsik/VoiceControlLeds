#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2,3);   

uint8_t records[7]; // save record
uint8_t buf[64];

int PinBlueLed = 12;
int PinRedLed = 13;
int PinGreenLed = 10;
int PinWhiteLed = 9;

// Макросы которые в коде будут заменены на номера записи команд
#define WhiteLed (0)
#define BlueLed (1) 
#define GreenLed (2) 
#define RedLed (3) 

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
  pinMode(PinBlueLed, OUTPUT);
  pinMode(PinGreenLed, OUTPUT);
  pinMode(PinWhiteLed, OUTPUT);
  pinMode(RedLed, OUTPUT);
    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)BlueLed) >= 0){
    Serial.println("BlueLed");
  }
  
  if(myVR.load((uint8_t)GreenLed) >= 0){
    Serial.println("GreenLed");
  }

    if(myVR.load((uint8_t)WhiteLed) >= 0){
    Serial.println("WhiteLed");
  }

    if(myVR.load((uint8_t)RedLed) >= 0){
    Serial.println("RedLed");
  }
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case WhiteLed:
        digitalWrite(PinWhiteLed, digitalRead(PinWhiteLed) ^ 1);
        break;
      case BlueLed:
        digitalWrite(PinBlueLed, digitalRead(PinBlueLed) ^ 1);
        break;
      case GreenLed:
        digitalWrite(PinGreenLed, digitalRead(PinGreenLed) ^ 1);
        break;
      case RedLed:
        digitalWrite(PinRedLed, digitalRead(PinRedLed) ^ 1);
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }
    printVR(buf);
  }
}



