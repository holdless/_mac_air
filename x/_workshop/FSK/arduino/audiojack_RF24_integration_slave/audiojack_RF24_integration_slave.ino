#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10
byte pinLed = 8;

// NOTE: the "LL" at the end of the constant is "LongLong" type
//const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D3LL };
const int _DEVICE = 0;

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int joystick[2];  // 2 element array holding Joystick readings
/////
char myString[20];
void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
//21041103  
//    radio.enableDynamicPayloads() ;
//  radio.enableDynamicAck();

  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  
  radio.openWritingPipe(pipes[1]);
    
  pinMode(pinLed, OUTPUT);

}//--(end setup )---

//int receiveCnt = 0;
void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
//  radio.stopListening();
//  radio.write("check", sizeof("check"));
  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
//    while (!done)
    while (radio.available())
    {
      // Fetch the data payload
//      done = radio.read( myString, sizeof(myString) );
      radio.read( myString, sizeof(myString) );
    }
    
    Serial.println(myString);
      
    // 20141105, add home srv communication protocol (first 2 char s,d)
    char sd[2],commandString[18];
    byte j = 0;
    for (int i = 0; i < sizeof(myString); i++)
    {
      if (i < 2)
      {
        sd[i] = myString[i];
      }
      else if (i >= 2)
      {
        commandString[j] = myString[i];
        j++;
      }
    }
    
    if ((sd[1]>>_DEVICE & 0x01))
    {
      Serial.println(sd[1]>>_DEVICE & 0x01);
      Serial.println(commandString);

      if (!strcmp(commandString, "ON"))
      {
        digitalWrite(pinLed, HIGH);
      }
      else if (!strcmp(commandString, "OFF"))
      {
        digitalWrite(pinLed, LOW);
      }

      // First, stop listening so we can talk
      radio.stopListening();
      delay(100);

      // add response string here,
      // now i reply nothing but original string to master
      radio.write(myString, sizeof(myString), true);
      radio.startListening();

      // set different delay time for different client to response to serever
      delay(10);
    }
  }
  else
  {    
//      Serial.println("No radio available");
  }

}
