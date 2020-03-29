#ifndef _SAVE_
#define _SAVE_

#include <string>
#include <glm/fwd.hpp>

void saveImageBufferToFile(const glm::ivec2& size, glm::vec3* buffer, const std::string& filename);

#endif // _SAVE_