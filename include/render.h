#ifndef RENDER_H
#define RENDER_H

#include "data.h"

#include <glm/vec3.hpp>
#include <thread>

struct Scene;

void RenderScene(const Scene& scene, RenderBuffer renderBuffer);

void RenderSceneMT(const Scene& scene, RenderBuffer& renderBuffer,
                   uint32_t threadCount = std::thread::hardware_concurrency());

#endif // RENDER_H
