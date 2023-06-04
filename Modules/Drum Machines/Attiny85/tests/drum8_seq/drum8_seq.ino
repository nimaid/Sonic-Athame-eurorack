/*

*/

#define PIN_TRIGGER 4
#define PIN_SR_D 2
#define PIN_SR_C 1
#define PIN_SR_L 0

#define trigger_time 3


uint16_t tempo = 120;



uint8_t drum_pattern[] =
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



void shift_out(uint8_t byte_out) {
  digitalWrite(PIN_SR_L, LOW);
  for(uint8_t i=0; i<8; i++) {
    if(byte_out & (0b10000000>>i)) {  // Last bit first
      digitalWrite(PIN_SR_D, HIGH);
    }
    else {
      digitalWrite(PIN_SR_D, LOW);
    }

    digitalWrite(PIN_SR_C, HIGH);
    digitalWrite(PIN_SR_C, LOW);
  }
  digitalWrite(PIN_SR_L, HIGH);
}

uint16_t bpm2ms(uint16_t bpm) {
  return 15000 / bpm;  // (60,000 / bpm) / 4
}



void setup() {
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_SR_D, OUTPUT);
  pinMode(PIN_SR_C, OUTPUT);
  pinMode(PIN_SR_L, OUTPUT);
}

void loop() {
  for(uint8_t frame=0; frame<16; frame++) {
    // Set samples to play
    shift_out(drum_pattern[frame]);
    // trigger
    digitalWrite(PIN_TRIGGER, HIGH);
    delay(trigger_time);
    digitalWrite(PIN_TRIGGER, LOW);
    delay(trigger_time);

    // Wait the rest of the period
    delay(bpm2ms(tempo) - (trigger_time<<1));
  }
}
