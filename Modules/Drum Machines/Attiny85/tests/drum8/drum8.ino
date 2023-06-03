// Copyright 2023 Sonic Athame. 
// 
// Author: Ella Jameson
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
// 
// 
// Schematic:
// 
//         ┌───U───┐
//        ─┤       ├─ Vcc
//  Pitch ─┤A     D├─ Trigger
// Sample ─┤A     D├───10KΩ─┬─ Out
//    Gnd ─┤      D├─ Mode  ╪ 0.1uF
//         └───────┘       Gnd
// 
// Pin Functions:
// 
// Pitch:
//   An analog input which varies the sample playback speed for the
//     entire chip.
//   At 5v, the chip will play back at full speed. Lowe voltages will
//     result in slower playback, and therefore a lower overall pitch.
// Mode:
//   A digital input which selects the triggering mode.
//   When the input is high (or NC), the chip will be in "Mono" mode.
//   When the input is low, the chip will be in "Poly" mode.
// Trigger:
//   A digital input that is used to trigger samples.
//   On each rising edge, the Sample and Mode pins are used to determine
//     which sample(s) to play.
// Sample:
//   An analog input which selects which drum samples to play.
//   In "Mono" mode, the 3 most significant bits are sampled and used to
//     select one of the 8 samples to play.
//   In "Poly" mode, the 8 most significant bits are sampled, and for
//     each bit that is set to 1, that sample is played. This means that
//     any combination of samples may be triggered at the same instant.
// Out:
//   A high-speed PWM "analog" audio output.
//   True analog output is realized via the RC lowpass filter.


#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "default/drums.h"  // Choose your sample kit here

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 


//---------- Ringbuf parameters -----------
uint8_t Ringbuffer[256];
uint8_t RingWrite=0;
uint8_t RingRead=0;
volatile uint8_t RingCount=0;
//-----------------------------------------

int trigger_pin = 2;
int mode_pin = 0;



// An indexable array of the sample pointers in drums.h
const uint8_t *drum[] =
{
  drum0,
  drum1,
  drum2,
  drum3,
  drum4,
  drum5,
  drum6,
  drum7
};
// sizeof() can't work with the above array of pointers, so here is a helper array
const uint16_t sizeof_drum[] =
{
  sizeof(drum0),
  sizeof(drum1),
  sizeof(drum2),
  sizeof(drum3),
  sizeof(drum4),
  sizeof(drum5),
  sizeof(drum6),
  sizeof(drum7)
};

uint16_t samplecnt[8];
uint16_t samplepnt[8];



void append_ring_buffer(uint8_t sample) {
  cli();
  Ringbuffer[RingWrite]=sample;
  RingWrite++;
  RingCount++;
  sei();
}

void update_ring_buffer() {
  int16_t total=0;
  for(int i=0; i<8; i++) {
    if (samplecnt[i]) {
      total+=(pgm_read_byte_near(drum[i] + samplepnt[i]++)-128);
      samplecnt[i]--;
    }
  }
  total>>=1;
  total+=128;  
  if (total>255) total=255;
  append_ring_buffer(total);
}

// Play a single sample (Indexed 0-7)
void play_sample(uint8_t sample_num) {
  samplepnt[sample_num]=0;
  samplecnt[sample_num]=sizeof_drum[sample_num];
}

// Play multiple samples
// Each binary digit of the input byte set to 1 will trigger the drum sample at it's index
// So "9" would be 00001001, and would play drum 0 and 3
void play_samples(uint8_t samples_byte)
{
  for(int i=0; i<8; i++) {
    if(samples_byte & (1<<i)) { // If drum i should be played, will not be 0
      play_sample(i);
    }
  }
}

// 0-1023, 1023 is full
void set_playback_speed(uint16_t playback_speed) {
  OCR0A = 49+((127- (playback_speed>>3) ));
}

void set_adc_mux(uint8_t mux) {
  ADMUX = mux;
}

uint16_t read_adc() {
  return ADCL+(ADCH<<8);
}



void setup() {
  OSCCAL=255;
  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1<<PCKE | 1<<PLLE;     

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                                // Timer interrupts OFF
  TCCR1 = 1<<PWM1A | 2<<COM1A0 | 1<<CS10;   // PWM A, clear on match, 1:1 prescale
  //GTCCR = 1<<PWM1B | 2<<COM1B0;             // PWM B, clear on match
  OCR1A = 128; //OCR1B = 128;               // 50% duty at start

  
  pinMode(1, OUTPUT);  // Enable PWM output pin
  pinMode(trigger_pin, INPUT_PULLUP);
  pinMode(mode_pin, INPUT_PULLUP);


  //Set up Timer/Counter0 for 20kHz interrupt to output samples.
  
  TCCR0A = 3<<WGM00;             // Fast PWM
  TCCR0B = 1<<WGM02 | 2<<CS00;   // 1/8 prescale
  TIMSK = 1<<OCIE0A;             // Enable compare match, disable overflow
  OCR0A = 49;                    // Divide by 400

  uint16_t dummy=analogRead(0);
}

void loop() {
  uint8_t trigger;
  uint8_t trigger_old;
  uint8_t sample_select;
  uint8_t MUX=2;

  set_adc_mux(MUX);  // Set ADC source
  sbi(ADCSRA, ADSC);  // Start ADC conversion

  while(1) {
    if (RingCount<32) {  // If there is space in ringbuffer
      update_ring_buffer();

      trigger=digitalRead(trigger_pin);
      if (trigger != trigger_old) {  // Detect trigger state change
        trigger_old = trigger;  // Update trigger state
        if (trigger_old) {  // If on a rising edge, trigger the sample
          if(digitalRead(mode_pin)) {  // If the mode select is high, play a single sample
            play_sample(sample_select>>5);  // 8 bits - 5 bits = 3 bits MSB (0-7)
          }
          else {  // If the mode pin is low, play multiple samples at once
            play_samples(sample_select);  // 8 bits for 8 possible drums
          }
        }
      }
      
      // Read ADC (cycle between sources on each loop, skip if ADC is not ready yet)
      if (!(ADCSRA & (1<<ADSC))) {  // If an ADC conversion is not already in progress (ready to read)
        if (MUX==3) set_playback_speed(read_adc());
        if (MUX==2) sample_select=(read_adc()>>2);  // 10 bits - 2 bits = 8 bits MSB (0-255)

        // Update ADC source
        MUX++;
        if (MUX==4) MUX=2;
        set_adc_mux(MUX);

        sbi(ADCSRA, ADSC);  // Start next ADC conversion
      }
    }
  }

}

ISR(TIMER0_COMPA_vect) {
  //-------------------  Ringbuffer handler -------------------------
    
    if (RingCount) {                            // If entry in FIFO
      OCR1A = Ringbuffer[(RingRead++)];         // Output 8-bit DAC
      RingCount--;
    }

  //-----------------------------------------------------------------
}
