#include	"Bounding.h"

ComPtr<ID3D11Buffer> BOUNDING::pIndexBuffer_BOX;
const char BOUNDING::IndexNum_Box = 2 * 12;
aegis::COLOR BOUNDING::Default_Color = aegis::COLOR(1.0f, 0.f, 0.f, 1.0f);