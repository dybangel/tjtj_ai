int val11;
int val2;
void setup() {
 // put your setup code here, to run once:
pinMode(LED_BUILTIN,OUTPUT);
Serial.begin(9600);
Serial.println("Emartee.Com");
Serial.println("Voltage:");
Serial.print("V");
}

void loop() {
 // put your main code here, to run repeatedly:
float temp;
val11=analogRead(1);
temp=val11/4.092;
val11=(int)temp;//
val2=((val11%100)/10);
Serial.println(val2);
delay(500);
if(val2<2)  
{  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(100);                       // wait for a second
}  
}
