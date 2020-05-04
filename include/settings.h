#ifndef SETTINGS_H
#define SETTINGS_H

#include <glm/vec2.hpp>

struct Settings
{
    glm::ivec2 imageSize;
    uint32_t samplesPerPixel;
    uint32_t maxBounces;
};

inline Settings DefaultSettings() { return Settings{{600, 300}, 20, 10}; }

#endif // SETTINGS_H