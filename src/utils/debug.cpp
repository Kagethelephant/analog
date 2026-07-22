#include "debug.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>
#include <string>

void print(){
      std::cout << "\n";
}


void print(const glm::mat3& mat){
   for (int row = 0; row < 3; row++){
      for (int col = 0; col < 3; col++){
         std::cout << std::fixed << std::setprecision(5) << std::showpos;
         std::cout << mat[col][row] << "  ";
      }
      std::cout << "\n";
   }
}

void print(const glm::mat4& mat){
   for (int row = 0; row < 4; row++){
      for (int col = 0; col < 4; col++){
         std::cout << std::fixed << std::setprecision(5) << std::showpos;
         std::cout << mat[col][row] << "  ";
      }
      std::cout << "\n";
   }
}


void print(const glm::vec2& vec){

   std::cout << std::fixed << std::setprecision(5) << std::showpos;
   std::cout << "( " << vec.x << ", " << vec.y <<  ")" << std::endl;
}
void print(const glm::vec3& vec){

   std::cout << std::fixed << std::setprecision(5) << std::showpos;
   std::cout << "( " << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
}

void print(const glm::vec4& vec){

   std::cout << std::fixed << std::setprecision(5) << std::showpos;
   std::cout << "( " << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")" << std::endl;
}

void print(float num){
   std::cout << num << std::endl;
}

void print(int num){
   std::cout << num << std::endl;
}
void print(double num){
   std::cout << num << std::endl;
}
void print(std::string text){
   std::cout << text << std::endl;
}



std::string toString(glm::vec3 v){
return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}
