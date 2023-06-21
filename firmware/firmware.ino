







/*************************************************************************
* File Name          : Firmware.ino
* Author             : Ander
* Updated            : Ander
* Modded ESP32       : Jesus Romero
* Version            : V1.10101
* Date               : 16/06/2023
* Description        : Firmware for Makeblock Electronic modules with Scratch.  
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 - 2014 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
**************************************************************************/
#include <ESP32_Servo.h>
#include "BluetoothSerial.h"
#include <NonBlockingRtttl.h>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

Servo servo;
BluetoothSerial BT;
  

typedef struct MeModule
{
    int device;
    int port;
    int slot;
    int pin;
    int indice;
    float values[3];
} MeModule;

union{
    byte byteVal[4];
    float floatVal;
    long longVal;
}val;

union{
  byte byteVal[8];
  double doubleVal;
}valDouble;

union{
  byte byteVal[2];
  short shortVal;
}valShort;

String mVersion = "1.1.102";
boolean isAvailable = false;
boolean isBluetooth = false;

int len = 52;
char buffer[52];
char bufferBt[52];
byte indice = 0;
byte dataLen;
byte modulesLen=0;
boolean isStart = false;
unsigned char irRead;
char serialRead;
uint8_t command_index = 0;
char vectorCaracteres[504];
int j_viejo = 0;
#define VERSION 0
#define ULTRASONIC_SENSOR 1
#define TEMPERATURE_SENSOR 2
#define LIGHT_SENSOR 3
#define POTENTIONMETER 4
#define JOYSTICK 5
#define GYRO 6
#define SOUND_SENSOR 7
#define RGBLED 8
#define SEVSEG 9
#define MOTOR 10
#define SERVO 11
//#define ENCODER 12
#define IR 13
#define PIRMOTION 15
#define INFRARED 16
#define LINEFOLLOWER 17
#define SHUTTER 20
#define LIMITSWITCH 21
#define BUTTON 22
#define DIGITAL 30
#define ANALOGICAL 31
#define PWM 32
#define SERVO_PIN 33
#define TONE 34
#define ULTRASONIC_ARDUINO 36
#define STEPPER 40
#define ENCODER 41
#define TIMER 50
#define TOUCH 63
#define BLUET 64
#define MRTTTL 65

#define GET 1
#define RUN 2
#define RESET 4
#define START 5


float angleServo = 90.0;
unsigned char prevc=0;
double lastTime = 0.0;
double currentTime = 0.0;



void setup(){
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  delay(300);
  digitalWrite(2,LOW);
  Serial.begin(115200);
  BT.begin("ESP32_BT"); //Nombra el dispositivo bluetooth para que nos conectemos 

}
void loop(){
  currentTime = millis()/1000.0-lastTime;
  irRead = 0;
  readSerial();
  if(isAvailable){
    unsigned char c = serialRead&0xff; //grabamos serial read en la variable c
    if(c==0x55&&isStart==false){ //comprobamos que entra la cabecera de un mensaje 0xff 0x55
     if(prevc==0xff){
      indice=1;
      isStart = true;
     }
    }else{
      prevc = c;                //va metiendo los datos para configurar el paquete
      if(isStart){
        if(indice==2){
         dataLen = c; 
        }else if(indice>2){
          dataLen--;
        }
        writeBuffer(indice,c);
      }
    }
     indice++;
     if(indice>51){
      indice=0; 
      isStart=false;
     }
     if(isStart&&dataLen==0&&indice>3){ //comprueba que despues de la cabecera lleguen datos y cuando no, termina el paquete y llama a parseData
        isStart = false;
        parseData(); 
        indice=0;
     }
  }
}
unsigned char readBuffer(int indice){
 return isBluetooth?bufferBt[indice]:buffer[indice]; 
}
void writeBuffer(int indice,unsigned char c){
 if(isBluetooth){
  bufferBt[indice]=c;
 }else{
  buffer[indice]=c;
 } 
}
void writeHead(){
  writeSerial(0xff);
  writeSerial(0x55);
}
void writeEnd(){
 Serial.println(); 
}
void writeSerial(unsigned char c){
 Serial.write(c);

}
void readSerial(){
  isAvailable = false;
  if(Serial.available()>0){
    isAvailable = true;
    isBluetooth = false;
    serialRead = Serial.read();
  }  
}
/*
ff 55 len idx action device port  slot  data a
0  1  2   3   4      5      6     7     8
*/
void parseData(){
  isStart = false;
  int idx = readBuffer(3);
  int action = readBuffer(4);
  //BT.println(action);
  int device = readBuffer(5);
  command_index = (uint8_t)idx;
  switch(action){
    case GET:{
        writeHead();
        writeSerial(idx);
        readSensor(device);
        writeEnd();
     }
     break;
     case RUN:{
       runModule(device);
       callOK();
     }
     break;
     case START:{
        //start
        callOK();
      }
     break;
  }
}
void callOK(){
    writeSerial(0xff);
    writeSerial(0x55);
    writeEnd();
}
void sendByte(char c){
  writeSerial(1);
  writeSerial(c);
}
void sendString(String s){
  int l = s.length();
  writeSerial(4);
  writeSerial(l);
  for(int i=0;i<l;i++){
    writeSerial(s.charAt(i));
  }
}
void sendFloat(float value){ 
     writeSerial(0x2);
     val.floatVal = value;
     writeSerial(val.byteVal[0]);
     writeSerial(val.byteVal[1]);
     writeSerial(val.byteVal[2]);
     writeSerial(val.byteVal[3]);
}
void sendShort(double value){
     writeSerial(3);
     valShort.shortVal = value;
     writeSerial(valShort.byteVal[0]);
     writeSerial(valShort.byteVal[1]);
     writeSerial(valShort.byteVal[2]);
     writeSerial(valShort.byteVal[3]);
}
void sendDouble(double value){
     writeSerial(2);
     valDouble.doubleVal = value;
     writeSerial(valDouble.byteVal[0]);
     writeSerial(valDouble.byteVal[1]);
     writeSerial(valDouble.byteVal[2]);
     writeSerial(valDouble.byteVal[3]);
}
short readShort(int idx){
  valShort.byteVal[0] = readBuffer(idx);
  valShort.byteVal[1] = readBuffer(idx+1);
  return valShort.shortVal; 
}
float readFloat(int idx){
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.floatVal;
}


