/*
   Demopage to use the AdminPage on new Projects
*/
#include <SPI.h>
#include <Ethernet.h>
#include <AdminPage.h>
String swversion = "0.9b";

// export swversion to AdminPage
Page Page(swversion);

int bufferSize, contentbufferSize = 0;
char header[500];

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
// 00:ab:4e:fe:3b:ac
byte mac[] = { 0x00, 0xAB, 0x4E, 0xCE, 0x3B, 0xAC };
// IP Address for Webserver
IPAddress ip(192, 168, 88, 2);
// Subnet
IPAddress subn(255, 255, 255, 0);
// Gateway
IPAddress gw(192, 168, 88, 1);
// DNS
IPAddress dnserver(192, 168, 88, 1);

EthernetServer server(80);

void setup(){
  Serial.begin(115200);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, dnserver, gw, subn);
  server.begin();
  Serial.print(F("IP = "));
  Serial.println(Ethernet.localIP());
}

void loop(){
  EthernetClient client = server.available();
  if (client) {
      Serial.println("new client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          if (bufferSize < 500) header[bufferSize++] = c;

          if (c == '\n' && currentLineIsBlank) {

            // send Response Header
            if (strstr(header, "GET / HTTP/1.1")) {
              // send website to client
              Page.Admin(client);
            }
            if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
            }
            else if (c != '\r') {
              // you've gotten a character on the current line
              currentLineIsBlank = false;
          }
        }
      }
    }
  }
}

