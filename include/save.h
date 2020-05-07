#ifndef _SAVE_
#define _SAVE_

#include <glm/fwd.hpp>
#include <string>

void SaveImageBufferToFile(glm::vec3* buffer, const glm::ivec2& size, const std::string& filename);

#endif // _SAVE_