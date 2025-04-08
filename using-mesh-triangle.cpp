#include "al/app/al_App.hpp"
class MyApp : public al::App {
    al::Mesh m;

    void onCreate() override {
        // pull back a little so we can see things that are positioned at the origin
        nav().pos(0, 0, 3); // set the camera position

        m.primitive(al::Mesh::TRIANGLES);
        m.vertex(0, 0, 0);
        m.vertex(1, 0, 0);
        m.vertex(0, 1, 0);
    }

    void onDraw(al::Graphics& g) override {
        g.clear(0.27); // make a gray backgound
        g.color(1, 0, 0);
        g.draw(m);
    }
};
int main() {
    MyApp().start();
}