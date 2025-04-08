#include "al/app/al_App.hpp"
int main() {

  //std::vector<std::vector<int>> ar;

  std::vector<float> ar; // std::vector is a dynamic array
  ar.push_back(1.0f); // add an element to the end of the array

  for (auto element : ar) {
    std::cout << element << std::endl; // print the element
  } 

}