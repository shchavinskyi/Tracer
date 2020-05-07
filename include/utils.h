#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

struct Scene;

void GenerateRandomScene(Scene& scene, size_t sphereCount, size_t materialCount);

void CornellBox(Scene& scene);

#endif // UTILS_H
