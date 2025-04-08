#include "al/app/al_App.hpp"
#include "al/math/al_Random.hpp" // al::rnd namespace

auto rs() { return al::rnd::uniformS(); }
auto r() { return al::rnd::uniform(); }
auto rvec() { return al::Vec3d(rs(), rs(), rs()); }
auto rcolor() { return al::Color(r(), r(), r()); }

class MyApp : public al::App {
    al::Mesh m;

    void onCreate() override {
        // pull back a little so we can see things that are positioned at the origin
        nav().pos(0, 0, 3); // set the camera position

        m.primitive(al::Mesh::TRIANGLES);

        for (int i = 0; i < 5; ++i) {
            m.vertex(rvec()); // adds a vertex to the mesh
            m.vertex(rvec());
            m.vertex(rvec());
            m.color(rcolor());
            m.color(rcolor());
            m.color(rcolor());
        }
    }

    void onAnimate(double dt) override {
        nav().faceToward(al::Vec3d(0, 0, 0)); // look at the origin
    }


    void onDraw(al::Graphics& g) override {
        g.clear(0.27); // make a gray backgound
        // g.color(1, 0, 0); // set global color and drawing style
        g.meshColor(); // use the color of each vertex from the mesh
        g.draw(m);
    }
};
int main() {
    MyApp().start();
}