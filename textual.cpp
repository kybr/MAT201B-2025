#include "al/app/al_App.hpp"
#include "al_ext/assets3d/al_Asset.hpp"

#include <algorithm>  // max
#include <cstdint>    // uint8_t
#include <vector>

using namespace al;
using namespace std;

struct MyApp : App {
  Scene *ascene{nullptr};
  vector<Mesh> meshes;

  void onCreate() {

    std::string fileName = "../Neon_letters.obj";
    ascene = Scene::import(fileName);
    if (!ascene) {
      printf("error reading %s\n", fileName.c_str());
      return;
    }

    // extract meshes from scene
    meshes.resize(ascene->meshes());
    for (int i = 0; i < ascene->meshes(); i += 1) {
      ascene->mesh(i, meshes[i]);
    }

    nav().pos(0, 0, 3);

    // recenter A
    Vec3f center;
    for (auto& v : meshes[0].vertices()) {
      center += v;
    }
    center /= meshes[0].vertices().size();
    meshes[0].translate(-center);
  }

  void onDraw(Graphics &g) {
    g.clear(0.1);
    gl::depthTesting(true);
    g.lighting(true);
    g.draw(meshes[0]);
  }
};

int main(int argc, char *const argv[]) {
  MyApp app;
  app.start();
  return 0;
}