#ifndef SAVE_H
#define SAVE_H

#include <glm/fwd.hpp>
#include <string>

struct ImageSize;

void SaveImageBufferToFile(glm::vec3* buffer, const ImageSize& size, const std::string& filename);

#endif // SAVE_H
