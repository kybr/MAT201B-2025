#include "Gamma/Envelope.h"
#include "Gamma/Filter.h"
#include "Gamma/Oscillator.h"
#include "al/app/al_App.hpp"
using namespace gam;

using namespace al;

class MyApp : public App {
 public:
  Saw<> saw;        // Saw oscillator
  Biquad<> lpf;     // Resonant low-pass filter
  AD<> env;         // Envelope on amplitude and cutoff
  LFO<> modCutoff;  // Modulator on cutoff
  OnePole<> freq;   // Portamento filter
  Accum<> tmr;      // Sequencer timer
  int step;         // Sequencer step

  MyApp() {
    lpf.type(LOW_PASS);         // Set filter to low-pass response
    lpf.res(4);                 // Set resonance amount to emphasize filter
    env.attack(0.01);           // Set short (10 ms) attack
    env.decay(0.4);             // Set longer (400 ms) decay
    tmr.freq(120. / 60. * 4.);  // Set timer frequency to 120 BPM
    tmr.phaseMax();             // Ensures timer triggers on first sample
    modCutoff.period(30);       // Set period of cutoff modulation
    modCutoff.phase(0.5);       // Start half-way through cycle
    freq.lag(0.1);              // Lag time of portamento effect
    step = 0;
  }

  void onSound(AudioIOData& io) {
    while (io()) {
      if (tmr()) {
        // Our sequence of pitches
        float pitches[] = {0, 0, 12, 0, 0, 10, -5, 0};
        // Map pitch class to a frequency in Hz
        float f = 55 * pow(2, pitches[step] / 12.);
        // Increment step counter
        step = (step + 1) % 8;
        // Set new target frequence of portamento
        freq = f;
        // Restart envelope using a soft reset (to avoid clicks)
        env.resetSoft();
      }

      // Set saw frequency from portamento filter
      saw.freq(freq());

      // Get next envelope value
      float e = env();
      // Map envelope value to cutoff frequency
      lpf.freq(e * (modCutoff.paraU() * 6000 + 500) + 40);
      // Generate next saw sample
      float s = saw() * 0.3;
      // Filter saw sample
      s = lpf(s) * e;
      // Send sample to DAC
      io.out(0) = io.out(1) = s;
    }
  }
};

int main() { MyApp().start(); }
