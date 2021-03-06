#include<Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "SHTSensor.h"
SHTSensor sht;//温湿度传感器的对象
int BH1750address=0x23;//光强传感器地址
byte buff[2];
int UVOUT = A0; //Output from the sensor
int REF_3V3 = A1; //3.3V power on the Arduino board
unsigned int time=0;
float Lux,Tem,Hum,Uv,UVMAX=0;
float X=0,Y=0,Z=0;  //温湿度X是温度，Y是湿度，Z是紫外线

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

 union temp
{
  float Float;
  byte Byte[4];
}temp1;

void setup() {
  u8g2.setI2CAddress(0x78);//屏幕地址
  u8g2.begin();
  Wire.begin();
}

void loop() {
  firstDisplay();
  u8g2.setFont(u8g2_font_victoriabold8_8r);
  while(1){
  u8g2.clearBuffer();
  u8g2.drawFrame(4,4,120,56);
  u8g2.drawHLine(4,32,120);
<<<<<<< HEAD:Fianl/master/master.ino
  u8g2.drawVLine(64,4,56);  //以上是屏幕初始化
  int L=0;
  tem_hum(X,Y);         //读取温湿度X是温度，Y是湿度
  uv(Z);            //读取紫外线指数
  L=light();          //读取光强
  Trans();          //传输光强
  if(UVMAX<Z)         //以下是屏幕显示
{
=======
  u8g2.drawVLine(64,4,56);
  float X,Y,Z,L;
  tem_hum(X,Y); //X是 温度，Y是湿度
  uv(Z); //紫外线指数
  L=light();
  Trans(L);
  if(UVMAX<Z){
>>>>>>> fbb4e21a897d8417279d5b63020e6a6ca6c39ab8:Fianl/final/final/final.ino
    UVMAX=Z;
  }
  u8g2.setCursor(6,13);
  u8g2.print("Temp:");
  u8g2.setCursor(6,26);
  u8g2.print(X);
  
  u8g2.setCursor(66,13);
  u8g2.print("Hum:");
  u8g2.setCursor(66,26);
  u8g2.print(Y);
  
  u8g2.setCursor(6,41);
  u8g2.print("Lux:");
  u8g2.setCursor(6,54);
  u8g2.print(L);
  
  u8g2.setCursor(66,41);
  u8g2.print("UV:");
  u8g2.setCursor(66,54);
  u8g2.print(UVMAX);
  u8g2.sendBuffer();
  Serial.print(X);
  Serial.print(Y);
  Serial.print(L);
  Serial.print(Z);
  delay(2000);
  }
}

//HXCL logo
void firstDisplay(){
  u8g2.clear();
  u8g2.setFont(u8g2_font_maniac_tr);
  u8g2.drawBox(4,4,120,56);
  u8g2.setDrawColor(0);
  u8g2.setFontMode(1);
  u8g2.drawStr(25,40,"HXCL");
  u8g2.sendBuffer();
  u8g2.setDrawColor(1);
  delay(1500);
}

<<<<<<< HEAD:Fianl/master/master.ino
void Trans()             //传输函数
{
  Wire.beginTransmission(0x99);//括号内补充从机地址
  Wire.write(buff [0]);
  Wire.write(buff [1]);
  temp1.Float=X;
  for(int i=0;i<4;i++)
  {
    Wire.write(temp1.Byte[i]);
  }
  temp1.Float=Y;
  for(int i=0;i<4;i++)
  {
    Wire.write(temp1.Byte[i]);
  }
  temp1.Float=Z;
  for(int i=0;i<4;i++)
  {
    Wire.write(temp1.Byte[i]);
  }
  Wire.endTransmission();
}
void tem_hum(float &x,float &y)   //获取温湿度函数
=======
void Trans(float L)
{
  Wire.beginTransmission(0x99);//括号内补充从机地址
  Wire.write(L);
  Wire.endTransmission();
}
void tem_hum(float &x,float &y)
>>>>>>> fbb4e21a897d8417279d5b63020e6a6ca6c39ab8:Fianl/final/final/final.ino
{
      if (sht.init());
      sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM); 
      if (sht.readSample())
  {
     y=sht.getHumidity();x=sht.getTemperature();
  }
}

void uv(float &uvIntensity)       //获取紫外线函数
{
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  
  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;
  
  //uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);
  uvIntensity = (outputVoltage - 0.99) * 15.0 / 1.91;
}

//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 16;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

int light(){                //获取光强函数
  int i;
  double lux=0;
  
  BH1750_Init(BH1750address);

  if(2==BH1750_Read(BH1750address)){
    lux=((buff[0]<<8)|buff[1])/1.2;     //数据处理
  }
  return lux;
}

int BH1750_Read(int address){
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);

  while(Wire.available()){
    buff[i] = Wire.read();  // receive one byte
    i++;
  }

  Wire.endTransmission();  

  return i;
}

void BH1750_Init(int address){
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}
