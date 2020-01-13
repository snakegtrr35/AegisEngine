#include	"ForwardPlus.h"
#include	"Renderer.h"

bool FORWARDLUS::Init()
{
	return true;
}

void FORWARDLUS::Update()
{

}

void FORWARDLUS::Draw()
{

}

void FORWARDLUS::Uninit()
{

}

void FORWARDLUS::CreateBuffer(UINT bindFlags, const void* data, size_t count, UINT stride, CPUAccess cpuAccess, bool bUAV)
{
	// Assign the data to the system buffer.
	size_t numBytes = (UINT)count * stride;

	// Create a GPU buffer to store the data.
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = (UINT)numBytes;

	vector<BYTE> Data;

	if (data)
	{
		Data.assign((uint8_t*)data, (uint8_t*)data + numBytes);
	}
	else
	{
		Data.reserve(numBytes);
	}

	if (((int)cpuAccess & (int)CPUAccess::Read) != 0)
	{
		bufferDesc.Usage = D3D11_USAGE_STAGING;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	}
	else if (((int)cpuAccess & (int)CPUAccess::Write) != 0)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else
	{
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (bUAV)
		{
			bufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}
	}

	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = stride;

	{
		HRESULT hr = S_OK;
		ID3D11Buffer* Buffer = nullptr;

		{
			D3D11_SUBRESOURCE_DATA subResourceData;
			subResourceData.pSysMem = (void*)Data.data();
			subResourceData.SysMemPitch = 0;
			subResourceData.SysMemSlicePitch = 0;

			hr = CRenderer::GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &Buffer);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return;
			}
		}

		if ((bufferDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0)
		{
			ID3D11ShaderResourceView* srv = nullptr;

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvDesc.Buffer.FirstElement = 0;
			srvDesc.Buffer.NumElements = (UINT)count;

			hr = CRenderer::GetDevice()->CreateShaderResourceView(Buffer, &srvDesc, &srv);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return;
			}

			m_SRV.reset(srv);
		}

		if ((bufferDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) != 0)
		{
			ID3D11UnorderedAccessView* uav = nullptr;

			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Buffer.NumElements = (UINT)count;
			uavDesc.Buffer.Flags = 0;

			hr = CRenderer::GetDevice()->CreateUnorderedAccessView(Buffer, &uavDesc, &uav);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return;
			}
			m_UAV.reset(uav);
		}
	}
}