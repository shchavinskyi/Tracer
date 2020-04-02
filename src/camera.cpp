#include "camera.h"

#include <glm/glm.hpp>

Camera cameraFromView(const glm::vec3& lookFrom, const glm::vec3& lookAt, const glm::vec3& up,
                      float vfov, // top to bottom, in degrees
                      float aspect)
{
    float radians = glm::radians(vfov);
    float halfHeight = glm::tan(radians / 2.0f);
    float halfWidth = aspect * halfHeight;
    glm::vec3 w = glm::normalize(lookFrom - lookAt);
    glm::vec3 u = glm::normalize(glm::cross(up, w));
    glm::vec3 v = glm::cross(w, u);

    Camera result;
    result.origin = lookFrom;
    result.lowerLeft = lookFrom - halfWidth * u - halfHeight * v - w;
    result.horizontal = 2.0f * halfWidth * u;
    result.vertical = 2.0f * halfHeight * v;
    return result;
}
