/*

*/

const uint8_t trigger_pin = 4;
const uint8_t sample_pin[] = {0, 1, 2};

uint16_t tempo = 120;
const uint8_t trigger_time = 10;


const uint8_t drum_pattern[] =
{
  0b00000001,
  0b00000000,
  0b01000001,
  0b00000000,

  0b00000010,
  0b00000000,
  0b01000000,
  0b00000010,

  0b00000000,
  0b00000010,
  0b01000001,
  0b00000001,

  0b00000010,
  0b00000000,
  0b01000000,
  0b00000010
};


void set_sample(uint8_t sample_num) {
  for(uint8_t i=0; i<3; i++) {
    digitalWrite(sample_pin[i], sample_num & (1<<i));
  }
}

uint16_t bpm2ms(uint16_t bpm) {
  return 15000 / bpm;  // (60,000 / bpm) / 4
}


void setup() {
  pinMode(trigger_pin, OUTPUT);
  for(uint8_t i=0; i<3; i++) {
    pinMode(sample_pin[i], OUTPUT);
  }
}

void loop() {
  for(uint8_t frame=0; frame<16; frame++) {
    uint16_t play_delay = 0;
    for(uint8_t s=0; s<8; s++) {
      if(drum_pattern[frame] & (1<<s)){
        // Set sample
        set_sample(s);
        // Trigger
        digitalWrite(trigger_pin, HIGH);
        delay(trigger_time);
        digitalWrite(trigger_pin, LOW);
        delay(trigger_time);
        play_delay += (trigger_time<<1);
      }
    }
    // Wait the rest of the period
    delay(bpm2ms(tempo)-play_delay);
  }
}
