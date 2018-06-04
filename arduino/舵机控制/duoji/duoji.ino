/*
#include <Arduino.h>

#define PWM_pin 9

int pulsewidth = 0;\
void setup()
{
  pinMode(PWM_pin,OUTPUT);
}

void loop() 
{
  pulse(90);
}

void pulse(int angle)
{
  pulsewidth=int ((angle*11)+500);
  digitalWrite(PWM_pin,HIGH);
  delayMicroseconds(pulsewidth);
  digitalWrite(PWM_pin,LOW);
  delay(20-pulsewidth/1000);
}
*/
#include <Arduino.h>
#include <Servo.h>   //引入lib
Servo myservo;       //创建一个伺服电机对象
#define potpin A5    //设定连接可变电阻的模拟引脚
int val;             //创建变，储存从模拟端口读取的（0到102）
void setup()
{
  myservo.attach(9);  //9号引脚输出电机控制信号  注：仅能使用9、10号引脚
 }
 void loop()
 {
   val = analogRead(potpin);
   //读取来自可变电阻的模拟值(0到1023之间)
   val = map(val,0,1023,0,179);  //利用"map"函数缩放该，得到伺服电机需要的角度(0到180之间)
   myservo.write(val);         //设置伺服电机的位置
   delay(15);              //等待电机旋转到目标角度
  }
