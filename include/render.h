#ifndef RENDER_H
#define RENDER_H

#include "objects.h"

#include <glm/vec3.hpp>

struct Scene;

void RenderScene(const Scene& scene, RenderBuffer renderBuffer);

void RenderSceneMT(const Scene& scene, RenderBuffer& renderBuffer, uint32_t threadCount);

#endif // RENDER_H
