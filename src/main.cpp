#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* ssid = "XXXX"; // fill it your self lol
const char* password = "XXXX"; // fill it yourself

WiFiServer server(80);

String prepareHtmlPage()
{
  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation
  htmlPage = F("HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n"  // the connection will be closed after completion of the response
               "Refresh: 5\r\n"         // refresh the page automatically every 5 sec
               "\r\n"
               "<!DOCTYPE HTML>"
               "<html>"
               "<p> chal lodu </p>"
               "</html>"
                "\r\n");
  return htmlPage;
}


void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.printf("Attempting to connect to %s ", ssid); // ssid is a C style string, remember. We used printf not print!
  WiFi.begin(ssid, password); // connect to the router
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

void loop() 
{
  WiFiClient client = server.available();
  Serial.print(client);
  if(client)
  {
    Serial.println("Client has connected!\n");
    while(client.connected())
    {
        if(client.available())
        {
          String line = client.readStringUntil('\r');
          Serial.print(line);

          if(line.length() == 1 && line[0] == '\n')
            {
              client.println(prepareHtmlPage());
              break;
            }
        }
    }
  }
  while (client.available())
  {
    client.read();
  }

  client.stop();
  Serial.println("Client has been disconnected\n");
}
