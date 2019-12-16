#include	"Bounding.h"

unique_ptr<ID3D11Buffer, Release> BOUNDING::pVertexBuffer_BOX;
unique_ptr<ID3D11Buffer, Release> BOUNDING::pIndexBuffer_BOX;
const char BOUNDING::IndexNum_Box = 2 * 12;