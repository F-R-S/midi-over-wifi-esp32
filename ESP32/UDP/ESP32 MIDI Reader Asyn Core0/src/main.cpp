/*
Using hardwareSerial2 Sebagai Keluaran MIDI
Kandidat Penerima 1
*/


#define LED_INDIKATOR 2
#define UDP_PORT 1112
#define DEBUG_THRU 
#define DEBUG_UDP_PACKET
#define DEBUG_PACKET_COUNTER
#define PIN_CEK_PAKET 23

#include <Arduino.h>

#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";

#ifdef DEBUG_PACKET_COUNTER
unsigned int packetCounter=0;
unsigned int dataCounter=0;
#endif


AsyncUDP udp;




void setup()
{
  Serial2.begin(31250);
  Serial.begin(115200);

  pinMode(LED_INDIKATOR, OUTPUT);

  #ifdef DEBUG_PACKET_COUNTER
  pinMode(PIN_CEK_PAKET, INPUT_PULLUP);
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  } if (udp.listen(UDP_PORT)) {
    Serial.print("UDP Listening on IP: "); Serial.println(WiFi.localIP()); 
    Serial.print("Port: "); Serial.println(UDP_PORT);
    Serial.print ("Task Core: "); Serial.println(xPortGetCoreID());
    udp.onPacket([](AsyncUDPPacket packet) {
      digitalWrite(LED_INDIKATOR, HIGH);
      Serial2.write(packet.data(), packet.length());

      #ifdef DEBUG_PACKET_COUNTER
      packetCounter ++;
      dataCounter += packet.length();
      #endif

      #ifdef DEBUG_UDP_PACKET
      Serial.print(packet.length()); 
      Serial.print("   ");
      #endif

      #ifdef DEBUG_THRU
      for (int i = 0; i < packet.length(); i++) {
        Serial.print(packet.data()[i], HEX);
      }
      Serial.println();
      #endif

      //Serial.print ("Task Core: "); Serial.println(xPortGetCoreID());
      digitalWrite(LED_INDIKATOR, LOW);
    });
  }
}
void loop()
{
  #ifdef DEBUG_PACKET_COUNTER
  if (!digitalRead(PIN_CEK_PAKET)){
    Serial.println("\n\n===========================");
    Serial.print("Paket Diterima= ");
    Serial.println(packetCounter);
    Serial.print("Paket Sebesar= ");
    Serial.println(dataCounter);
    Serial.println("===========================");
    while(1)
    {
      if(digitalRead(PIN_CEK_PAKET)){
        break;
      } 
    }
  }
  #endif
  //delay(1000);
  //Ora ngopo-ngopo
}
