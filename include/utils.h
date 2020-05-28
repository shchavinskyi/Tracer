#ifndef UTILS_H
#define UTILS_H

#include "data.h"
#include "scene.h"

#include <cstdint>
#include <cstdlib>
#include <glm/glm.hpp>

// Camera
Camera CameraFromView(const glm::vec3& lookFrom, const glm::vec3& lookAt, const glm::vec3& up,
                      float vfov, // top to bottom, in degrees
                      float aspect);

// Buffer helpers
RenderBuffer CreateImageBuffer(const ImageSize& size);
void ReleaseBuffer(RenderBuffer& buffer);

// Sample scene generators
void GenerateRandomScene(Scene& scene, uint32_t sphereCount, uint32_t materialCount);
void CornellBox(Scene& scene);

// -- Inline functions --

inline RenderBuffer CreateImageBuffer(const ImageSize& size)
{
    const uint32_t pixelCount = size.width * size.height;
    return RenderBuffer{static_cast<Color*>(malloc(sizeof(Color) * pixelCount)), 0, pixelCount};
}

inline void ReleaseBuffer(RenderBuffer& buffer)
{
    free(buffer.buffer);
}

#endif // UTILS_H
