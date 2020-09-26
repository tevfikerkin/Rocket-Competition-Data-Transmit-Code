
// include the library
#include <LoRaLib.h>


SX1276 lora = new LoRa(14, 15, 16); // yedek kart için nss = 10, DIO0 = 15, DIO1 = 14 (10,15,14)

int counter = 1;
int buzzer = 8;
String temp;
int val = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  // initialize SX1278 with default settings
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

}

void loop() {
  String str;


  int state = lora.receive(str);



  if (state == ERR_NONE) {

    // print data of the packet
    Serial.println(str);
    Serial.println(String(lora.getRSSI()) + " rssı");


  } else if (state == ERR_RX_TIMEOUT) {

  } else if (state == ERR_CRC_MISMATCH) {
    Serial.println(F("CRC error!"));

  }

}
