#pragma once

#ifndef FORWARDLUS_H
#define FORWARDLUS_H

class FORWARDLUS {
private:
	unique_ptr<ID3D11DepthStencilView, Release>			m_pDepthStencilView;

	unique_ptr<ID3D11DepthStencilState, Release>		m_DepthStateEnable;

	unique_ptr <ID3D11VertexShader, Release>			m_VertexShader;

public:

	FORWARDLUS() {}
	~FORWARDLUS() { Uninit(); };

	bool Init();
	void Update();
	void Draw();
	void Uninit();
};

#endif // !FORWARDLUS_H