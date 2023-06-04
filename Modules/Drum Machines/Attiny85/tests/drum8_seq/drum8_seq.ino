/*

*/

const uint8_t trigger_pin = 4;
const uint8_t shiftreg_data_pin = 2;
const uint8_t shiftreg_clock_pin = 1;
const uint8_t shiftreg_latch_pin = 0;

uint16_t tempo = 120;
const uint8_t trigger_time = 3;


/*
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
*/
const uint8_t drum_pattern[] =
{
  0b00000001,
  0b00000000,
  0b00000000,
  0b00000000,

  0b00000001,
  0b00000000,
  0b00000000,
  0b00000000,

  0b00000001,
  0b00000000,
  0b00000000,
  0b00000000,

  0b00000001,
  0b00000000,
  0b00000001,
  0b00000000,
};


void shift_out(uint8_t byte_out) {
  digitalWrite(shiftreg_latch_pin, LOW);
  for(uint8_t i=0; i<8; i++) {
    if(byte_out & (0b10000000>>i)) {  // Last bit first
      digitalWrite(shiftreg_data_pin, HIGH);
    }
    else {
      digitalWrite(shiftreg_data_pin, LOW);
    }

    digitalWrite(shiftreg_clock_pin, HIGH);
    digitalWrite(shiftreg_clock_pin, LOW);
  }
  digitalWrite(shiftreg_latch_pin, HIGH);
}

uint16_t bpm2ms(uint16_t bpm) {
  return 15000 / bpm;  // (60,000 / bpm) / 4
}



void setup() {
  pinMode(trigger_pin, OUTPUT);
  pinMode(shiftreg_data_pin, OUTPUT);
  pinMode(shiftreg_clock_pin, OUTPUT);
  pinMode(shiftreg_latch_pin, OUTPUT);
}

void loop() {
  for(uint8_t frame=0; frame<16; frame++) {
    uint16_t play_delay = 0;

    shift_out(frame);

    for(uint8_t s=0; s<8; s++) {
      if(drum_pattern[frame] & (1<<s)){
        // Set sample
        //TODO
        // Trigger
        digitalWrite(trigger_pin, HIGH);
        delay(trigger_time);
        digitalWrite(trigger_pin, LOW);
        delay(trigger_time);
        play_delay += trigger_time << 1;
      }
    }
    // Wait the rest of the period
    delay(bpm2ms(tempo) - play_delay);
  }
}
