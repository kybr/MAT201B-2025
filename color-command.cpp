#include "al/app/al_App.hpp"
#include "al/graphics/al_Font.hpp"
using namespace al;


std::vector<std::string> lineToWords(const std::string& line) {
    std::vector<std::string> words;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}


class MyApp : public App {
  Font font;
  Mesh mesh;

  std::unordered_map<std::string, RGB> color_word{
      {"green", RGB(0.0, 1.0, 0.0)},  {"red", RGB(1.0, 0.0, 0.0)},
      {"blue", RGB(0.0, 0.0, 1.0)},   {"yellow", RGB(1.0, 1.0, 0.0)},
      {"purple", RGB(0.5, 0.0, 0.5)}, {"orange", RGB(1.0, 0.5, 0.0)},
      {"pink", RGB(1.0, 0.75, 0.8)},  {"brown", RGB(0.6, 0.3, 0.2)},
      {"black", RGB(0.0, 0.0, 0.0)},  {"white", RGB(1.0, 1.0, 1.0)}};

  RGB background{0.0, 0.0, 0.0};

  void onCreate() override {
    nav().pos(0, 0, 10);
    nav().setHome();
    font.load("../ConsolaMono-Book.ttf", 28, 2048);
    font.alignCenter();
    font.write(mesh, "(listening...)", 0.3f);
  }

  void onMessage(osc::Message& m) override {
    //std::cout << "Received message: " << m.addressPattern() << std::endl;
    if (m.addressPattern() == "/whisper") {
      std::string text;
      m >> text;
      std::transform(text.begin(), text.end(), text.begin(), ::tolower);
      text.erase(std::remove_if(text.begin(), text.end(), ::ispunct), text.end());
      
      for (auto& word : lineToWords(text)) {
        if (color_word.find(word) != color_word.end()) {
          background = color_word[word];
        }
      }

      size_t foundIndex = text.find("alexa");
      if (foundIndex != std::string::npos) {
        // we heard alexa name
      }
      font.write(mesh, text.c_str(), 0.3f);
    }
  }

  void onAnimate(double dt) override {}

  void onDraw(Graphics& g) override {
    g.clear(background);
    g.blending(true);
    g.blendTrans();
    g.texture();
    font.tex.bind();
    g.draw(mesh);
    font.tex.unbind();
  }
};
int main() { MyApp().start(); }