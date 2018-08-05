#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial ss(10, 11);
void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
}
void loop()
{
  float flat, flon;
  unsigned long age;
  int year;
  byte month, day, hour, minute, second, hundredths;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  gps.f_get_position(&flat, &flon, &age);
 if(TinyGPS::GPS_INVALID_F_ANGLE != flat){
  Serial.print(flat, 6);
  Serial.print(" ");
  Serial.print(flon, 6);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute); 
 }
  smartdelay(1000);
}
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
