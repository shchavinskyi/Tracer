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
void GenerateBox(Scene& scene, float boxWidth);
void GenerateCornellBox(Scene& scene);

std::string SettingToString(const Settings& setting);

// Load mesh to scene
bool LoadMeshToScene(Scene& scene, float scale, const glm::vec3& offset, const uint32_t matId,
                     const std::string& filename);

#endif // UTILS_H
