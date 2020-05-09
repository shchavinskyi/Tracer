#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstdint>

struct ImageSize
{
    uint32_t width;
    uint32_t height;
};

struct Settings
{
    ImageSize imageSize;
    uint32_t samplesPerPixel;
    uint32_t maxBounces;
};

inline Settings DefaultSettings() { return Settings{{400, 400}, 250, 10}; }

#endif // SETTINGS_H
