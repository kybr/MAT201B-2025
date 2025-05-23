#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/graphics/al_Image.hpp"
#include "al/io/al_File.hpp"

using namespace std;
using namespace al;

struct MyApp : public App {
    Mesh skydome;
    Texture texture;
    void onCreate() {
        // make a spherical mesh with texture coordinates
        //
        addSphereWithTexcoords(skydome, 1.0, 160, true);

        // load an image from a file
        //
        auto file = File::currentPath() + "../map.jpg";
        auto image = Image(file);

        // create a texture and load it with the image data
        //
        texture.create2D(image.width(), image.height());
        texture.submit(image.array().data(), GL_RGBA, GL_UNSIGNED_BYTE);
        texture.filter(Texture::LINEAR);

    }
    void onAnimate(double dt) {
    }
    void onDraw(al::Graphics& g) {
        g.clear(1);

        g.depthTesting(true);

        texture.bind();
        g.texture(); // tell allolib to use the texture shader pipeline
        g.draw(skydome);
    }
    bool onKeyDown(Keyboard const& k) {
        return true;
    }
};

int main() {
    MyApp app;
    app.configureAudio(48000, 512, 2, 0);
    app.start();
}