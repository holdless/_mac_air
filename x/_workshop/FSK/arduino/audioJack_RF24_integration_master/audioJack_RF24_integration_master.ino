#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// audio jack
#include <SoftModem.h> //ライブラリをインクルード
#include <ctype.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10
//#define JOYSTICK_X A0
//#define JOYSTICK_Y A1
//byte ledPin = 13;

// NOTE: the "LL" at the end of the constant is "LongLong" type
//const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL, 0xF0F0F0F0D3LL };

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
SoftModem modem; //SoftModemのインスタンスを作る

/*-----( Declare Variables )-----*/
//int joystick[2];  // 2 element array holding Joystick readings
char radioInMsg[20];
char audioInMsg[20];
byte k = 0;
boolean stringDone = false;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  radio.begin();
//  radio.openWritingPipe(pipe);
  modem.begin(); // setup()でbeginをコールする

//  pinMode(ledPin, OUTPUT);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.openReadingPipe(2,pipes[2]);
  radio.startListening();

}//--(end setup )---

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  char c[1] = {0};

  ////.. audio jack part
  // read from iPhone
  while(modem.available())
  { //iPhoneからデータ受信しているか確認
    c[0] = modem.read(); //1byteリード
/*    
    if(isprint(c[0]))
    {
      Serial.println(c[0]); //Arduinoに送信
    }
    else
    {
      Serial.print("("); //表示できない文字はHexで表示
      Serial.print(c[0], HEX);
      Serial.print(")");      
    }
*/    
    /////
//    if (c[0] == 'z')
//    {
//      digitalWrite(ledPin, HIGH);
//    }
//    else if (c[0] == 'x')
//    {
//      digitalWrite(ledPin, LOW);
//    }
    
  }
  
  while(Serial.available())
  { //PCからデータを受信しているか確認
    char c = Serial.read(); //1byteリード
    modem.write(c); //iPhoneに送信
    
    Serial.print(c);
  }

//// 2.4GHz radio part
//  joystick[0] = analogRead(JOYSTICK_X);
//  joystick[1] = analogRead(JOYSTICK_Y);
  
//  radio.write( joystick, sizeof(joystick) );

  if (c[0] != 0) // 判斷 c 是否為空集合
  {
    if (c[0] != '\r' && c[0] != '\n')
    {
      stringDone = false;
      audioInMsg[k] = c[0];
      k++;
    }
    else
    {
      if (!stringDone)
      {
        stringDone = true;
        k = 0;
        Serial.print("Command from iPhone: ");
        Serial.println(audioInMsg);
    
        // write to radio slave
        radio.stopListening();
        radio.write( audioInMsg, sizeof(audioInMsg) );
        radio.startListening();
        
        memset(audioInMsg, 0, sizeof(audioInMsg));
        
        delay(50);
      }
    }
  }
  // 20141103
  uint8_t pipe_num;
  Serial.print("pine 1: ");
  Serial.println(pipe_num);
  if (radio.available(&pipe_num))
  {
    //20141103
  Serial.print("pine 2: ");
  Serial.println(pipe_num);

    bool done = false;
    // read from radio slave
    while (!done)
    {
      done = radio.read(radioInMsg, sizeof(radioInMsg));
    }
   
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
}
