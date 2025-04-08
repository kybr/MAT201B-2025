#include "al/app/al_App.hpp"
int main() {
    al::Vec3d v1;
    v1.x = 1;
    v1.println(); // dump a vec to the console
    // v1 "points".... right

    al::Vec3d v2(0, 1, 0);
    v2.println();
    // v2 "points".... up

    std::cout << "v2 = " << v2 << std::endl; // dump a vec to the console

    al::Vec3d v3 = cross(v1, v2);
    // v3 "points".... out of the screen
    v3.println();

    auto dot = v1.dot(v2);
    std::cout << "dot = " << dot << std::endl; // dot product

    auto v4 = v1 + v2; // add two vectors
    v4.println(); // v4 "points".... up and to the right

    std::cout << "v4 dot v1 = " << v4.dot(v1) << std::endl; // dot product

}