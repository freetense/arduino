#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>

 
void setup(){
  Serial.begin(9600);

  Mirf.spi = &MirfHardwareSpi; 
  Mirf.cePin = 10;
  Mirf.csnPin = 9;
  Mirf.init();
 
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = 12;
  Mirf.channel = 66;
  Mirf.config();
  Serial.println("запуск ...");
}
void loop() {
  byte data[12];
  // Ждём данных
  if(!Mirf.isSending() && Mirf.dataReady()){
    Serial.println("Пришёл пакет");
    Mirf.getData((byte *) &data);
    Mirf.setTADDR((byte *)"clie1");
    int i;

    for(i = 0;i<sizeof(data);i++){
      Serial.write(byte(data[i]));
    }
    Serial.println();
  }
  delay(10);
}
