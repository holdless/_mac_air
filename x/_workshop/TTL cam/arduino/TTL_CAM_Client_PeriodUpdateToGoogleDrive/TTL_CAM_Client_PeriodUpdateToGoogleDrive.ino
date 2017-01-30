#include <SoftwareSerial.h>
#include <SPI.h>
#include <WiFi.h>


IPAddress ip(192, 168, 1, 106);  // the fixed internal IP address for WiFi shield: 
IPAddress srvIp(192, 168, 1, 109);    // Server(PC)'s IP address
int status = WL_IDLE_STATUS;
char ssid[] = "JF"; //  your network SSID (name)
char pass[] = "08310122";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
unsigned int port = 80; // HTTP

// Initialize the client library
WiFiClient client;

SoftwareSerial mySerial(5,6);          // Set Arduino pin 5 and 6 as softserial
 
// TTL Cam related parameters 
long int i=0x0000;
byte ZERO = 0x00;
byte incomingbyte;
uint8_t MH,ML;
byte imgSize = ZERO; //ZERO: 640x480, 0x11:320x240, 0x22:160x120
 
void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
      ; // wait for serial port to connect. Needed for Leonardo only
  }
 
  ////// WiFi Setting //////
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // check for WiFI firmware version
  String fv = WiFi.firmwareVersion();
  Serial.print("fireware version: ");
  Serial.println(fv);
  
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
  Serial.println("");
  
  WiFi.config(ip); // 設定WiFi shield的固定ip

  printWifiStatus();
  Serial.println("");
  
  
  
    // start HTML request
  Serial.println("Starting connection to server...");
  while(!client.connect(srvIp, port)); // wait for connection
  Serial.println(">>>>Connected !");
  Serial.println("");




  ///////////////////////////////////
  ////// init Soft-Serial Port //////
//  mySerial.begin(115200);
  mySerial.begin(38400); // set baud rate
  delay(100); 
 
  ////// init Camera //////
  SendResetCmd(); // reset camera
  delay(2000);
 
  SetBaudRateCmd(0x2A); // set camera baud rate
  delay(500);
 
  // flush mySerial port
  while(mySerial.available()>0) 
  {
    incomingbyte=mySerial.read();
  }
  
  SetImageSizeCmd2(imgSize); // set image size
  
  // read return value
  while(!mySerial.available());
  Serial.println();
  Serial.println("Set Image Size Done: ");
  while(mySerial.available()>0)
  {
    incomingbyte=mySerial.read();
    Serial.print(incomingbyte, HEX);
    Serial.print(" ");
  }
  Serial.println();
  delay(500);
 
  // Reset  
//  SendResetCmd();
//  delay(100);
       
  
//  mySerial.begin(38400);
//  delay(100);   
 
}
 
void loop()
{ 
//  SendResetCmd();
//  delay(2000); //Wait 2-3 second to send take picture command
    
  SendTakePhotoCmd();
  Serial.println();
  Serial.println("Start taking photo:");
  byte p[5];
  for (int i = 0; i < 5; i++)
  {
    while(!mySerial.available());
    p[i] = mySerial.read();
    if(p[i]<0x10)  Serial.print("0");      
    Serial.print(p[i], HEX);
    Serial.print(" ");
  }
//  delay(1000);
 
  /////// pre flush the getSize information on serial //////
  while(mySerial.available()>0)
  {
    incomingbyte=mySerial.read();
  }

  GetJPEGSize();
    
  Serial.println("");
  Serial.println("Get Size Info Done:");
  byte b[9];
  for (int i = 0; i < 9; i++)
  {
    while(!mySerial.available());
    b[i] = mySerial.read();
    if(b[i]<0x10)  Serial.print("0");      
    Serial.print(b[i], HEX);
    Serial.print(" ");
  }
    
  unsigned short jpegSize1 = b[7]*256;
  byte jpegSize2 = b[8] & 0x00FF;
  Serial.println("");
  Serial.print("jpeg size1: ");
  Serial.println(jpegSize1);
  Serial.print("jpeg size2: ");
  Serial.println(jpegSize2);

  unsigned short jpglen = jpegSize1 + jpegSize2;
  Serial.print("Storing ");
  Serial.print(jpglen, DEC);
  Serial.println(" byte image.");
  Serial.println("");
 
  ////// Prepare HTML request //////
  String start_request = "";
  String end_request = "";
  start_request = start_request + "\n" + "--AaB03x" + "\n" + "Content-Disposition: form-data; name=\"picture\"; filename=\"CAM.JPG\"" + "\n" + "Content-Type: image/jpeg" + "\n" + "Content-Transfer-Encoding: binary" + "\n" + "\n";  
  end_request = end_request + "\n" + "--AaB03x--" + "\n";

  unsigned short extra_length;
  extra_length = start_request.length() + end_request.length();

  Serial.print("Extra length:");  
  Serial.println(extra_length);
    
  unsigned short len = jpglen + extra_length;
  
  /////// pre flush the non-JPEG information on serial //////
  while(mySerial.available()>0)
  {
    incomingbyte=mySerial.read();
  }
  
  ///// prepare HTML request for file uploading
  // show the HTML request on serial monitor
  Serial.println("Full request:");
  Serial.println("POST /camera.php HTTP/1.1");
  Serial.println("Host: 192.168.1.106:80");
  Serial.println("Content-Type: multipart/form-data; boundary=AaB03x");
  Serial.print("Content-Length: ");
  Serial.println(len);
  Serial.print(start_request);
  Serial.print("binary data");
  Serial.print(end_request);
  Serial.println("");
  Serial.println("");
/*  
  // start HTML request
  Serial.println("Starting connection to server...");
  while(!client.connect(srvIp, port)); // wait for connection
  Serial.println(">>>>Connected !");
  Serial.println("");
*/    
  // real HTTP command here:
//  client.println("POST /upload.html HTTP/1.1");
  client.println("POST /camera.php HTTP/1.1");
  client.println("Host: 192.168.1.106:80");
  client.println("Content-Type: multipart/form-data; boundary=AaB03x");
  client.print("Content-Length: ");
  client.println(len);
  client.print(start_request);
          
  long int j=0,k=0,count=0;
  boolean EndFlag=false;
  byte a[32];
  i=0x0000; // for read data accumulator counter

  while(!EndFlag)
  {
    j=0;
    k=0;
    count=0;
    //mySerial.flush();
        
    // read data
    SendReadDataCmd();
    delay(20);
        
    while(!mySerial.available());
    while(mySerial.available()>0)
    {
      incomingbyte=mySerial.read();
      k++;
      delay(1); //250 for regular
        
      if((k>5)&&(j<32)&&(!EndFlag))
      {
        a[j]=incomingbyte;
        if((a[j-1]==0xFF)&&(a[j]==0xD9))     //tell if the picture is finished
        {
          EndFlag=1;
        }
        j++;
        count++;
      }
    }
 
    // show data on serial monitor
    for(j=0;j<count;j++)
    {
      if(a[j]<0x10)  Serial.print("0");
      Serial.print(a[j],HEX);           // observe the image through serial port
      Serial.print(" ");
    }
    
    //// write to Server        
    for(int ii=0; ii<count; ii++)
      client.write(a[ii]);
    Serial.println();
        
  }

  client.print(end_request);
  client.println();//////
  
  Serial.println();
  Serial.println("Transmission over");
  Serial.print("Finished writing data to Server");
  Serial.println();
  
  while (!client.available());
  Serial.println("-- Server Response: --");
  while (client.available()) 
  {
    // Read answer
    char c = client.read();
    Serial.print(c);
  }
  
  
  // 20141112
  StopTakePhotoCmd();
    
  Serial.println("");
  Serial.println("Stop taking photo:");
  byte d[5];
  for (int i = 0; i < 5; i++)
  {
    while(!mySerial.available());
    d[i] = mySerial.read();
    if(d[i]<0x10)  Serial.print("0");      
    Serial.print(d[i], HEX);
    Serial.print(" ");
  }

//  delayManySeconds(60);
  
  Serial.println();
  Serial.println("------------------------");
  Serial.println("------------------------");
}

