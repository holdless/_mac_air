#include <JPEGCamera.h>

#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
 
SoftwareSerial mySerial(5,6);          // Set Arduino pin 4 and 5 as softserial
 
byte ZERO = 0x00;
byte incomingbyte;
long int j=0,k=0,count=0,i=0x0000;
uint8_t MH,ML;
boolean EndFlag=0;
File  myFile;

//Create an instance of the camera
JPEGCamera camera;
//Create a character array to store the cameras response to commands
char response[32];
//camCount is used to store the number of characters in the response string.
unsigned int camCount=0;
//Size will be set to the size of the jpeg image.
int size=0;
 
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
 
void setup()
{
    Serial.begin(38400);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for Leonardo only
    }
 
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
 
    mySerial.begin(115200);
    delay(100);
    
    //Reset the camera
    camCount=camera.reset(response);
    delay(2000);

    //Set baud rate
    camCount=camera.setBaudRate(response, 0x2a);
    delay(500);

    //Set image size
    camCount=camera.setImageSize(response, 0x22);
    delay(500);

    mySerial.begin(38400);
    delay(100);
}
 
void loop()
{
    byte a[32];
    int ii;
 
    //Reset the camera
    camCount=camera.reset(response);
    delay(2000);                            //Wait 2-3 second to send take picture command

    //Take a picture
    camCount=camera.takePicture(response);
    delay(1000);
   
    while(mySerial.available()>0)
    {
        incomingbyte=mySerial.read();
    }
 
    myFile = SD.open("pic.jpg", FILE_WRITE); //The file name should not be too long
 
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
 
        for(ii=0; ii<count; ii++)
        myFile.write(a[ii]);
        Serial.println();
    }
 
        //Get the size of the picture
    camCount = camera.getSize(response, &size);
    //Print the size
    Serial.print("Size: ");
    Serial.println(size);

    myFile.close();
    Serial.print("Finished writing data to file");
    while(1);
}
