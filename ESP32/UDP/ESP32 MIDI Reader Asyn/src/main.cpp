#include <Arduino.h>

#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";

AsyncUDP udp;

void setup()
{
  Serial.begin(31250);
  //Serial.begin(115200);
  //Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  } if (udp.listen(1112)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {
      Serial.write(packet.data(), packet.length());
    });
  }
}

void loop()
{
  //Ora ngopo-ngopo
}
