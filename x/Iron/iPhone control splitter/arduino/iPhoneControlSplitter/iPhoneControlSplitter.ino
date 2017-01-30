#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <TimerOne.h>


// the fixed internal IP address for WiFi shield:
IPAddress ip(192, 168, 1, 109);    

int status = WL_IDLE_STATUS;
char ssid[] = "JF"; //  your network SSID (name)
char pass[] = "08310122";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 8888;//2390;      // local port to listen on

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet
char ReplyBuffer[] = "acknowledged"; // a string to send back

WiFiUDP Udp;
//WiFiUDP Udp2;

// 2014-9-12
// DO/AO
const byte resetPin = 6;
const byte motorON = 5; // DO output to control motor
const byte wifiStatusLED = 8; // DO, wifi connection status signal
// DI/AI
const byte moistSensor = A0; // analog input
const byte micSensor = A1; // analog input

//WiFi flag
boolean wifiConnected = false;

void setup() 
{
  // hiroshi, 2013-9-12. pin definition
  pinMode(motorON, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(wifiStatusLED, OUTPUT);

//  delay(200);
  
//  pinMode(micSensor, INPUT);
//  pinMode(moistSensor, INPUT);
  
  // Attach external interrupt to pin 2
//  attachInterrupt(0, wifiConnecting, RISING);

  // add TimerOne interrupt
  Timer1.initialize(10000); // set a timer of unit micro-second
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  ////////// WiFi connecting routine begin /////////////////
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // check for WiFI firmware version
  String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(3000);
  }
  Serial.println("Connected to wifi");
  
  WiFi.config(ip); // 設定WiFi shield的固定ip

  printWifiStatus();
  ////////// WiFi connecting routine end /////////////////

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:

  // begin UDP socket
  Udp.begin(localPort);
  
  wifiConnected = true;
}



int moistCnt = 0;
int _MoistThreshold = 1000;
int _MicOnThreshold = 300;
int moistValue = 0;
int micValue = 0;
byte micCnt = 0;
byte _waitCnt = 300;
byte micOn = 0;
int inputValue = 0;

void loop() 
{  
    // MIC part
  micValue = analogRead(micSensor); // read MIC output
  
  if (micValue > _MicOnThreshold)
  {
    micCnt = _waitCnt;
    micOn = 1;
    Serial.print("MIC: ");
    Serial.println(micValue);
  }
  else if (micCnt == 0)
  {
    micOn = 0;
  }

  if (micCnt > 0)
    micCnt--;

  // moistSensor
  if (moistCnt >= _MoistThreshold)
  {
    moistCnt = 0;
    // for moisture sensor value
    moistValue = analogRead(moistSensor);
//      Serial.print("Moist: ");
//      Serial.println(moistValue);

  }
  else
  {
    moistCnt++;
  }
  
  // UDP communication part
  // if there's data available, read a packet  
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
//    Serial.print("From ");
//    IPAddress remoteIp = Udp.remoteIP();
//    Serial.print(remoteIp);
//    Serial.print(", port ");
//    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    if (len > 0) packetBuffer[len] = 0;
  
//    Serial.println("Contents:");
//    Serial.println(packetBuffer);
    
    /// read from UDP
    // from char to int
    inputValue = atoi(packetBuffer);
//    Serial.print("input value: ");
//    Serial.println(inputValue);
    // write to motorON  
//    digitalWrite(motorON, (byte)inputValue);
        
    for(int i = 0; i < UDP_TX_PACKET_MAX_SIZE;i++)
    { 
      packetBuffer[i] = 0;
    }

    /// write to UDP
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());

    char myString[4];
    char myLedLogic[12] = ""; // as source string for strcat, the array dimension should be large enough
    strcat(myLedLogic, itoa(micOn, myString, 10));
    myString[0] = 0;
    strcat(myLedLogic, ",");
    // response to client
    Udp.write(strcat(myLedLogic, itoa(moistValue, myString, 10)));
    
//    Serial.println(myLedLogic);

    Udp.endPacket();  
  }
  else
  {
    Serial.print("No Receive:");
    Serial.println(packetSize);

  }
  
//  Serial.print("Motor Status: ");
//  Serial.println(inputValue);
}

// *** timere 1
byte count = 0;
unsigned int motorCnt = 0;
const int forceStopLimit = 2000;
boolean forceStop = false;

void timerIsr()
{
  // forceStop situation
  if (forceStop)
  {
    inputValue = 0;
    digitalWrite(resetPin, HIGH);  //call reset
  }
  else
  {
    if (inputValue == 1)
    {
      if (motorCnt > forceStopLimit)
      {
        forceStop = true;
      }
      motorCnt++;  
    }
    else
    {
      motorCnt = 0;
    }
  }
  
  // digital write
  digitalWrite(motorON, (byte)inputValue);

  
  // wifi status LED
  if (!wifiConnected)
  {
    if (count < 20)
    {
      digitalWrite(wifiStatusLED, HIGH);
    }
    else
    {
      digitalWrite(wifiStatusLED, LOW);
    } 
    
    if (count == 40)
      count = 0;
    else
      count++;            
  }
  else
  {
    digitalWrite( wifiStatusLED, HIGH );
  }  
}

/*
// *** external interrupt
void wifiConnecting() 
{
    digitalWrite(micStatusLED, digitalRead(micStatusLED) ^ 1);
    // send a reply, to the IP address and port that sent us the packet we received

}     
*/

// ...wifi functions
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
