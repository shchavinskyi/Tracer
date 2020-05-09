#include "camera.h"

#include <glm/glm.hpp>

Camera CameraFromView(const glm::vec3& lookFrom, const glm::vec3& lookAt, const glm::vec3& up,
                      float vfov, // top to bottom, in degrees
                      float aspect)
{
    float radians = glm::radians(vfov);
    constexpr float half = 0.5f;
    float halfHeight = glm::tan(radians * half);
    float halfWidth = aspect * halfHeight;
    glm::vec3 w = glm::normalize(lookFrom - lookAt);
    glm::vec3 u = glm::normalize(glm::cross(up, w));
    glm::vec3 v = glm::cross(u, w);

    constexpr float two = 2.0f;
    return Camera{lookFrom, lookFrom - halfWidth * u - halfHeight * v - w, two * halfWidth * u, two * halfHeight * v};
}
