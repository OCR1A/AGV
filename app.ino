#include "BluetoothSerial.h"
#define freq 10000
#define resolution 8

#define wfrChannel 0
#define wfrPwm 34
#define wfra 27
#define wfrb 33

#define wflChannel 1
#define wflPwm 23
#define wfla 2
#define wflb 4

#define wbrChannel 2
#define wbrPwm 13
#define wbra 21
#define wbrb 19
#define wbrEnable 12

#define wblChannel 3
#define wblPwm 32
#define wbla 18
#define wblb 26
#define wblEnable 17

// Check if Bluetooth configurations are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

BluetoothSerial ESP_BT; //Object for Bluetooth

int16_t wfr, wfl, wbr, wbl;
bool wfrNeg, wflNeg, wbrNeg, wblNeg;

void setup() {
  Serial.begin(115200);
  ESP_BT.begin("ESP32_BT");
  
  //Enable
  pinMode(wbrEnable, OUTPUT);
  pinMode(wblEnable, OUTPUT);
  digitalWrite(wbrEnable, HIGH);
  digitalWrite(wblEnable, HIGH);
  
  pinMode(wblPwm, OUTPUT);
  pinMode(wbla, OUTPUT);
  pinMode(wblb, OUTPUT);

  pinMode(wbrPwm, OUTPUT);
  pinMode(wbra, OUTPUT);
  pinMode(wbrb, OUTPUT);

  pinMode(wflPwm, OUTPUT);
  pinMode(wfla, OUTPUT);
  pinMode(wflb, OUTPUT);

  pinMode(wfrPwm, OUTPUT);
  pinMode(wfra, OUTPUT);
  pinMode(wfrb, OUTPUT);

  //Reverso
  ledcSetup(wbrChannel, freq, resolution);  
  ledcAttachPin(wbrPwm, wbrChannel);
  ledcSetup(wblChannel, freq, resolution);
  ledcAttachPin(wblPwm, wblChannel);

}


void loop() {
      if(ESP_BT.available()>=9){
        if(ESP_BT.read()=='H'){
          wfr=((int16_t)ESP_BT.read())|(((int16_t)ESP_BT.read())<<8);
          wfl=((int16_t)ESP_BT.read())|(((int16_t)ESP_BT.read())<<8);
          wbr=((int16_t)ESP_BT.read())|(((int16_t)ESP_BT.read())<<8);
          wbl=((int16_t)ESP_BT.read())|(((int16_t)ESP_BT.read())<<8);

          Serial.print("wfl= ");
          Serial.println(wfl);
          Serial.print("wfr= ");
          Serial.println(wfr);
          Serial.print("wbl= ");
          Serial.println(wbl);
          Serial.print("wbr= ");
          Serial.println(wbr);

          if(wfr>0){
            digitalWrite(wfra, LOW);
            digitalWrite(wfrb, HIGH);
          }else if(wfr<0){
            digitalWrite(wfra, HIGH);
            digitalWrite(wfrb, LOW);
          }else if(wfr==0){
            digitalWrite(wfra, LOW);
            digitalWrite(wfrb, LOW);
          }

          if(wfl>0){
            digitalWrite(wfla, HIGH);
            digitalWrite(wflb, LOW);
          }else if(wfl<0){
            digitalWrite(wfla, LOW);
            digitalWrite(wflb, HIGH);
          }else if(wfl==0){
            digitalWrite(wfla, LOW);
            digitalWrite(wflb, LOW);
          }

          if(wbr<0){
            wbrNeg=true;
          }else{
            wbrNeg=false;
          }//else

          wbr=abs(wbr);
          if(wbrNeg){
            Serial.print("WBR NEGATIVO");
            digitalWrite(wbra,HIGH);
            digitalWrite(wbrb,LOW);
          }else{
            Serial.print("WBR Positivo");
            digitalWrite(wbra,LOW);
            digitalWrite(wbrb,HIGH);
          }//else
          ledcWrite(wbrChannel,wbr);

          if(wbl<0){
            wblNeg=true;
          }else{
            wblNeg=false;
          }//else

          wbl=abs(wbl);
          if(wblNeg){
            Serial.print("WBL negativo");
            digitalWrite(wbla,HIGH);
            digitalWrite(wblb,LOW);
          }else{
            Serial.print("WBL positivo");
            digitalWrite(wbla,LOW);
            digitalWrite(wblb,HIGH);
          }//else
          ledcWrite(wblChannel,wbl);
    
        }else{
          Serial.read();
        }
      }

  delay(20);
}