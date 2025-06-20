#include "al/app/al_DistributedApp.hpp"
#include "al/app/al_GUIDomain.hpp"
#include "al/math/al_Random.hpp"
#include "al_ext/statedistribution/al_CuttleboneDomain.hpp"
#include "al_ext/statedistribution/al_CuttleboneStateSimulationDomain.hpp"

using namespace al;

#include <fstream>
#include <vector>
using namespace std;

Vec3f randomVec3f(float scale) {
  return Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * scale;
}
string slurp(string fileName);  // forward declaration

#define NN (100)

struct CommonState {
  Vec3f position[NN];
  float pointSize;
  Pose nav;
  //
};

class Like {
 public:
  int i, j;
  float energy;
};

class Spring {
 public:
  int i, j;
  float length;
  float stiffness;
};

struct AlloApp : DistributedAppWithState<CommonState> {
  Parameter pointSize{"/pointSize", "", 2.0, 1.0, 10.0};
  Parameter timeStep{"/timeStep", "", 0.1, 0.01, 0.6};
  Parameter dragFactor{"/dragFactor", "", 0.1, 0.0, 0.9};
  //

  ShaderProgram pointShader;

  //  simulation state
  Mesh mesh;  // position *is inside the mesh* mesh.vertices() are the positions
  vector<Vec3f> velocity;
  vector<Vec3f> force;
  vector<float> mass;

  std::vector<Spring> spring_list;
  std::vector<Like> like_list;

  void onInit() override {
    auto cuttleboneDomain =
        CuttleboneStateSimulationDomain<CommonState>::enableCuttlebone(this);
    if (!cuttleboneDomain) {
      std::cerr << "ERROR: Could not start Cuttlebone. Quitting." << std::endl;
      quit();
    }

    // set up GUI
    if (isPrimary()) {
      auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
      auto &gui = GUIdomain->newGUI();
      gui.add(pointSize);   // add parameter to GUI
      gui.add(timeStep);    // add parameter to GUI
      gui.add(dragFactor);  // add parameter to GUI
    }
    //

    if (isPrimary()) {
      // initialize common state
    }
  }

  void onCreate() override {
    // compile shaders
    pointShader.compile(slurp("../point-vertex.glsl"),
                        slurp("../point-fragment.glsl"),
                        slurp("../point-geometry.glsl"));

    // set initial conditions of the simulation
    //

    // c++11 "lambda" function
    auto randomColor = []() { return HSV(rnd::uniform(), 1.0f, 1.0f); };

    mesh.primitive(Mesh::POINTS);
    // does 1000 work on your system? how many can you make before you get a low
    // frame rate? do you need to use <1000?
    for (int _ = 0; _ < NN; _++) {
      mesh.vertex(randomVec3f(5));
      mesh.color(randomColor());

      // float m = rnd::uniform(3.0, 0.5);
      float m = 3 + rnd::normal() / 2;
      if (m < 0.5) m = 0.5;
      mass.push_back(m);

      // using a simplified volume/size relationship
      mesh.texCoord(pow(m, 1.0f / 3), 0);  // s, t

      // separate state arrays
      velocity.push_back(randomVec3f(0.1));
      force.push_back(randomVec3f(1));
    }

    nav().pos(0, 0, 10);
  }

