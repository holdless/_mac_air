#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "pitches.h"

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
// Define the transmit pipe
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL, 0xF0F0F0F0D3LL };

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int photocellPin = 2; // 光敏電阻 (photocell) 接在 anallog pin 2
int varResistorPin = 3;
int photocellVal = 0; // photocell variable
int varResistorVal = 0;
char radioOut[2] = {0};

int soundPin = 7;

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.openReadingPipe(2,pipes[2]);
  radio.startListening();
}

void loop() 
{
  photocellVal = analogRead(photocellPin);
  varResistorVal = analogRead(varResistorPin);
  
  Serial.println(varResistorVal - 511);
  if (photocellVal <  varResistorVal - 511)
  {
    // write to radio slave
    radio.stopListening();
    radioOut[0] = '1';
    radio.write( radioOut, sizeof(radioOut), true );
  }
  else
  {
    // write to radio slave
    radio.stopListening();
    radioOut[0] = '0';
    radio.write( radioOut, sizeof(radioOut), true );
    playSound();
  }
//  Serial.println(radioOut[0]);
//  Serial.println(varResistorVal);
  delay(500);
}

void playSound()
{
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(soundPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(soundPin);
  }  
}
