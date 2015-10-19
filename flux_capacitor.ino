//#include <pcmConfig.h>
//#include <pcmRF.h>
#include <TMRpcm.h>

#include <SD.h>
#include <SPI.h>


/*
  Flux Capacitor
  Lights up LEDs in a sequence to look like the flux capacitor.

  October 16, 2015
  by Thaddeus Bort, Jr
 */

#define SD_CHIP_SELECT_PIN 10

TMRpcm audio; // an object for playing sounds

const long interval = 120;
const byte ledGroups[4] = { A0, A1, A2, A3 };

bool ledEnabled = true;
unsigned long previousMillis = 0;        // will store last time LED was updated
byte currentLedGroup = 0;

void setup() {
  Serial.begin(9600);
  for(byte i=0; i<sizeof(ledGroups); i++) {
    pinMode(ledGroups[i], OUTPUT);
  }

  // initialize
  audio.speakerPin = 9;
    
  // setup SD-card
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CHIP_SELECT_PIN)) {
    Serial.println(" failed!");
  } else {
    Serial.println(" done.");

    audio.volume(1);
    audio.play("twinkle.wav");
    //char info[32];
    //audio.listInfo("song.wav",info,0);
    //Serial.print(info);Serial.println(":"); 
    Serial.println("playing...");
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if(Serial.available() > 0 && Serial.read() == 'o') {
    ledEnabled = !ledEnabled;
    if(!ledEnabled)
      turnAllLedsOff();
  }
 
  if(ledEnabled && (currentMillis - previousMillis >= interval)) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;

    // turn current led off and next led on
    digitalWrite(ledGroups[currentLedGroup], LOW);
    if(++currentLedGroup >= sizeof(ledGroups))
      currentLedGroup = 0;
    digitalWrite(ledGroups[currentLedGroup], HIGH);
  }
}

void turnAllLedsOff() {
  for(byte i=0; i<sizeof(ledGroups); i++) {
    digitalWrite(ledGroups[i], LOW);
  }
  currentLedGroup = 0;
  previousMillis = 0;
}

