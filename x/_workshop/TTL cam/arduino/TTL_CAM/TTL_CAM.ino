#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

SoftwareSerial mySerial(5,6);          
// Rx/Tx. Set Arduino pin 5(Rx, to red/Tx) and 6(Tx, to black/Rx) as softserial
 
// TTL Cam related parameters 
byte ZERO = 0x00;
byte incomingbyte;
long int j=0,k=0,count=0,i=0x0000;
uint8_t MH,ML;
boolean EndFlag=0;
File  myFile;
  
void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
      ; // wait for serial port to connect. Needed for Leonardo only
  }

  ////// init SD card //////
  Serial.print("Initializing SD card...");
    // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
    // Note that even if it's not used as the CS pin, the hardware SS pin
    // (10 on most Arduino boards, 53 on the Mega) must be left as an output
    // or the SD library functions will not work.
  pinMode(10, OUTPUT);
  if (!SD.begin(4))
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  Serial.println("please waiting ....");
  Serial.println("");
 
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
  
  SetImageSizeCmd2(0x22); // set image size
  
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
  delay(500);
 
  // Reset  
//  SendResetCmd();
//  delay(100);
       
  
//  mySerial.begin(38400);
//  delay(100);   
}
 
void loop()
{
  byte a[32];
  int ii;
 
//  SendResetCmd();
//  delay(2000); //Wait 2-3 second to send take picture command
//  SetImageSizeCmd();
//  delay(500);
    
  SendTakePhotoCmd(); // take photo
  delay(1000);

  /////// pre flush the getSize information on serial //////
  while(mySerial.available()>0)
  {
    incomingbyte=mySerial.read();
  }
 
  GetJPEGSize(); // get image size
    
  Serial.println("");
  Serial.println("get size info:");
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

  /////// pre flush the non-JPEG information on serial //////
  while(mySerial.available()>0)
  {
    incomingbyte=mySerial.read();
  }
 
  myFile = SD.open("Pi.jpg", FILE_WRITE); //The file name should not be too long
 
  while(!EndFlag)
  {
    j=0;
    k=0;
    count=0;
    //mySerial.flush();
        
    // read data
    SendReadDataCmd();
    delay(20);
        
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
 
    for(j=0;j<count;j++)
    {
      if(a[j]<0x10)  Serial.print("0");
      Serial.print(a[j],HEX);           // observe the image through serial port
      Serial.print(" ");
    }
 
    //// write to file
    for(ii=0; ii<count; ii++)
        myFile.write(a[ii]);
    Serial.println();        
  }
 
  myFile.close();
  Serial.print("Finished writing data to file");
        
  while(1);
}

///// private functions ////
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

