#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
//const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D3LL };
/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int ledPin = 6;

char radioIn[2] = {0};

void setup() 
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  
  radio.begin();
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
}

int light = 1;
void loop() 
{
  boolean done = false;
  
  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;

    while (!done)
    {
      // Fetch the data payload
      radio.read( radioIn, sizeof(radioIn) );
      done = true;
    Serial.println(radioIn);
    }
    
    Serial.println(radioIn);
      
  }
  else
  {    
      Serial.println("No radio available");
  }

  if (radioIn[0] == '0')
  {
    light = -light;
    digitalWrite(ledPin, light > 0);
//    playSound();
  }
  else
  {
    digitalWrite(ledPin, 0);
  }
  delay(500);
}


