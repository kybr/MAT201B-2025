#include "al/app/al_App.hpp"
#include "al/app/al_GUIDomain.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Random.hpp"
#include "al/math/al_Vec.hpp"

using namespace al;

#include <fstream>
#include <vector>
using namespace std;

Vec3f randomVec3f(float scale) {
  return Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * scale;
}

class Agent : public Nav {
  std::vector<Pose> path;
  public:
  virtual ~Agent() = default;
  Agent() {
    path.resize(5);
  }
  virtual void step(double dt) {
    //
    Nav::step(dt);
  }
};

struct AlloApp : App {
  Parameter timeStep{"/timeStep", "", 0.1, 0.01, 0.6};

  Light light;
  Material material;  // Necessary for specular highlights

  Mesh mesh;

  // size, color, species, sex, age, etc.
  std::vector<Agent> agent;

  void onInit() override {
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto &gui = GUIdomain->newGUI();
    gui.add(timeStep);
  }

  void onCreate() override {
    nav().pos(0, 0, 10);
    // addSphere(mesh);
    // mesh.translate(0, 0, -0.1);
    addCone(mesh);
    mesh.scale(1, 1, 1.3);
    mesh.scale(0.5);

    mesh.generateNormals();
    light.pos(0, 10, 10);

    for (int i = 0; i < 10; ++i) {
      Agent p;
      p.pos() = randomVec3f(5);
      p.quat()
          .set(rnd::uniformS(), rnd::uniformS(), rnd::uniformS(),
               rnd::uniformS())
          .normalize();
      // p.set(randomVec3f(5), randomVec3f(1));
      agent.push_back(p);
    }
  }

  Vec3f food;
  double time = 0;
  void onAnimate(double dt) override {
    if (time > 7) {
      time -= 7;

      food = randomVec3f(15);
      //
      //
    }
    time += dt;

    agent[0].moveF(5);
    agent[0].faceToward(agent[agent.size()-1].pos(), 0.02); // Nav faceToward()
    for (int i = 1; i < agent.size(); ++i) {
      agent[i].moveF(al::rnd::uniform(4, 6));
      agent[i].faceToward(agent[i-1].pos(), 0.02); // Nav faceToward()
      // agent[i].uf(); // stands for "unit forward", a length 1 vector that
      // is in the reference frame of the agent (Nav). 
    }

    // calculating average heading
    Vec3f heading;
    for (int i = 0; i < agent.size(); ++i) {
      heading += agent[i].uf();
    }
    heading.normalize();

    agent[0].uu(); // the "unit up" vector, points up in the reference frame of the agent

    ///////
    for (int i = 0; i < agent.size(); ++i) {
      agent[i].step(dt);
    }
  }

  bool onKeyDown(const Keyboard &k) override {
    if (k.key() == '1') {
    }
  }

  void onDraw(Graphics &g) override {
    g.clear(0.27);
    g.depthTesting(true);
    g.lighting(true);
    light.globalAmbient(RGB(0.1));
    light.ambient(RGB(0));
    light.diffuse(RGB(1, 1, 0.5));
    g.light(light);
    material.specular(light.diffuse() * 0.2);
    material.shininess(50);
    g.material(material);

    for (int i = 0; i < agent.size(); ++i) {
      g.pushMatrix();
      g.translate(agent[i].pos());
      g.rotate(agent[i].quat());
      g.draw(mesh);
      g.popMatrix();
    }

    Mesh m;
    addSphere(m, 0.1);
    m.generateNormals();
    g.translate(food);
    g.color(1, 0, 0);
    g.draw(m);
  }
};

int main() {
  AlloApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}