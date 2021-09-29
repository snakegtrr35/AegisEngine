#include	"Bounding.h"

unique_ptr<ID3D11Buffer, Release> BOUNDING::pIndexBuffer_BOX;
const char BOUNDING::IndexNum_Box = 2 * 12;
Aegis::COLOR BOUNDING::Default_Color = Aegis::COLOR(1.0f, 0.f, 0.f, 1.0f);