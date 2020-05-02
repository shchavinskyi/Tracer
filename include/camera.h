#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

struct Camera
{
    glm::vec3 origin;
    glm::vec3 lowerLeft;
    glm::vec3 horizontal;
    glm::vec3 vertical;
};

Camera CameraFromView(const glm::vec3& lookFrom, const glm::vec3& lookAt, const glm::vec3& up,
                      float vfov, // top to bottom, in degrees
                      float aspect);

inline Ray GetRay(const Camera& camera, float u, float v)
{
    return Ray{camera.origin,
               glm::normalize(camera.lowerLeft + u * camera.horizontal + v * camera.vertical - camera.origin)};
}

#endif // CAMERA_H