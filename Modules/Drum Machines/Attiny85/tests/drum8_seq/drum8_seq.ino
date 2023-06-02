/*

*/

const int trigger_pin = 4;
const int sample_pin[] = {0, 1, 2};

int tempo = 120;
const int trigger_time = 3;


void set_sample(int sample_num) {
  for(int i = 0; i < 3; i++) {
    digitalWrite(sample_pin[i], sample_num & (1<<i));
  }
}

int bpm2ms(int bpm) {
  return 60000 / bpm;
}


void setup() {
  pinMode(trigger_pin, OUTPUT);
  for(int i = 0; i < 3; i++) {
    pinMode(sample_pin[i], OUTPUT);
  }
}

void loop() {
  for(int s = 0; s < 8; s++) {
    // Set sample
    set_sample(s);
    // Trigger
    digitalWrite(trigger_pin, HIGH);
    delay(trigger_time);
    digitalWrite(trigger_pin, LOW);
    // Wait the rest of the period
    delay(bpm2ms(tempo)-trigger_time);
  }
}
