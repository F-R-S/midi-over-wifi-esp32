/*
+++++ [UJI 2] Program Komunikasi UDP       ++++++
~~~~~~~~~~~~~~~~~Bagian Penerima~~~~~~~~~~~~~~~~~
Author	: Fahrizal Hari Utama
Board	: ESp32 Dev Board / ESP 32 DevKit
Multi core (RTOS) : no
Framework	: Arduino ESP32 RTOS
Kandidat Uji 2 Penerima  : 1 build 24
https://github.com/k2biru
===============================================
PENGUJIAN 2
Berisi program menguji koumikasi UDP Async WiFi
Pengujian ini mensimulasikan komunikasi UDP async dengan mengirimkan data sebesar 10 pesan MIDI setiap 4ms
*/

#define LED_INDIKATOR 2
#define UDP_PORT 1112
#define PIN_UDP_TRIG_BTN 23
#include <Arduino.h>

#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";
unsigned int packetCounter=0;
unsigned int dataCounter=0;
AsyncUDP udp;

void setup()
{
  Serial2.begin(31250);
  Serial.begin(115200);

  pinMode(LED_INDIKATOR, OUTPUT);
  pinMode(PIN_UDP_TRIG_BTN, INPUT_PULLUP);
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
	  pinMode(LED_INDIKATOR,HIGH);
      packetCounter ++;
      dataCounter += packet.length();
	  digitalWrite(LED_INDIKATOR, LOW);
    });
  }
}
void loop()
{
  if (!digitalRead(PIN_UDP_TRIG_BTN)){
    Serial.println("\n\n===========================");
    Serial.print("Paket Diterima = ");
    Serial.println(packetCounter);
    Serial.print("Paket Sebesar = ");
    Serial.println(dataCounter);
    Serial.println("===========================");
    while(1)
    {
      if(digitalRead(PIN_UDP_TRIG_BTN)){
        break;
      } 
    }
  }
  //Ora ngopo-ngopo
}
