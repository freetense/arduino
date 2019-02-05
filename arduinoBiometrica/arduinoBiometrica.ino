/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(11, 10);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup()  
{
  Serial.begin(57600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Найден датчик отпечатков пальцев!");
  } else {
    Serial.println("Не нашел датчик отпечатков пальцев :(");
    while (1) { delay(1); }
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{
  Serial.println("Готов записаться отпечаток!");
  Serial.println("Пожалуйста, введите ID # (от 1 до 127), который вы хотите сохранить в этом палец, как .");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Ожидание действительного пальца для регистрации в качестве #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Сделано изображение");
      break;
    case FINGERPRINT_NOFINGER:
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Ошибка связи");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Ошибка изображения");
      break;
    default:
      Serial.println("Неизвестная ошибка");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Изображение конвертировано");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Изображение слишком грязное");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Ошибка связи");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Не удалось найти функции отпечатков пальцев");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Не удалось найти функции отпечатков пальцев");
      return p;
    default:
      Serial.println("Неизвестная ошибка");
      return p;
  }
  
  Serial.println("Удалить палец");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Поместите тот же палец снова");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Сделано изображение");
      break;
    case FINGERPRINT_NOFINGER:
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Ошибка связи");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Ошибка изображения");
      break;
    default:
      Serial.println("Неизвестная ошибка");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Изображение конвертировано");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Изображение слишком грязное");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Ошибка связи");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Не удалось найти функции отпечатков пальцев");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Не удалось найти функции отпечатков пальцев");
      return p;
    default:
      Serial.println("Неизвестная ошибка");
      return p;
  }
  
  // OK converted!
  Serial.print("Создание модели для #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Совместимые принты!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Ошибка связи");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Отпечатки пальцев не совпадают");
    return p;
  } else {
    Serial.println("Неизвестная ошибка");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Хранится!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Ошибка связи");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Не удалось сохранить в этом месте");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Ошибка при записи");
    return p;
  } else {
    Serial.println("Неизвестная ошибка");
    return p;
  }   
}
