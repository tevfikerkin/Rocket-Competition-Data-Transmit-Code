
#include <LoRaLib.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

SX1276 lora = new LoRa(8, 7, 6); // yedek kart için nss = 10, DIO0 = 15, DIO1 = 14 (10,15,14)

Adafruit_BMP280 bmp;

double Minute = 60; // yere indikten kaç dakika sonra alıcı modül haline alması gerekliyse dakika cinsinden değer girilecek.

String ltd, lngt, sat, strAlt, data, indicator, choose = "sender";

String password = "neo", str;
int counter = 1, buzzer = 3, state, i, refAlt, preAlt, alt, apogee = 0, higher; // "apogee = 0" ise tepe noktasına ulaşılmamıştır. "higher" maksimum irtifayı gösterecektir.

int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);



void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  ss.begin(GPSBaud);

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  Serial.print(F("Initializing ... "));
  int state = lora.begin();
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
  lora.setFrequency(915);
  lora.setBandwidth(250);
  lora.setSpreadingFactor(9);
  lora.setCodingRate(8);
  lora.setSyncWord(0x13);
  lora.setOutputPower(17);
  lora.setCurrentLimit(100);
  lora.setPreambleLength(15);
  lora.setGain(0);

  Serial.println(F("All settings successfully changed!"));
  digitalWrite(buzzer, HIGH);

}

void loop() {

  GPSData();
  AltData();
  data = CombineData( ltd,  lngt,  strAlt,  sat, String(counter));



  if (millis() < millisToMinute(Minute) && apogee == 0 ) {


    int state = lora.transmit(data);//sending latitude
    if (state == ERR_NONE) {
      // the packet was successfully transmitted
      Serial.println(F(" success!"));


    } else if (state == ERR_PACKET_TOO_LONG) {
      Serial.println(F(" too long!"));

    } else if (state == ERR_TX_TIMEOUT) {
      Serial.println(F(" timeout!"));

    }
    Serial.println("Sender Mode is Activated!");
  }

  else { // tepe noktasına ulaşmış ve yere inmişse (apogee == 1) ek olarak 2 dakika geçmiş ise receiver olacak.

    int state = lora.receive(str);

    if (state == ERR_NONE) {

      Serial.print(F("Data: "));
      Serial.println(str);


    } else if (state == ERR_RX_TIMEOUT) {

    } else if (state == ERR_CRC_MISMATCH) {
      Serial.println(F("CRC error!"));

    }

    if (str == "1")
    {
      digitalWrite(buzzer, LOW);
    }
    else
    { digitalWrite(buzzer, HIGH);
    }
    Serial.println("Receiver Mode is Activated!");
  }
  counter++;

  Serial.println("\n---------------------------------------------------------\n");
  Serial.print("Ref Altitude:"); Serial.println(refAlt);
  Serial.print("Reserved Time in Minute:"); Serial.println(millisToMinute(Minute));
  Serial.print("Altitude:"); Serial.println(alt);
  Serial.print("Latitude:"); Serial.println(ltd);
  Serial.print("Longitude: "); Serial.println(lngt);
  Serial.print("Counter: "); Serial.println(counter);

}

void AltData() {

  if (i < 5) {
    refAlt = bmp.readAltitude(1013.25);
  }

  alt = bmp.readAltitude(1013.25);

  if (alt > preAlt) {
    higher = alt;
  }
  else if (higher - alt >= 5) {
    higher = preAlt;
    apogee = 1;
  }

  strAlt = String ( (alt - refAlt) );
  preAlt = alt;
  i++;

}

void GPSData() {

  ltd = String (gps.location.lat(), 6);
  lngt = String (gps.location.lng(), 6);
  sat = String (gps.satellites.value());

  smartDelay(50);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

String CombineData(String ltd, String lngt, String alt, String sat, String counter) {

  String comma = ",";
  String data = ltd + comma + lngt + comma + alt + comma + sat + comma + counter;
  return data;
}

unsigned long millisToMinute(double Minute) { // dakikayı

  return (60 * Minute * 1000);

}

