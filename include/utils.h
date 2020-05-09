#ifndef UTILS_H
#define UTILS_H

#include "objects.h"
#include "scene.h"

#include <cstdint>

void GenerateRandomScene(Scene& scene, uint32_t sphereCount, uint32_t materialCount);

void CornellBox(Scene& scene);

inline RenderBuffer CreateImageBuffer(const ImageSize& size)
{
    const uint32_t pixelCount = size.width * size.height;
    return RenderBuffer{static_cast<Color*>(malloc(sizeof(Color) * pixelCount)), 0, pixelCount};
}

inline void ReleaseBuffer(RenderBuffer& buffer) { free(buffer.buffer); }

#endif // UTILS_H
