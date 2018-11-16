// Maksimal data SysEx
//#define SYS_EX_MAX_SIZE           128
#define SYS_EX_MAX_SIZE           128
#define MIDI_MSG_MAX_UDP          255
// MIDI BUFFER
// 1 byte identitas 1, byte EOX (Sys Exclusive)
#define MIDI_BUFFER_MAX           SYS_EX_MAX_SIZE+1+(3*MIDI_MSG_MAX_UDP) 
#define FLAG_BIT_STATUS           0x80    ///10000000

/*
  /// 0
  #define SYS_COM_TUNE_REQ          B11110110
  #define SYS_EOX                   B11110111
  // #define SYS_START_SONG            B11111010
  // #define SYS_CONTINUE_SONG         B11111011
  // #define SYS_STOP_SONG             B11111100
  // #define SYS_ACTIVE_SENSING        B11111110
  // #define SYS_RESET                 B11111111
  #define SYS_REALTIME              B11111
  /// 1
  #define ST_SYS_COM_SONG_SELECT    B11110011
  #define CH_VOICE_PROGRAM_CHANGE   B1100
  #define CH_VOICE_CH_PRESS         B1101
  /// 2
  #define SYS_COM_SONG_P_POINTER    B11110011
  #define CH_VOICE_NOTE_OFF         B1000
  #define CH_VOICE_NOTE_ON          B1001
  #define CH_VOICE_POLY_KEY_PRESS   B1010
  #define CH_VOICE_CONTROL_CHANGE   B1011
  #define CH_VOICE_PITCH_BEND_CHG   B1110
  /// #define CH_VOICE_SEL_CH_MODE      B1011
  /// >=3
  #define SYS_EXCLUSIVE             B11110000
*/


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
