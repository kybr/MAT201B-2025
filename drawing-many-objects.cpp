#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp" // addCone, addCube, addSphere
using namespace al;

class MyApp : public App {
    Mesh mesh;

    Light light;
    Material material;

    void onCreate() override {
        nav().pos(0, 0, 5);

        for (int i = 0; i < 10; ++i) {
            mesh.translate(0, 0, -2);
            addCone(mesh);
        }

        mesh.generateNormals();

        light.pos(0, 10, 0);
    }

    void onDraw(Graphics& g) override {
        g.clear(0.27);

        g.depthTesting(true);
        light.ambient(RGB(0));
        light.diffuse(RGB(1, 1, 0.5));
        g.lighting(true);
        g.light(light);
        material.specular(light.diffuse() * 0.2);
        material.shininess(50);
        g.material(material);

        g.draw(mesh);
    }
};

int main() {
    MyApp().start();
}