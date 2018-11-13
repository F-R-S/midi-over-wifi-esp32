/*
Using hardwareSerial2 Sebagai Keluaran MIDI
Kandidat Penerima 1
*/

#define LED_INDIKATOR 2
#define UDP_PORT 1112
#define DEBUG_THRU_AND_UDP_COUNTER
#define PIN_UDP_TRIG_BTN 23
#include <Arduino.h>

#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";

#ifdef DEBUG_THRU_AND_UDP_COUNTER
unsigned int packetCounter=0;
unsigned int dataCounter=0;
#endif

AsyncUDP udp;

void setup()
{
  Serial2.begin(31250);
  Serial.begin(115200);

  pinMode(LED_INDIKATOR, OUTPUT);

#ifdef DEBUG_THRU_AND_UDP_COUNTER
  pinMode(PIN_UDP_TRIG_BTN, INPUT_PULLUP);
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  Serial.print("\n\n\n\nMenghubungkan ke AP: "); Serial.println(ssid); 
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    Serial.println("ESP Restart in 10 Sec"); 
    pinMode(LED_INDIKATOR,HIGH);
    delay(10000);
    ESP.restart();
  } 
  if (udp.listen(UDP_PORT)) {
    Serial.print("UDP Listening on IP: "); Serial.println(WiFi.localIP()); 
    Serial.print("Port: "); Serial.println(UDP_PORT);
    udp.onPacket([](AsyncUDPPacket packet) {
      digitalWrite(LED_INDIKATOR, HIGH);
      Serial2.write(packet.data(), packet.length());
#ifdef DEBUG_THRU_AND_UDP_COUNTER
      packetCounter ++;
      dataCounter += packet.length();
      Serial.print(packet.length()); 
      Serial.print("   ");
      for (int i = 0; i < packet.length(); i++) {
        Serial.print(packet.data()[i], HEX);
      }
      Serial.println();
#endif
      digitalWrite(LED_INDIKATOR, LOW);
    });
  }
}
void loop()
{
  delay(1000); //Beban task
  if (!WiFi.isConnected()){
    // Ora nyambung
    Serial.println("Not Connected, Restart in 10 Sec");
    pinMode(LED_INDIKATOR,HIGH);
    delay(1000);
    ESP.restart();
  }
#ifdef DEBUG_THRU_AND_UDP_COUNTER
  if (!digitalRead(PIN_UDP_TRIG_BTN)){
    Serial.println("\n\n===========================");
    Serial.print("Paket Diterima= ");
    Serial.println(packetCounter);
    Serial.print("Paket Sebesar= ");
    Serial.println(dataCounter);
    Serial.println("===========================");
    while(1)
    {
      if(digitalRead(PIN_UDP_TRIG_BTN)){
        break;
      } 
    }
  }
#endif
  //Ora ngopo-ngopo
}
