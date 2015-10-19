#include <TMRpcm.h>
#include <SD.h>
#include <SPI.h>

/*
  Flux Capacitor
  Lights up LEDs in a sequence to look like the flux capacitor.

  Started October 16, 2015
  by Thaddeus Bort, Jr
 */

// sd card pinout
// CS 10
// MOSI 11
// MISO 12
// SCK 13
#define SD_CHIP_SELECT_PIN 10

TMRpcm audio; // an object for playing sounds

#define DEFAULT_INTERVAL 50
long interval = DEFAULT_INTERVAL;
const byte ledGroups[4] = { A0, A1, A2, A3 };
const byte timeTravelLeds[2] = { A4, A5 };

bool ledEnabled = true;
unsigned long previousMillis = 0;        // will store last time LED was updated
byte currentLedGroup = 0;
bool currentState = false;

unsigned long previousTimeTravelMillis = 0;
bool timeTravelMode = false;

void setup() {
  Serial.begin(9600);
  for(byte i=0; i<sizeof(ledGroups); i++) {
    pinMode(ledGroups[i], OUTPUT);
  }
  for(byte i=0; i<sizeof(timeTravelLeds); i++) {
    pinMode(timeTravelLeds[i], OUTPUT);
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

  if(!timeTravelMode && (currentMillis-previousTimeTravelMillis >= 300000)) {
    timeTravelMode = true;
  }
 
  if(ledEnabled && (currentMillis - previousMillis >= interval)) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;

    // cycle through leds, turning on and then off and then moving to next led
    digitalWrite(ledGroups[currentLedGroup], currentState ? LOW : HIGH);
    currentState = !currentState;
    if(!currentState && ++currentLedGroup >= sizeof(ledGroups))
      currentLedGroup = 0;

    if(timeTravelMode) {
      if(currentLedGroup == 3 && !currentState) {
        digitalWrite(timeTravelLeds[0], HIGH);
        digitalWrite(timeTravelLeds[1], HIGH);
      } else {
        digitalWrite(timeTravelLeds[0], LOW);
        digitalWrite(timeTravelLeds[1], LOW);
      }
      if(--interval <= 0) {
        previousTimeTravelMillis = currentMillis;
        timeTravelMode = false;
        interval = DEFAULT_INTERVAL;
      }
    }
  }
}

void turnAllLedsOff() {
  for(byte i=0; i<sizeof(ledGroups); i++) {
    digitalWrite(ledGroups[i], LOW);
  }
  currentLedGroup = 0;
  currentState = false;
  previousMillis = 0;
}

