#include "al/app/al_App.hpp"
using namespace al;

class MyApp : public App {
    Mesh mesh;

    void onCreate() override {
        nav().pos(0, 0, 5);

        mesh.primitive(Mesh::TRIANGLE_FAN);
        mesh.vertex(0, 0, 0);
        for (int i = 0; i < 101; ++i) {
            float x = cos(i * 2 * M_PI / 100);
            float y = sin(i * 2 * M_PI / 100);
            mesh.vertex(x, y, 0);
        }
    }

    void onDraw(Graphics& g) override {
        g.clear(0.27);
        g.color(1, 0, 0);
        g.draw(mesh);
    }
};

int main() {
    MyApp().start();
}