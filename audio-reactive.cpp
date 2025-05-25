#include "Gamma/SamplePlayer.h"
#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"

using namespace al;
struct MyApp : public App {
  Mesh m;
  gam::SamplePlayer<float, gam::ipl::Linear, gam::phsInc::Loop> player;
  float level = 0.0f;

  void onCreate() override {
    player.load("8.wav");  // it is in the bin folder
    addRect(m, 1, 1, 0, 0);
    nav().pos(0, 0, 2);
  }

  void onDraw(Graphics& g) override {
    g.clear(0.27);
    g.scale(0.1, level * 5, 1);
    g.draw(m);
  }

  void onSound(AudioIOData& io) override {
    while (io()) {
      float s = player() * 0.8f;

	  // low-pass filter on the square of the value (always positive)
      level = 0.997f * level + 0.003f * s * s;

      io.out(0) = s;  // left channel
      io.out(1) = s;  // right channel
    }
  }
};

int main() {
  MyApp app;
  app.configureAudio(44100, 512, 2, 2);
  app.start();
}