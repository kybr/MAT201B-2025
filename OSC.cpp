#include "al/app/al_App.hpp"
using namespace al;
class MyApp : public App {
  ParameterColor background{"background"};
  //Parameter background{"background", 1.0f, 0.0f, 1.0f};
  void onInit() override {
    parameterServer().verbose(true);
    parameterServer().registerParameter(background);
  }

  // we don't want to add this message callback because it would
  // mean the parameter server does not get the messages
  // void onMessage(osc::Message& m) override {
  //   std::cout << "Received message: " << m.addressPattern() << std::endl;
  // }

  void onAnimate(double dt) override {
  }
  void onDraw(Graphics& g) override { g.clear(background); }
};
int main() { MyApp().start(); }