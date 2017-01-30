#include <SPI.h>
#include <WiFi.h>

IPAddress ip(192, 168, 1, 106);  // the fixed internal IP address for WiFi shield: 

char ssid[] = "JF";          //  your network SSID (name) 
char pass[] = "08310122";   // your network password

int status = WL_IDLE_STATUS;
IPAddress server(192, 168, 1, 109); 
//char server[] = "www.google.com";

// Initialize the client library
WiFiClient client;

void setup() {
  Serial.begin(9600);
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    // don't do anything else:
    while(true);
  } 
  else {
    Serial.println("Connected to wifi");
    WiFi.config(ip); // 設定WiFi shield的固定ip
    Serial.println("\nStarting connection...");
    // if you get a connection, report back via serial:
    if (client.connect(server, 80)) 
    {
      Serial.println("connected");
      // Make a HTTP request:
      client.println("POST /camera.php HTTP/1.1");
//      client.println("GET /search?q=arduino HTTP/1.0");
      client.println("Host: 192.168.1.106");
  client.println("Content-Type: multipart/form-data; boundary=AaB03x");
//  client.print("Content-Length: ");
//  client.println(12);
      client.println();
    }
  }
}

void loop() 
{
  while(client.available())
  {
    char c = client.read();
    Serial.print(c);
  }
}
