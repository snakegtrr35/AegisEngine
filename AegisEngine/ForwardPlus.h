#pragma once

#ifndef FORWARDLUS_H
#define FORWARDLUS_H

enum class CPUAccess {
	None = 0,					// No CPU access to this texture is necessary.
	Read = (1 << 0),			// CPU reads permitted.
	Write = (1 << 1),			// CPU writes permitted.
	ReadWrite = Read | Write
};

struct Frustum {
	XMFLOAT4 plane[4];
};

class FORWARDLUS {
private:
	unique_ptr<ID3D11Buffer, Release>				m_ConstantBuffer;

	unique_ptr<ID3D11ShaderResourceView, Release>	m_SRV;
	unique_ptr<ID3D11UnorderedAccessView, Release>	m_UAV;

	void CreateBuffer(UINT bindFlags, const void* data, size_t count, UINT stride, CPUAccess cpuAccess = CPUAccess::None, bool bUAV = false);

public:

	FORWARDLUS() {}
	~FORWARDLUS() { Uninit(); };

	bool Init();
	void Update();
	void Draw();
	void Uninit();
};

#endif // !FORWARDLUS_H