///// private functions ////
void delayManySeconds(int sec)
{
  for (int jj = 0; jj < sec; jj++)
    delay(1000);
}

void SendResetCmd()
{
    mySerial.write(0x56);
    mySerial.write(ZERO);
    mySerial.write(0x26);
    mySerial.write(ZERO);
}
 
/*************************************/
/* Set ImageSize :
/* <1> 0x22 : 160*120
/* <2> 0x11 : 320*240
/* <3> 0x00 : 640*480
/************************************/
/*
void SetImageSizeCmd()
{
  mySerial.write(0x56);
  mySerial.write(ZERO);
  mySerial.write(0x31);
  mySerial.write(0x05);
  mySerial.write(0x04);
  mySerial.write(0x01);
  mySerial.write(ZERO);
  mySerial.write(0x19);
  mySerial.write(0x22);
}
*/
void SetImageSizeCmd2(byte imgSize)
{
  mySerial.write(0x56);
  mySerial.write(ZERO);
  mySerial.write(0x54);
  mySerial.write(0x01);
  mySerial.write(imgSize);
}
 
/*************************************/
/* Set BaudRate :
/* <1>¡¡0xAE  :   9600
/* <2>¡¡0x2A  :   38400
/* <3>¡¡0x1C  :   57600
/* <4>¡¡0x0D  :   115200
/* <5>¡¡0xAE  :   128000
/* <6>¡¡0x56  :   256000
/*************************************/
void SetBaudRateCmd(byte baudrate)
{
    mySerial.write(0x56);
    mySerial.write(ZERO);
    mySerial.write(0x24);
    mySerial.write(0x03);
    mySerial.write(0x01);
    mySerial.write(baudrate);
}
 
void SendTakePhotoCmd()
{
    mySerial.write(0x56);
    mySerial.write(ZERO);
    mySerial.write(0x36);
    mySerial.write(0x01);
    mySerial.write(ZERO); 
}
 
void SendReadDataCmd()
{
    MH=i/0x100;
    ML=i%0x100;
    mySerial.write(0x56);
    mySerial.write(ZERO);
    mySerial.write(0x32);
    mySerial.write(0x0c);
    mySerial.write(ZERO);
    mySerial.write(0x0a);
    mySerial.write(ZERO);
    mySerial.write(ZERO);
    mySerial.write(MH);
    mySerial.write(ML);
    mySerial.write(ZERO);
    mySerial.write(ZERO);
    mySerial.write(ZERO);
    mySerial.write(0x20);
    mySerial.write(ZERO);
    mySerial.write(0x0a);
    i+=0x20; // 一次32個4bytes
}
 
void StopTakePhotoCmd()
{
    mySerial.write(0x56);
    mySerial.write(ZERO);
    mySerial.write(0x36);
    mySerial.write(0x01);
    mySerial.write(0x03);
}

// hiroshi, 20140930
void GetJPEGSize()
{
    mySerial.write(0x56);
    mySerial.write(ZERO);
    mySerial.write(0x34);
    mySerial.write(0x01);
    mySerial.write(ZERO);
}

// ...wifi functions
void printWifiStatus() 
{
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

