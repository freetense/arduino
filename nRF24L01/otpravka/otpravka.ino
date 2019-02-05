#include <SPI.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
 
void setup(){
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.cePin = 10;
  Mirf.csnPin = 9;

  Mirf.init();
 
  Mirf.setRADDR((byte *)"clie1");
  Mirf.channel = 66;
  Mirf.payload = 12;
  Mirf.config();
  Serial.println("Старт ... ");
}
 
void loop() {
    Mirf.init();
 
  Mirf.setRADDR((byte *)"clie1");
  Mirf.channel = 66;
  Mirf.payload = 12;
  Mirf.config();
  Mirf.setTADDR((byte *)"serv1");
  Mirf.send((byte *)"пиздец");
  Serial.println("пиздец");
  delay(3000);
  Serial.println("-----------------------------------------");

}
