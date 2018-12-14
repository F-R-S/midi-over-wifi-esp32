/*
+++++ [UJI 2] Program Komunikasi UDP       ++++++
~~~~~~~~~~~~~~~~~Bagian Pengirim~~~~~~~~~~~~~~~~~
Author	: Fahrizal Hari Utama
Board	: ESp32 Dev Board / ESP 32 DevKit
Multi core (RTOS) : yes
Framework	: Arduino ESP32 RTOS
Kandidat Uji 2 Pengirim  : 1 build 32
https://github.com/k2biru
===============================================
PENGUJIAN 2
Berisi program menguji koumikasi UDP Async WiFi
Pengujian ini mensimulasikan komunikasi UDP async dengan mengirimkan data sebesar 10 pesan MIDI setiap 4ms
*/
#include <Arduino.h>

#include "WiFi.h"
#include "AsyncUDP.h"
#include <Ticker.h>

#define BIT_SEND 64
#define _ARRAY   64
#define TIMER_MS 4
#define LOOP_UDP 100

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";

AsyncUDP udp;
bool timeToDo;
Ticker tickerDo;
byte msg[_ARRAY];
unsigned int packetCounter=0;
unsigned int dataCounter=0;
unsigned int ulang=1;

void tickerDoInterupt();

void setup()
{
  Serial.begin (115200); // ke PC
  Serial.println("\n\n\n Pengujian 2 | UDP Async Send data");
  Serial.println("I'M OK");
  Serial.print("ms : "); Serial.println(TIMER_MS);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
  tickerDo.attach_ms(TIMER_MS, tickerDoInterupt);
  msg[0] = 'A';
  for (int x = 1; x < _ARRAY; x++)
  {
    msg[x] = msg [0];
  }
  if (udp.connect(IPAddress(255, 255, 255, 255), 1112)) {
    Serial.println("\n UDP connected : 255, 255,255, 255");
  }
  delay(5000);
}

void loop()
{
  if (timeToDo){  
    unsigned int ret;
    ret = udp.write(msg, BIT_SEND);
    if (ret){
      dataCounter += ret;
      packetCounter ++;
    }
    msg[0]++;
    if (msg[0] > 'Z') {
      msg[0] = 'A';
    }
    for (int x = 1; x < _ARRAY; x++) {
      msg[x] = msg [0];
    }
    timeToDo= false;
    ulang++;
  }
  while (ulang>LOOP_UDP)
  {
    Serial.println("\n\n===========================");
    Serial.print("Paket Terkirim = ");
    Serial.println(packetCounter);
    Serial.print("Paket Sebesar= ");
    Serial.println(dataCounter);
    Serial.println("===========================");
    while(1){
      delay (1000);
    }
  }
}
void tickerDoInterupt() {
timeToDo =true;
}