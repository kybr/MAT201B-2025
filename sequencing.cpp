
#include "Gamma/Domain.h"
#include "Gamma/Envelope.h"
#include "Gamma/Oscillator.h"
#include "Gamma/SamplePlayer.h"
#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Random.hpp"
#include "al/scene/al_SynthSequencer.hpp"
#include "al/ui/al_ControlGUI.hpp"
#include "al/ui/al_Parameter.hpp"
#include "al/ui/al_PresetSequencer.hpp"

using namespace al;

class MyVoice : public SynthVoice {
  gam::SamplePlayer<> mSource;
  Mesh mesh; // The mesh now belongs to the voice
  float mSize{1.0}; // This are the internal parameters
  Vec3f position;

public:
  MyVoice() {
    addCone(mesh); // Prepare mesh to draw a cone
    mesh.primitive(Mesh::LINE_STRIP);
  }

  void onProcess(AudioIOData &io) override {
    while (io()) {
      io.out(0) += mSource();
    }
    if (mSource.done()) {
      free();
    }
  }

  double time = 0;

  //std::unique_ptr<std::function<Vec3f(double, Vec3f)>> trajectory;
  void update(double dt) override {
    time += dt;
    //position = trajectory(time, position);
  }

  void onProcess(Graphics &g) override {
    g.pushMatrix();
    g.translate(position);
    g.scale(mSize * mSource.pos() / mSource.max());
    g.draw(mesh); // Draw the mesh
    g.popMatrix();
  }

  void set(float x, float y, float size, const char *filename) {
    mSource.load(filename); // Load a sound file
    position.set(x, y, 0); // Set the position of the voice
    mSize = size;
  }

  void onTriggerOn() override {
   time = 0;
    mSource.reset();
  }
  void onTriggerOff() override { 
    mSource.finish(); 
  }

};

class MyApp : public App {
public:
  void onInit() override { gam::sampleRate(audioIO().framesPerSecond()); }

  void onCreate() override {
    nav().pos(Vec3d(0, 0, 8)); // Set the camera to view the scene
    sequencer().playSequence();
  }

  bool onKeyDown(const Keyboard &k) override {
    if (k.key() == ' ') {
      // If the space key is pressed, we will trigger the sequencer
      sequencer().playSequence();
    }
    return true;
  } 

  void onDraw(Graphics &g) override {
    g.clear();
    mSequencer.render(g);
  }

  void onSound(AudioIOData &io) override {
    mSequencer.render(io);
  }

  SynthSequencer &sequencer() { return mSequencer; }
  SynthSequencer mSequencer;
};

int main() {
  MyApp app;

  app.sequencer().add<MyVoice>(0, 1).set(0, 0, 0.5, "8.wav");
  app.sequencer().add<MyVoice>(0.5, 1).set(0, 0.5, 0.5, "8.wav");
  app.sequencer().add<MyVoice>(1, 2).set(0.5, 0.5, 0.7, "8.wav");
  app.sequencer().add<MyVoice>(1.1, 2).set(0.6, 0.5, 0.7, "8.wav");
  app.sequencer().add<MyVoice>(1.2, 2).set(0.3, 0.4, 0.7, "8.wav");
  app.sequencer().add<MyVoice>(1.3, 2).set(0.2, 0.3, 0.7, "8.wav");
  app.sequencer().add<MyVoice>(1.4, 2).set(0.1, 0.2, 0.7, "8.wav");
  app.sequencer().add<MyVoice>(1.5, 2).set(0.0, 0.2, 0.7, "8.wav");
  app.sequencer().add<MyVoice>(1.6, 2).set(-0.1, 0.1, 0.7, "8.wav");
  app.sequencer().add<MyVoice>(1.7, 2).set(-0.2, 0.0, 0.7, "8.wav");
  app.sequencer().add<MyVoice>(1.8, 2).set(-0.3, -0.1, 0.7, "8.wav");
  app.sequencer().add<MyVoice>(1.9, 2).set(-0.4, -0.2, 0.7, "8.wav");
  app.sequencer().add<MyVoice>(2.0, 2).set(-0.5, -0.3, 0.7, "8.wav");

  app.start();
  return 0;
}
