#ifndef SAVE_H
#define SAVE_H

#include <glm/fwd.hpp>
#include <string>

struct ImageSize;
struct RenderBuffer;

void SaveImageBufferToFile(const RenderBuffer& buffer, const ImageSize& size, const std::string& filename);

#endif // SAVE_H
