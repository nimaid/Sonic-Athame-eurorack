# Sonic Athame™ Disciples
## Assemble a following!

The `Sonic Athame™ Disciples` semi-modular Eurorack module is so much more than an envelope follower. There are 5 seperate effects circuits which are normally configured to act as a versitile envelope follower with seperate attack/decay knobs and ripple removal. However, each submodule can be bypassed in order to use it elsewhere in your patch!

`Sonic Athame™ Disciples` provides the following submodules:
* Input Amplifier
  * Input Jack
    * HPF 0.02Hz
    * LPF 300kHz
  * Gain Knob
    * Gain from 1x (0dB) to 100x (40dB)
  * Clipping Indicator
  * Aux Output Jack
* Full Wave Rectifier (FWR)
  * Input Jack
    * Normalized path to amplifier out
  * Clipping Indicator
  * Aux Output Jack
* Slew Rate Limiter
  * Input Jack
    * Normalized path to FWR out
  * Fast/Slow Toggle
    * Fast is good for envelope following
    * Slow is good for portamento / CV slew
  * Rise Time Knob
    * This is set as low as possible for envelope following
  * Fall Time Knob
    * This is set around 5% - 15% for envelope following
  * TODO: Shape controls
  * Slew Level Indicator
  * Aux Output Jack
* Lowpass Filter (LPF)
  * Input Jack
    * Normalized path to slew out
  * Cutoff Frequency Knob
    * Range TBD
    * Possible High/Low switch?
  * LPF Level Indicator
  * Output Jack
    * This is the envelope output for the normalized routing path
* Comparator
  * Positive Input Jack
    * Normalized path to LPF out
  * CV Knob
    * 0V - 5V
    * Aux Output Jack
  * Negative Input Jack
    * Normalized path to CV knob out
    * Acts as gate threshold for the normalized routing path
  * Comparator Level Indicator
  * Output Jack
    * This is the envelope gate for the normalized routing path
    * 0V or 5V