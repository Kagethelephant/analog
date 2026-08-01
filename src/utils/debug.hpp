#pragma once

#include <glm/glm.hpp>
#include <string>

void print();

void print(const glm::mat3& mat);
void print(const glm::mat4& mat);

void print(const glm::vec2& vec);
void print(const glm::vec3& vec);
void print(const glm::vec4& vec);
void print(float num);
void print(int num);
void print(double num);
void print(std::string num);

std::string toString(glm::vec3 v);

void clearTerminal();
