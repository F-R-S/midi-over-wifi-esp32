#include <Arduino.h>

#include "WiFi.h"
#include "AsyncUDP.h"
//#include <Ticker.h>

#define BIT_SEND 64
#define _ARRAY   64
//#define TIMER_MS 4


const char * ssid = "MIDI";
const char * password = "MIDIMIDI";


AsyncUDP udp;
unsigned int ns = 750;

bool timeToDo;

//Ticker tickerDo;

byte msg[_ARRAY];

void tickerDoInterupt();

void setup()
{
    Serial.begin (9600);
  Serial.println("I'M OK");
  Serial.print("ns : "); Serial.println(ns);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
  // tickerDo.attach_ms(TIMER_MS, tickerDoInterupt);
  msg[0] = 'A';
#ifdef BIT_MULTY
  for (int x = 1; x < _ARRAY; x++)
  {
    msg[x] = msg [0];
  }
#endif

  if (udp.connect(IPAddress(255, 255, 255, 255), 1112)) {
    Serial.println("\n UDP connected : 255, 255,255, 255");
  }
  delay(5000);
}

void loop()
{
  udp.write(msg, BIT_SEND);
  msg[0]++;
  delayMicroseconds(ns);
  if (msg[0] > 'Z') {
    msg[0] = 'A';
  }
}

void tickerDoInterupt() {
// timeToDo =true;
}