
#include "Gamma/SamplePlayer.h"
#include "al/app/al_App.hpp"
struct MyApp : public al::App {
  gam::SamplePlayer<> player;
  void onInit() override {
    player.load("8.wav");
    player.rate(0.8);
    player.phase(1); // start at the end; don't play
  }
  void onSound(al::AudioIOData &io) override {
    while (io()) {
      float s = player();
      io.out(0) = s;
      io.out(1) = s;
    }
  }
  bool onKeyDown(const al::Keyboard &k) override {
    if (k.key() == ' ') {
      player.phase(0);
    }
    return true;
  }
};
int main() {
  MyApp app;
  app.configureAudio(44100, 512, 2, 2);
  app.start();
  return 0;
}
