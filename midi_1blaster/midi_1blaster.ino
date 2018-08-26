/* midi_1blaster
   by nicolas escarpentier

   v 0.0.1
*/

//  == LIBRARIES & INIT
# include <MIDIUSB.h>

// pins
const uint8_t LED_PIN  =  8;
const uint8_t NOTE_PIN =  4;
const uint8_t FREQ_PIN = 19;
const uint8_t RATE_PIN = 18;
const uint8_t MODE_PIN =  7;

// variables init
bool note_press = true;
bool mode_press = true;
bool superblast = false;

float rate = 500;   // rate of fire for blaster mode
uint8_t pitch = 60; // current pitch
uint8_t mode = 0;   // mode:  0 - normal
                    //        1 - (hold) blast
                    //        2 - super blast

// === SETUP
void setup() {
  Serial.begin(115200);
  Serial.println("midi_1blaster v_0.0.1");
  Serial.println("======="); Serial.println();

  // initialize pins
  pinMode(LED_PIN, OUTPUT);
}



// === LOOP
void loop() {
  // update values
  update_values();

  // read note button
  bool note_p = digitalRead(NOTE_PIN);

  switch (mode) {
      // 0 - normal mode
      case 0:
        if (!note_p && note_press){
          Serial.print("  > note on: ");
          Serial.println(pitch);
          noteOn(0, pitch, 110);
          MidiUSB.flush();
        }
        else if (note_p && !note_press){
          Serial.print("  > note off: ");
          Serial.println(pitch);
          noteOff(0, pitch, 0);
          MidiUSB.flush();
        }
        break;
      // 1 - blaster (hold) mode
      case 1:
        break;
      // 2 - super blaster mode
      case 2:
        break;
      // default - nothing
      default:
        break;
    }
  
  note_press = note_p;

}


// === FUNCTIONS
void update_values(){
  // read mode button
  bool mode_p = digitalRead(MODE_PIN);
  if (!mode_p && mode_press){ // next mode
    if (++mode>2){ mode = 0; }
    Serial.print("  > mode change: ");
    Serial.println(mode);
  }
  mode_press = mode_p;
  
  // read pitch pot
  uint16_t pitch_val = analogRead(FREQ_PIN);
  pitch = (uint8_t) map(pitch_val, 0, 1023, 0, 127);

  // read rate pot
  uint16_t rate_val = analogRead(RATE_PIN);
  uint16_t bpm = map(rate_val, 0, 1023, 30, 320);
  rate = 60000/bpm;
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
