#include "al/app/al_App.hpp"
class MyApp : public al::App {
    void onCreate() override {
        // pull back a little so we can see things that are positioned at the origin
        nav().pos(0, 0, 3); // set the camera position
    }
    void onDraw(al::Graphics& g) override {
        g.clear(0.27); // make a gray backgound

        g.color(1, 0, 0);
        al::Mesh m;
        m.primitive(al::Mesh::LINES);
        //m.primitive(al::Mesh::POINTS);
        m.vertex(0, 0, 0);
        m.vertex(1, 0, 0);
        g.draw(m);
    }
};
int main() {
    MyApp().start();
}