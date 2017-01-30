#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// audio jack
#include <SoftModem.h> //ライブラリをインクルード
#include <ctype.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
// Define the transmit pipe
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL, 0xF0F0F0F0D3LL };

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
SoftModem modem; //SoftModemのインスタンスを作る

/*-----( Declare Variables )-----*/
//int joystick[2];  // 2 element array holding Joystick readings
char radioInMsg[20];
char audioInMsg[20];

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  radio.begin();
//  radio.openWritingPipe(pipe);
  modem.begin(); // setup()でbeginをコールする

//  pinMode(ledPin, OUTPUT);

// 20141103
//  radio.enableDynamicPayloads() ;
//  radio.enableDynamicAck();

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.openReadingPipe(2,pipes[2]);
  radio.startListening();

}//--(end setup )---

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  //... audio jack part ...
  // sendback message to iPhone via FSK
  while(Serial.available())
  { //PCからデータを受信しているか確認
    char c = Serial.read(); //1byteリード
    modem.write(c); //iPhoneに送信
    
    Serial.print(c);
  }

  // read message from iPhone via FSK
  char c[1] = {0};
  byte k = 0;
  bool stringDone = false;
  
  while (c[0] != '\r' && c[0] != '\n')
  {
    // read from iPhone
    while(modem.available())
    { //iPhoneからデータ受信しているか確認
      c[0] = modem.read(); //1byteリード
      if (c[0] != '\r' && c[0] != '\n')
      {
        audioInMsg[k] = c[0];
        k++;
        stringDone = true;
//        Serial.println(c[0]);
      }
    }
  }
    
  if (stringDone)
  {
    Serial.print("Command from iPhone: ");
    Serial.println(audioInMsg);
    
    // write to radio slave
    radio.stopListening();
    radio.write( audioInMsg, sizeof(audioInMsg), true );
    radio.startListening();
        
    memset(audioInMsg, 0, sizeof(audioInMsg));
        
    delay(50);
  
    // 20141103
    uint8_t pipe_num;
    unsigned long time_started_waiting = millis();
    boolean timeout = false;
    while( !timeout ) 
    {
      if (radio.available(&pipe_num))
      {
        //20141103
        Serial.print("pine# ");
        Serial.print(pipe_num);
        Serial.print(": ");

        // read from radio slave
        radio.read(radioInMsg, sizeof(radioInMsg));
   
        // print received string
        Serial.print("received from slave: ");
        Serial.println(radioInMsg);
   
        // write back to iPhone server from slave
        for (int i = 0; i < sizeof(radioInMsg); i++)
        {
          if (radioInMsg[i] == 0)
          break;
          modem.write(radioInMsg[i]);
        }

        modem.write('\r');
      }
      else if ( (millis() - time_started_waiting) > 3000 ) 
      {
        timeout = true;
        printf("This round is over.\r\n\r\n");
      }
    }
    
    stringDone = false;
  }
}
