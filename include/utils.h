#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

struct Scene;

void GenerateRandomScene(Scene& scene, std::size_t sphereCount, std::size_t materialCount);

void CornellBox(Scene& scene);

#endif // UTILS_H
