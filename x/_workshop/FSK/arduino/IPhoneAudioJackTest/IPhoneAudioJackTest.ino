#include <SoftModem.h> //ライブラリをインクルード
#include <ctype.h>

SoftModem modem; //SoftModemのインスタンスを作る

byte ledPin = 13;

void setup()
{
  Serial.begin(9600); // not necessary 57600, 9600 is ok
  modem.begin(); // setup()でbeginをコールする
  
  ////
  pinMode(ledPin, OUTPUT);
}

int c(0);
void loop()
{
  while(modem.available()){ //iPhoneからデータ受信しているか確認
    c = modem.read(); //1byteリード
    if(isprint(c)){
      Serial.print((char)c); //PCに送信
    }
    else{
      Serial.print("("); //表示できない文字はHexで表示
      Serial.print(c, HEX);
      Serial.print(")");      
    }
    
    /////
    if (c == 'z')
    {
      digitalWrite(ledPin, HIGH);
    }
    else if (c == 'x')
    {
      digitalWrite(ledPin, LOW);
    }
    
  }
  
  
  while(Serial.available()){ //PCからデータを受信しているか確認
    char c = Serial.read(); //1byteリード
    modem.write(c); //iPhoneに送信
    
    Serial.print(c);
  }
}
