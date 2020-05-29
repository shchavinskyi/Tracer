#ifndef UTILS_H
#define UTILS_H

#include "data.h"
#include "scene.h"

#include <glm/glm.hpp>

// Camera
Camera CameraFromView(const glm::vec3& lookFrom, const glm::vec3& lookAt, const glm::vec3& up,
                      float vfov, // top to bottom, in degrees
                      float aspect);

// Buffer helpers
RenderBuffer CreateImageBuffer(const ImageSize& size);
void ReleaseBuffer(RenderBuffer& buffer);

// Scene sample generators
void GenerateRandomScene(Scene& scene, uint32_t sphereCount, uint32_t materialCount);
void CornellBox(Scene& scene);

#endif // UTILS_H
