#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>

struct Camera
{
    glm::vec3 origin;
    glm::vec3 lowerLeft;
    glm::vec3 horizontal;
    glm::vec3 vertical;
};

Camera cameraFromView(const glm::vec3& lookFrom, const glm::vec3& lookAt, const glm::vec3& up,
                      float vfov, // top to bottom, in degrees
                      float aspect);

#endif // CAMERA_H