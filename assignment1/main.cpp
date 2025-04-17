#include "al/app/al_App.hpp"
#include "al/app/al_GUIDomain.hpp"
#include "al/graphics/al_Image.hpp"
#include "al/math/al_Random.hpp"
using namespace al;
#include <fstream>  // for slurp()
#include <string>   // for slurp()

Vec3f rvec() {
  return Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS());
}
RGB rcolor() { return RGB(rnd::uniform(), rnd::uniform(), rnd::uniform()); }

std::string slurp(std::string fileName);  // only a declaration
class MyApp : public App {
  Mesh grid, rgb, hsl, mine;
  Mesh mesh;
  ShaderProgram shader;
  Parameter pointSize{"pointSize", 0.004, 0.0005, 0.015};

  void onInit() override {
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto& gui = GUIdomain->newGUI();
    gui.add(pointSize);  // add parameter to GUI
  }

  void onCreate() override {
    auto image = Image("../colorful.png");
    if (image.width() == 0) {
      std::cout << "Image not found" << std::endl;
      exit(1);
    }

    mesh.primitive(Mesh::POINTS);

    for (int y = 0; y < image.height(); ++y) {
      for (int x = 0; x < image.width(); ++x) {
        auto pixel = image.at(x, y);
        mesh.vertex(float(x) / image.width(), float(y) / image.width(), 0);
        mesh.color(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0);
        mesh.texCoord(0.1, 0);

        // fill in rgb, mine, and hsl meshes
      }
    }

    nav().pos(0, 0, 5);

    if (!shader.compile(slurp("../point-vertex.glsl"),
                        slurp("../point-fragment.glsl"),
                        slurp("../point-geometry.glsl"))) {
      printf("Shader failed to compile\n");
      exit(1);
    }
  }

  double time = 0;

  void onAnimate(double dt) override {
    time += dt;

    if (time < 3) {
    }

    // clear and regenerate the mesh
    // OR
    // update mesh as a linear combination of other meshes

    // iterate over mesh, changing its vertices.
  }

  void onDraw(Graphics& g) override {
    g.clear(0.1);
    g.shader(shader);
    g.shader().uniform("pointSize", pointSize);
    g.blending(true);
    g.blendTrans();
    g.depthTesting(true);
    g.draw(mesh);
  }

  bool onKeyDown(const Keyboard& k) override {
    if (k.key() == 'q') {
      std::cout << "Exiting..." << std::endl;
      quit();
    }

    if (k.key() == ' ') {
      mesh.reset();  // deletes all vertices and colors
      for (int i = 0; i < 100; ++i) {
        mesh.vertex(rvec());
        mesh.color(rcolor());
        mesh.texCoord(0.1, 0);
      }
    }

    if (k.key() == '1') {
    }

    return true;
  }
};

int main() { MyApp().start(); }

std::string slurp(std::string fileName) {
  std::fstream file(fileName);
  std::string returnValue = "";
  while (file.good()) {
    std::string line;
    getline(file, line);
    returnValue += line + "\n";
  }
  return returnValue;
}