  bool freeze = false;
  void onAnimate(double dt) override {
    if (!isPrimary()) {
      // we are a renderer

      // rebuild the mesh from the state
      mesh.vertices().clear();
      for (int i = 0; i < NN; ++i) {
        mesh.vertex(state().position[i]);
      }

      // copy the simulator's camera/nav
      nav().set(state().nav);

      return;
    }

    state().pointSize = pointSize.get();
    state().nav = nav();

    if (freeze) return;

    for (int k = 0; k < spring_list.size(); ++k) {
      auto spring = spring_list[k];
      Vec3f a = mesh.vertices()[spring.i];
      Vec3f b = mesh.vertices()[spring.j];
      Vec3f displacement = b - a;
      float distance = displacement.mag();
      Vec3f f = displacement.normalize() * spring.stiffness *
                (distance - spring.length);
      force[spring.i] += f;
      force[spring.j] -= f;
    }

    for (int k = 0; k < like_list.size(); ++k) {
      auto like = like_list[k];
      Vec3f a = mesh.vertices()[like.i];
      Vec3f b = mesh.vertices()[like.j];
      Vec3f displacement = b - a;
      Vec3f f = displacement.normalize() * like.energy;
      force[like.i] += f;
      force[like.j] += f;
    }
    // Calculate forces

    for (int i = 0; i < mesh.vertices().size(); ++i) {
      for (int j = i + 1; j < mesh.vertices().size(); ++j) {
        // i and j are a pair
        // limit large forces... if the force is too large, ignore it
      }
    }

    //

    // XXX you put code here that calculates gravitational forces and sets
    // accelerations These are pair-wise. Each unique pairing of two particles
    // These are equal but opposite: A exerts a force on B while B exerts that
    // same amount of force on A (but in the opposite direction!) Use a nested
    // for loop to visit each pair once The time complexity is O(n*n)
    //
    // Vec3f has lots of operations you might use...
    // • +=
    // • -=
    // • +
    // • -
    // • .normalize() ~ Vec3f points in the direction as it did, but has
    // length 1 • .normalize(float scale) ~ same but length `scale` • .mag() ~
    // length of the Vec3f • .magSqr() ~ squared length of the Vec3f •
    // .dot(Vec3f f) • .cross(Vec3f f)

    // drag
    for (int i = 0; i < velocity.size(); i++) {
      force[i] += -velocity[i] * dragFactor;
    }

    // Integration
    //
    vector<Vec3f> &position(mesh.vertices());
    for (int i = 0; i < velocity.size(); i++) {
      // "semi-implicit" Euler integration
      velocity[i] += force[i] / mass[i] * timeStep;
      position[i] += velocity[i] * timeStep;
    }

    for (int i = 0; i < NN; ++i) {
        state().position[i] = position[i];
    }

    // clear all accelerations (IMPORTANT!!)
    for (auto &a : force) a.set(0);
  }

  bool onKeyDown(const Keyboard &k) override {
    if (k.key() == ' ') {
      freeze = !freeze;
    }

    if (k.key() == '1') {
      // introduce some "random" forces
      for (int i = 0; i < velocity.size(); i++) {
        // F = ma
        force[i] += randomVec3f(1);
      }
    }

    if (k.key() == '2') {
      // choose 2 particles at random
      int i = rnd::uniform(mesh.vertices().size());
      int j = rnd::uniform(mesh.vertices().size());
      while (i == j) {
        j = rnd::uniform(mesh.vertices().size());
      }
      // i and j are different particles....
      spring_list.push_back({i, j, 2.0, 2.0});
    }

    if (k.key() == '3') {
      // choose 2 particles at random
      int i = rnd::uniform(mesh.vertices().size());
      int j = rnd::uniform(mesh.vertices().size());
      while (i == j) {
        j = rnd::uniform(mesh.vertices().size());
      }
      // i and j are different particles....
      like_list.push_back({i, j, rnd::uniform()});
    }

    return true;
  }

  void onDraw(Graphics &g) override {
    g.clear(0.3);
    g.shader(pointShader);
    g.shader().uniform("pointSize", state().pointSize / 100);
    g.blending(true);
    g.blendTrans();
    g.depthTesting(true);
    g.draw(mesh);

    g.color(1.0, 1.0, 0.0);  // resets shader....
    Mesh springs(Mesh::LINES);
    for (int k = 0; k < spring_list.size(); ++k) {
      auto spring = spring_list[k];
      Vec3f a = mesh.vertices()[spring.i];
      Vec3f b = mesh.vertices()[spring.j];
      springs.vertex(a);
      springs.vertex(b);
    }
    g.draw(springs);
  }
};

int main() {

  rnd::global().seed(0);
  rnd::global_normal().seed(0);

  AlloApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}

string slurp(string fileName) {
  fstream file(fileName);
  string returnValue = "";
  while (file.good()) {
    string line;
    getline(file, line);
    returnValue += line + "\n";
  }
  return returnValue;
}
