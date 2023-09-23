#include "LightPos.hpp"

LightPos::LightPos() {
    resetPosition();
}

void LightPos::resetPosition() {
    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;

    pos[0] = 3.0f;
    pos[1] = 10.0f;
    pos[2] = -4.0f;
}