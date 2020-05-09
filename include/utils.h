#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

struct Scene;

void GenerateRandomScene(Scene& scene, uint32_t sphereCount, uint32_t materialCount);

void CornellBox(Scene& scene);

#endif // UTILS_H
