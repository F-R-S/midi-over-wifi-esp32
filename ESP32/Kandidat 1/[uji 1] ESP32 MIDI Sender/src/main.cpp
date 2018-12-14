/*
PERHATIAN
Program ini terkadang susah dimengerti. It's OK
Jadi Mohon Pengertiannya :)

++ [UJI 1] Program Identifikasi pesan MIDI 	  ++
~~~~~~~~~~~~~~~~~##############~~~~~~~~~~~~~~~~~
Author	: Fahrizal Hari Utama
Board	: ESp32 Dev Board / ESP 32 DevKit
Multi core (RTOS) : yes
Framework	: Arduino ESP32 RTOS
Kandidat Uji 1 : 1 build 14
https://github.com/k2biru
===============================================
*/

#include <Arduino.h>
#include "midi_type.h"

#define LED_INDIKATOR 2         // pin LED proses
#define PIN_CEK_PAKET 23        // pin untuk triger penampilan debug perhitungan paket

unsigned int packetCounter=0;
unsigned int dataCounter=0;

void setup();
void loop();
void kirim();
void kosongkan();
MidiType getStatusType(byte status_);
int getStatus_dataCount (byte inStatus);
void readSerial_multy();

void setup() {
  Serial2.begin(31250);
  Serial.begin(115200);

  pinMode(LED_INDIKATOR, OUTPUT);
  pinMode(PIN_CEK_PAKET, INPUT_PULLUP);

  Serial.println("\n\nPengujian 1");
  delay(1000); 
  Serial.println("Siap");
  Serial.println("Panjang |    Data");
}
void loop() {
  if (Serial2.available()) {
    readSerial_multy();
  }
  if (!digitalRead(PIN_CEK_PAKET)){
    Serial.println("\n\n===========================");
    Serial.print("Paket Terkirim = ");
    Serial.println(packetCounter);
    Serial.print("Paket Sebesar= ");
    Serial.println(dataCounter);
    Serial.println("===========================");
    delay(100);
    while(1)
    {
      if(digitalRead(PIN_CEK_PAKET)){
        break;
      } 
    }
  }
}
void kirim() {
  digitalWrite(LED_INDIKATOR, HIGH);
  unsigned int ret;
  ret = panjangPesanTersimpan;
  Serial.print(ret);
  Serial.print("          ");
  for (int i = 0; i < panjangPesanTersimpan; i++) {

    Serial.print(_midiMessage[i], HEX); Serial.print(" ");
  }
  Serial.println();
  digitalWrite(LED_INDIKATOR, LOW);
}
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
    return MidiType(status_ & 0xf0);
  }
  return MidiType(status_);
}
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
  kirim();	// dikirim ke fungsi lain
  panjangPesanTersimpan = 0;	// Reset Variabel
  jumlahPesan = 0;			// Reset Variabel
}