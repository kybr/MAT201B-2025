#include "al/app/al_App.hpp"
using namespace al;
class MyApp : public App {
  ParameterColor background{"background"};
  void onInit() override {
    parameterServer().verbose(true);
    parameterServer().registerParameter(background);
  }

  void onMessage(osc::Message& m) override {
    std::cout << "Received message: " << m.addressPattern() << std::endl;
  }

  void onAnimate(double dt) override {
  }
  void onDraw(Graphics& g) override { g.clear(background); }
};

int main() { MyApp().start(); }
