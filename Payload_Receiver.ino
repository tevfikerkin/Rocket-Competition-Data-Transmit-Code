
// include the library
#include <LoRaLib.h>

// create instance of LoRa class using SX1278 module
// this pinout corresponds to RadioShield
// https://github.com/jgromes/RadioShield
// NSS pin:   10 (4 on ESP32/ESP8266 boards)
// DIO0 pin:  2
// DIO1 pin:  3
SX1276 lora = new LoRa(10, 15, 14); // yedek kart için nss = 10, DIO0 = 15, DIO1 = 14 (10,15,14)

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
  lora.setSpreadingFactor(10);
  lora.setCodingRate(8);
  lora.setSyncWord(0x25);
  lora.setOutputPower(17);
  lora.setCurrentLimit(100);
  lora.setPreambleLength(8);
  lora.setGain(0);
  /*
/*
  if (lora.setBandwidth(7.8) == ERR_INVALID_BANDWIDTH) {
    Serial.println(F("Selected bandwidth is invalid for this module!"));
    while (true);
  }

  if (lora.setSpreadingFactor(12) == ERR_INVALID_SPREADING_FACTOR) {
    Serial.println(F("Selected spreading factor is invalid for this module!"));
    while (true);
  }

  if (lora.setCodingRate(8) == ERR_INVALID_CODING_RATE) {
    Serial.println(F("Selected coding rate is invalid for this module!"));
    while (true);
  }

  if (lora.setFrequency(434.0) == ERR_INVALID_FREQUENCY) {
    Serial.println(F("Selected frequency is invalid for this module!"));
    while (true);
  }

  if (lora.setSyncWord(0x10) != ERR_NONE) {
    Serial.println(F("Unable to set sync word!"));
    while (true);
  }

  if (lora.setOutputPower(17) == ERR_INVALID_OUTPUT_POWER) {
    Serial.println(F("Selected output power is invalid for this module!"));
    while (true);
  }

  if (lora.setCurrentLimit(100) == ERR_INVALID_CURRENT_LIMIT) {
    Serial.println(F("Selected current limit is invalid for this module!"));
    while (true);
  }

  if (lora.setPreambleLength(8) == ERR_INVALID_PREAMBLE_LENGTH) {
    Serial.println(F("Selected preamble length is invalid for this module!"));
    while (true);
  }

  if (lora.setGain(0) == ERR_INVALID_GAIN) {
    Serial.println(F("Selected gain is invalid for this module!"));
    while (true);
  }
  if (lora.setCRC(false) != ERR_NONE) {
    Serial.println(F("Unable to set CRC!"));
    while (true);
  }
*/
  Serial.println(F("All settings successfully changed!"));
  digitalWrite(buzzer, HIGH);

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
