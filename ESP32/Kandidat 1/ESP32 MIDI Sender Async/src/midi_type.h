// Maksimal data SysEx
#define SYS_EX_MAX_SIZE           128
#define MIDI_MSG_MAX_UDP          255
// MIDI BUFFER
// 1 byte identitas 1, byte EOX (Sys Exclusive)
#define MIDI_BUFFER_MAX           SYS_EX_MAX_SIZE+1+(3*MIDI_MSG_MAX_UDP) 
#define FLAG_BIT_STATUS           0x80    ///0B10000000

enum MidiType
{
  InvalidType           = 0x00,    ///< KOSONG / ERROR
  NoteOff               = 0x80,    ///< Note Off
  NoteOn                = 0x90,    ///< Note On
  AfterTouchPoly        = 0xA0,    ///< Polyphonic AfterTouch
  ControlChange         = 0xB0,    ///< Control Change / Channel Mode
  ProgramChange         = 0xC0,    ///< Program Change
  AfterTouchChannel     = 0xD0,    ///< Channel (monophonic) AfterTouch
  PitchBend             = 0xE0,    ///< Pitch Bend
  SystemExclusive       = 0xF0,    ///< System Exclusive
  TimeCodeQuarterFrame  = 0xF1,    ///< System Common - MIDI Time Code Quarter Frame
  SongPosition          = 0xF2,    ///< System Common - Song Position Pointer
  SongSelect            = 0xF3,    ///< System Common - Song Select
  TuneRequest           = 0xF6,    ///< System Common - Tune Request
  EndOfSystemExclusive  = 0xF7,    ///< End Of System Exclusive
  Clock                 = 0xF8,    ///< System Real Time - Timing Clock
  Start                 = 0xFA,    ///< System Real Time - Start
  Continue              = 0xFB,    ///< System Real Time - Continue
  Stop                  = 0xFC,    ///< System Real Time - Stop
  ActiveSensing         = 0xFE,    ///< System Real Time - Active Sensing
  SystemReset           = 0xFF,    ///< System Real Time - System Reset
};

byte _dataCount;
byte _midiMessage[MIDI_BUFFER_MAX];
byte _flagEndSysExclusive = false;
byte jumlahPesan=0;
unsigned int panjangPesanTersimpan;
