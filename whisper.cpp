/*

Karl Yerkes
2025-05-15

For a while, I have wanted realtime voice commands in my apps and artworks.
Today, in 201B, I googled "on device c++ audio transcription" and found
this:

https://github.com/ggml-org/whisper.cpp

...which is a C++ port of OpenAI's Whisper:

https://openai.com/index/whisper

...which has been around for a few years.

I forked whisper.cpp and hacked to to broadcast OSC, then wrote this AlloLib
example to receive and display transcriptions. It is not low-latency, but it
works okay.

From a macOS terminal, start up whisper.cpp like this:

```bash
brew install liblo sdl2
git clone https://github.com/kybr/whisper.cpp
cd whisper.cpp
sh ./models/download-ggml-model.sh base.en
cmake -B build -DWHISPER_SDL2=ON
cmake --build build --config Release
./build/bin/whisper-stream -m ./models/ggml-base.en.bin --step 0 --length 5000
```

Then, run this example.

*/

#include "al/app/al_App.hpp"
#include "al/graphics/al_Font.hpp"
using namespace al;

class MyApp : public App {
  al::FontRenderer mFontRenderer;

  void onCreate() override {
    nav().pos(0, 0, 10);
    nav().setHome();
    mFontRenderer.load(al::Font::defaultFont().c_str(), 64, 2048);
    mFontRenderer.alignCenter();
    mFontRenderer.write("(listening...)", 0.3f);
  }

  void onMessage(osc::Message& m) override {
    std::cout << "Received message: " << m.addressPattern() << std::endl;
    if (m.addressPattern() == "/whisper") {
      std::string statement;
      m >> statement;
      std::cout << "Received text: " << statement << std::endl;
      mFontRenderer.write(statement.c_str(), 0.3f);
    }
  }

  void onAnimate(double dt) override {}

  void onDraw(Graphics& g) override {
    g.clear(0);
    g.blending(true);
    g.blendTrans();
    mFontRenderer.renderAt(g, al::Vec3d(0.0));
  }
};

int main() { 
  MyApp().start(); 
  return 0;
}