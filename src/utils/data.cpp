#include "data.hpp"


glm::vec4 getColor(const Color color) {
   glm::vec4 floatColor; // (0-1)
   unsigned int hexValue = static_cast<unsigned int>(color);
   // Extract the R, G, B, and A bytes using bit shifting and masking
   floatColor[0] = ((hexValue >> 24) & 0xFF)/256.0f; // Extract the AA byte
   floatColor[1] = ((hexValue >> 16) & 0xFF)/256.0f; // Extract the RR byte
   floatColor[2] = ((hexValue >> 8) & 0xFF)/256.0f;  // Extract the GG byte
   floatColor[3] = ((hexValue) & 0xFF)/256.0f;       // Extract the BB byte
   return floatColor;
}

