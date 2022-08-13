#include "Bounding.h"

IMPLEMENT_ABSTRACT_OBJECT_TYPE_INFO(COMPONENT, BOUNDING)

aegis::uniquePtr<aegis::Buffer> BOUNDING::IndexBufferBox;
const char BOUNDING::IndexNum_Box = 2 * 12;
aegis::COLOR BOUNDING::Default_Color = aegis::COLOR(1.0f, 0.f, 0.f, 1.0f);