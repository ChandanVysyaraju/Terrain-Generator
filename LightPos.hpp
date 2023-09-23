#pragma once

struct LightPos {
    LightPos();
    void resetPosition();
    
    float color[3];
    float pos[3];
};