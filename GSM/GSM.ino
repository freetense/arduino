#include <SoftwareSerial.h>

String value;
SoftwareSerial ss(7, 8); // RX, TX
void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
}

void loop()
{
char value1 = 'e';
  if(Serial.available() > 0)
{
value = Serial.readString();
delay(5);
ss.println(value);
}
  int c;
  String v;
  while (ss.available()) {  //сохраняем входную строку в переменную v
    c = ss.read();
    v += char(c);
    delay(10);
    value1 = 'i';
  }
  if(value1 == 'i'){
  Serial.println(v);
  }
}
