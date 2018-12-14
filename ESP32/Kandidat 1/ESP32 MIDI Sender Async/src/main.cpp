/*
PERHATIAN
Program ini terkadang susah dimengerti. It's OK
Jadi Mohon Pengertiannya :)

++ Program MIDI melalui Protokol UDP Broadcast ++
~~~~~~~~~~~~~~~~~Bagian Pengirim~~~~~~~~~~~~~~~~~
Author	: Fahrizal Hari Utama
Board	: ESp32 Dev Board / ESP 32 DevKit
Multi core (RTOS) : yes
Framework	: Arduino ESP32 RTOS
Kandidat Pengirim : 1 build 332
https://github.com/k2biru
===============================================
Uncomment DEBUG_THRU_AND_UDP_COUNTER untuk mengaktifkan 
fitur DEBUG (menampilkan data terproses) 
*/

#include <Arduino.h>

#define UDP_PORT 1112           // port UDP
//#define DEBUG_THRU_AND_UDP_COUNTER
#define LED_INDIKATOR 2         // pin LED proses
#define PIN_CEK_PAKET 23        // pin untuk triger penampilan debug perhitungan paket

#include "WiFi.h"
#include "AsyncUDP.h"
#include "midi_type.h"

const char * ssid = "MIDI";
const char * password = "MIDIMIDI";
AsyncUDP udp;
#ifdef DEBUG_THRU_AND_UDP_COUNTER
unsigned int packetCounter=0;
unsigned int dataCounter=0;
#endif

void setup();
void loop();
void kirim();
MidiType getStatusType(byte status_);
int getStatus_dataCount (byte inStatus);
void readSerial_multy();

void setup() {
  Serial2.begin(31250);	// MIDI
  Serial.begin(115200);	// Ke PC
  pinMode(LED_INDIKATOR, OUTPUT);
#ifdef DEBUG_THRU_AND_UDP_COUNTER
  pinMode(PIN_CEK_PAKET, INPUT_PULLUP);
#endif

  delay(1000);
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
  Serial.print("\nTerkoneksi dengan IP: "); Serial.println(WiFi.localIP()); 
  delay(1000);
	// Mendaftarkan alamat UDP dan port ke class UDP
    if (udp.connect(IPAddress(255, 255, 255, 255), UDP_PORT)) {
    Serial.println("\nUDP connected to : 255.255.255.255");
  }
}

void loop() {
  if (Serial2.available()) {
    readSerial_multy();
  }
  // fitur ini berfungsi untuk melihat data yang sudah terproses
  // Diaktifkan dengan menghubungkan PIN_CEK_PAKET ke ground
#ifdef DEBUG_THRU_AND_UDP_COUNTER
  if (!digitalRead(PIN_CEK_PAKET)){
    Serial.println("\n\n===========================");
    Serial.print("Paket Terkirim = ");
    Serial.println(packetCounter);
    Serial.print("Paket Sebesar = ");
    Serial.println(dataCounter);
    Serial.println("===========================");
    delay(500);
    while(1)
    {
      if(digitalRead(PIN_CEK_PAKET)){
        break;
      } 
    }
  }
#endif
}

void kirim() {
  digitalWrite(LED_INDIKATOR, HIGH);	// LED Aktivitas On
  unsigned int ret;
#ifdef DEBUG_THRU_AND_UDP_COUNTER
  // bungkus buffer ke UDP 
  ret = udp.write(_midiMessage, panjangPesanTersimpan); 
  Serial.print(ret);
  Serial.print("   ");
    if (ret){
    dataCounter += panjangPesanTersimpan;
    packetCounter ++;
  }
    for (int i = 0; i < panjangPesanTersimpan; i++) {
    Serial.print(_midiMessage[i], HEX);
  }
  Serial.println();
#else
  // bungkus buffer ke UDP 
  udp.write(_midiMessage, panjangPesanTersimpan);
#endif
  digitalWrite(LED_INDIKATOR, LOW);		// LED Aktivitas off
}

// menfilter Status Kanal
MidiType getStatusType(byte status_) {
  if ((status_ < 0x80) ||
      (status_ == 0xf4) ||
      (status_ == 0xf5) ||
      (status_ == 0xf9) ||
      (status_ == 0xfD))
  {
    return InvalidType;
  }
  if (status_ < 0xf0)
  {
	// Mengubah Status kanal ke kanal 1
    return MidiType(status_ & 0xf0);
  }
  return MidiType(status_);
}

// Mencari tahu Status MIDI dan mengisi variabel dan flag
int getStatus_dataCount (byte inStatus) {
  switch (getStatusType(inStatus))
  {
    case Start:
    case Continue:
    case Stop:
    case Clock:
    case ActiveSensing:
    case SystemReset:
    case TuneRequest:
      _dataCount = 0;
      return true;
      break;
    case ProgramChange:
    case AfterTouchChannel:
    case TimeCodeQuarterFrame:
    case SongSelect:
      _dataCount = 1;
      return true;
      break;
    case NoteOn:
    case NoteOff:
    case ControlChange:
    case PitchBend:
    case AfterTouchPoly:
    case SongPosition:
      _dataCount = 2;
      return true;
      break;
    case EndOfSystemExclusive:
      _flagEndSysExclusive = true;
      _dataCount = 0;
      return true;
      break;
    case SystemExclusive:
      _dataCount = SYS_EX_MAX_SIZE + 3;
      _flagEndSysExclusive = false;
      return true;
      break;
    case InvalidType:
    default:
      return false;
      break;
  }
}

void readSerial_multy() {
  //tempat menampung byte Serial
  byte _buff;
  while (1) {
    if (Serial2.available()) {
      _buff = Serial2.read();
      //memisahkan byte status/data MIDI
      if (_buff & FLAG_BIT_STATUS) {
        //masuk status MIDI
        //mendapatkan jenis status byte, variable dan flag
        if (getStatus_dataCount(_buff)) {
          // Benar-benar Status
          if (_flagEndSysExclusive) {
            //Merupakan status EoX
            _midiMessage[panjangPesanTersimpan] = _buff;
            panjangPesanTersimpan++;
            _flagEndSysExclusive = false;
          } else {
			// Status biasa
            _midiMessage[panjangPesanTersimpan] = _buff;
            panjangPesanTersimpan++;
          }
        }
      } else {
        if (_dataCount) {
			// Masuk data MIDI
          _midiMessage[panjangPesanTersimpan] = _buff;
          panjangPesanTersimpan++;
          _dataCount--;
        }
      }

      if (!_dataCount) {
        jumlahPesan++; // Menghitung jumlah pesan MIDI
      }
      if (panjangPesanTersimpan > MIDI_BUFFER_MAX - 1) {
        // Buffer penuh -> Keluar while
        break;
      }
      if (jumlahPesan >= MIDI_MSG_MAX_UDP) {
        // Jumlah pesan melebihi batas -> Keluar While 
        break;
      }
    } else {
      // Tidak ada buffer di serial
      if (!_dataCount) {
        // Tidak ada byte data -> Keluar While
        break;
      }
    }
  }
  kirim();	// dikirim ke bagian UDP
  panjangPesanTersimpan = 0;	// Reset Variabel
  jumlahPesan = 0;				// Reset Variabel
}