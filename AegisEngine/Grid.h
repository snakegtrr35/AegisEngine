#pragma once

#ifndef CUBE_H
#define CUBE_H

#include "GameObject.h"
#include "Renderer.h"

class GRID : public GameObject {

	OBJECT_TYPE_INFO(GameObject, GRID)

private:
	aegis::uniquePtr<aegis::Buffer> VertexBuffer;	// 頂点バッファ

public:
	GRID();
	~GRID();

	void Init() override;
	void Draw() override;
	void Draw_DPP() {}
	void Update(float delta_time) override;
	void Uninit() override;
};

#endif // !CUBE_H