void runModule(int device){
  //0xff 0x55 0x6 0x0 0x1 0xa 0x9 0x0 0x0 0xa 
  int port = readBuffer(6);
  int pin = port;
  switch(device){

   case DIGITAL:{
     pinMode(pin,OUTPUT);
     int v = readBuffer(7);
     digitalWrite(pin,v);
   }
   break;
   case PWM:{
     pinMode(pin,OUTPUT);
     int v = readBuffer(7);
     analogWrite(pin,v);
   }
   case BLUET:{
     // recibo la longitud en buffer(6) y los elementos en los bytes siguientes empezando en buffer(7)
     unsigned long longitud = readBuffer(6);
     for (int indice = 7;indice<longitud+7;indice++) {
        BT.print((char)readBuffer(indice));
     }
     BT.println();
   }  
   break;
   case MRTTTL:{
    // recibo un maximo de 504 caracteres que vendran repartidos en varios paquetes
    // y los almaceno en vectorCaracteres que es global
    pinMode(pin,OUTPUT);    
    int j = readBuffer(8);   //indica el orden del paquete o si es 0 que es el ultimo    
    unsigned long longitud = readBuffer(7);   // indica el tamaño del contenido del paquete
    if (j!=0) {
      j_viejo = j;                      //lo usare para saber el paquete final en el orden de concatenacion
      for (int indice = 9;indice<longitud+9;indice++) {
        vectorCaracteres[(indice +((j-1)*42)) - 9] = (char)readBuffer(indice);
      }
    }
    else {
      for (int indice = 9;indice<longitud+9;indice++) {
        vectorCaracteres[(indice +((j_viejo)*42)) - 9] = (char)readBuffer(indice);
      }
      //vectorCaracteres[longitud +(j_viejo * 42)] = '\0';  //como inicialmente vectorCaracteres solo contiene '\0' no es necesario
      j_viejo = 0;
      rtttl::begin(pin, vectorCaracteres);
      while( !rtttl::done() ) 
        {
      rtttl::play();
        }
      for (int indice = 0;indice<504;indice++){
        vectorCaracteres[indice]='\0';
      }
     
    }
    
   }          
   break;  
   case TONE:{
     pinMode(pin,OUTPUT);
     int hz = readShort(7);
     int ms = readShort(9);
     if(ms>0){
       tone(pin, hz, ms); 
     }else{
       noTone(pin); 
     }
   }
   break;
   case SERVO_PIN:{
     int v = readBuffer(7);
     if(v>=0&&v<=180){
       servo.attach(pin, 500, 2400); // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"
       servo.write(v);
     }
   }
   break;
   case TIMER:{
    lastTime = millis()/1000.0; 
   }
   break;
  }
}
void readSensor(int device){
  /**************************************************
      ff 55 len idx action device port slot data a
      0  1  2   3   4      5      6    7    8
  ***************************************************/
  float value=0.0;
  int port,slot,pin;
  port = readBuffer(6);
  pin = port;
  switch(device){
    case ULTRASONIC_ARDUINO:{
      uint8_t trig = readBuffer(6);
      uint8_t echo = readBuffer(7);
      long pw_data;
      float dis_data;
      pinMode(trig,OUTPUT);
      digitalWrite(trig,LOW);
      delayMicroseconds(2);
      digitalWrite(trig,HIGH);
      delayMicroseconds(10);
      digitalWrite(trig,LOW);
      pinMode(echo, INPUT);
      pw_data = pulseIn(echo,HIGH,30000);
      dis_data = (double)pw_data/58.0;
      delay(5);
      sendFloat(dis_data);
    }
    break;
   case  VERSION:{
     sendString(mVersion);
   }
   break;
   case  DIGITAL:{
     pinMode(pin,INPUT);
     sendFloat(digitalRead(pin));
   }
   break;
   case  ANALOGICAL:{
     //pin = analogs[pin];
     pinMode(pin,INPUT);
     sendShort(analogRead(pin));
   }
   break;

   case  TOUCH:{
     pinMode(pin,INPUT);
     sendShort(touchRead(pin));
   }
   break;

     case  BLUET:{          // aqui es donde miro si alguien a enviado algo por BT al esp32
        switch(pin){        
          case  0:{         
            sendShort(BT.available());
          }
          break;
          case  1:{  
            sendString(BT.readStringUntil('\n'));
          }
          break;         
        }
   }
   break;   
   case TIMER:{
     sendFloat((float)currentTime);
   }
   break;
  }
}
