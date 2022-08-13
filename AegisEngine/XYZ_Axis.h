#pragma once

#ifndef XYZ_AXIS_H
#define XYZ_AXIS_H

#include "GameObject.h"
#include "Renderer.h"

class AXIS : public GameObject {

	OBJECT_TYPE_INFO(GameObject, AXIS)

private:
	 aegis::uniquePtr<aegis::Buffer> VertexBuffer[3];	// 頂点バッファ
	 aegis::uniquePtr<aegis::Buffer> IndexBuffer;		// インデックスバッファ

public:
	AXIS();
	~AXIS();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override {}
	void Update(float delta_time) override;
	void Uninit() override;
};

#endif // !CUBE_H