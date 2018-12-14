/*
PERHATIAN
Program ini terkadang susah dimengerti. It's OK
Jadi Mohon Pengertiannya :)

++ Program MIDI melalui Protokol UDP Broadcast ++
~~~~~~~~~~~~~~~~~Bagian Penerima~~~~~~~~~~~~~~~~~
Author	: Fahrizal Hari Utama
Board	: ESp32 Dev Board / ESP 32 DevKit
Multi core (RTOS) : no
Framework	: Arduino ESP32 RTOS
Kandidat Penerima : 1 build 20
https://github.com/k2biru
===============================================
Uncomment DEBUG_THRU_AND_UDP_COUNTER untuk mengaktifkan 
fitur DEBUG (menampilkan data terproses) 
*/

#define LED_INDIKATOR 2
#define UDP_PORT 1112
//#define DEBUG_THRU_AND_UDP_COUNTER
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
  Serial2.begin(31250);	// MIDI
  Serial.begin(115200); // ke PC

  pinMode(LED_INDIKATOR, OUTPUT);
#ifdef DEBUG_THRU_AND_UDP_COUNTER
  pinMode(PIN_UDP_TRIG_BTN, INPUT_PULLUP);
#endif

  WiFi.mode(WIFI_STA);			// Mode Station
  WiFi.begin(ssid, password);	// Menghubungkan ke WiFi
  WiFi.setSleep(false);			// Supaya Fungsi Radio Tidak Sleep
  Serial.print("\n\n\n\nMenghubungkan ke AP: "); Serial.println(ssid); 
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    Serial.println("ESP Restart in 10 Sec"); 
    pinMode(LED_INDIKATOR,HIGH);
    delay(10000);
    ESP.restart();
  } 
  if (udp.listen(UDP_PORT)) {
	// Mendengarkan UDP pada port UDP_PORT
    Serial.print("UDP Listening on IP: "); Serial.println(WiFi.localIP()); 
    Serial.print("Port: "); Serial.println(UDP_PORT);
    udp.onPacket([](AsyncUDPPacket packet) {
	// Menjalankan ini saat paket tiba
    digitalWrite(LED_INDIKATOR, HIGH);	// LED Aktivitas On
	// tulis isi paket ke MIDI
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
    digitalWrite(LED_INDIKATOR, LOW);	// LED Aktivitas Off
    });
  }
}
void loop()
{
  delay (1000); // beban buatan supaya Watchdog gak "rewel".
  // baca watchdog timer dan RTOS untuk lebih lanjut
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
