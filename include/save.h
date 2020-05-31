#ifndef SAVE_H
#define SAVE_H

#include <glm/fwd.hpp>
#include <string>

struct ImageSize;
struct RenderBuffer;

void SaveImageBufferToFile(const RenderBuffer& buffer, const ImageSize& size, std::string_view filename);

#endif // SAVE_H
