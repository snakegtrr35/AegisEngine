#include "RenderDX11.h"

#include <io.h>

#include "manager.h"
#include "ShadowMap.h"

#include "include/engine/core/AllocatorWrapper.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include "Sprite.h"

extern ID3D11Debug* gpD3dDebug;

namespace
{
	std::unique_ptr<SPRITE> sprite;

//	inline void SetName(ID3D11Device* resource, aegis::string debugName)
//	{
//#if defined(_DEBUG)
//		auto nameSet = resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.size(), debugName.c_str());
//		assert(SUCCEEDED(nameSet), "Failed to set debug name");
//#endif
//	}
//
//	inline void SetName(ID3D11DeviceChild* resource, aegis::string debugName)
//	{
//#if defined(_DEBUG)
//		auto nameSet = resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.size(), debugName.c_str());
//		assert(SUCCEEDED(nameSet), "Failed to set debug name");
//#endif
//	}
//
//	inline void SetDebugName(IDXGIObject* resource, aegis::string debugName)
//	{
//#if defined(_DEBUG)
//		auto nameSet = resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.size(), debugName.c_str());
//		assert(SUCCEEDED(nameSet), "Failed to set debug name");
//#endif
//	}
//
//	inline void SetDebugName(IDXGISwapChain1* resource, aegis::string debugName)
//	{
//#if defined(_DEBUG)
//		auto nameSet = resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.size(), debugName.c_str());
//		assert(SUCCEEDED(nameSet), "Failed to set debug name");
//#endif
//	}

	template<typename T>
	inline void SetName(T* resource, aegis::string debugName)
	{
#if defined(_DEBUG)
		auto nameSet = resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.size(), debugName.c_str());
		assert(SUCCEEDED(nameSet), "Failed to set debug name");
#endif
	}
}

namespace aegis
{
	namespace Details
	{
		D3D11_CLEAR_FLAG Convert2ClearFlag(const ClearFlag flag)
		{
			switch (flag)
			{
				case ClearFlag::Depth:
					return D3D11_CLEAR_DEPTH;
				case ClearFlag::Stencil:
					return D3D11_CLEAR_STENCIL;
			}
		}

		D3D11_PRIMITIVE_TOPOLOGY Convert2Topology(const PrimitiveTopology primitiveTopology)
		{
			switch (primitiveTopology)
			{
				case PrimitiveTopology::PointList:
					return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
				case PrimitiveTopology::LineList:
					return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				case PrimitiveTopology::LineStrip:
					return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
				case PrimitiveTopology::TriangleList:
					return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				case PrimitiveTopology::TriangleStrip:
					return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			}
		}

		D3D11_BLEND Convert2Blend(const Blend blend)
		{
			switch (blend)
			{
				case Blend::BlendZero:
					return D3D11_BLEND_ZERO;
				case Blend::BlendOne:
					return D3D11_BLEND_ONE;
				case Blend::BlendSrcColor:
					return D3D11_BLEND_SRC_COLOR;
				case Blend::BlendInvSrcColor:
					return D3D11_BLEND_INV_SRC_COLOR;
				case Blend::BlendSrcAlpha:
					return D3D11_BLEND_SRC_ALPHA;
				case Blend::BlendInvSrcAlpha:
					return D3D11_BLEND_INV_SRC_ALPHA;
				case Blend::BlendDestAlpha:
					return D3D11_BLEND_DEST_ALPHA;
				case Blend::BlendInvDestAlpha:
					return D3D11_BLEND_INV_DEST_ALPHA;
				case Blend::BlendDestColor:
					return D3D11_BLEND_DEST_COLOR;
				case Blend::BlendInvDestColor:
					return D3D11_BLEND_INV_DEST_COLOR;
				case Blend::BlendSrcAlphaSAT:
					return D3D11_BLEND_SRC_ALPHA_SAT;
				case Blend::BlendBlendFactor:
					return D3D11_BLEND_BLEND_FACTOR;
				case Blend::BlendInvBlendFactor:
					return D3D11_BLEND_INV_BLEND_FACTOR;
				case Blend::BlendSrc1Color:
					return D3D11_BLEND_SRC1_COLOR;
				case Blend::BlendInvSrc1Color:
					return D3D11_BLEND_INV_SRC1_COLOR;
				case Blend::BlendSrc1Alpha:
					return D3D11_BLEND_SRC1_ALPHA;
				case Blend::BlendInvSrc1Alpha:
					return D3D11_BLEND_INV_SRC1_ALPHA;
			}
		}

		D3D11_BLEND_OP Convert2BlendOp(const BlendOp blendOp)
		{
			switch (blendOp)
			{
				case BlendOp::BlendOpAdd:
					return D3D11_BLEND_OP_ADD;
				case BlendOp::BlendOpSubtract:
					return D3D11_BLEND_OP_SUBTRACT;
				case BlendOp::BlendOpRevSubtract:
					return D3D11_BLEND_OP_REV_SUBTRACT;
				case BlendOp::BlendOpMin:
					return D3D11_BLEND_OP_MIN;
				case BlendOp::BlendOpMax:
					return D3D11_BLEND_OP_MAX;
			}
		}

		D3D11_BLEND_DESC Convert2BlendDesc(const BlendDesc& blendState)
		{
			D3D11_BLEND_DESC desc;
			desc.AlphaToCoverageEnable = blendState.AlphaToCoverageEnable;
			desc.IndependentBlendEnable = blendState.IndependentBlendEnable;
			for (int32 i = 0; i < 8; i++)
			{
				desc.RenderTarget[i].BlendEnable = blendState.RenderTarget[i].BlendEnable;
				desc.RenderTarget[i].SrcBlend = Details::Convert2Blend(blendState.RenderTarget[i].SrcBlend);
				desc.RenderTarget[i].DestBlend = Details::Convert2Blend(blendState.RenderTarget[i].DestBlend);
				desc.RenderTarget[i].BlendOp = Details::Convert2BlendOp(blendState.RenderTarget[i].BlendOp);
				desc.RenderTarget[i].DestBlendAlpha = Details::Convert2Blend(blendState.RenderTarget[i].DestBlendAlpha);
				desc.RenderTarget[i].BlendOpAlpha = Details::Convert2BlendOp(blendState.RenderTarget[i].BlendOpAlpha);
				desc.RenderTarget[i].RenderTargetWriteMask = blendState.RenderTarget[i].RenderTargetWriteMask;
			}

			return desc;
		}

		uint32 Convert2BindFlag(const BindFlag bindFlag)
		{
			uint32 flag = 0;

			for (auto item : aegis::BindFlag())
			{
				if ((uint32)item & (uint32)bindFlag)
				{
					switch (item)
					{
						case aegis::BindFlag::Vertexbuffer:
							flag |= D3D11_BIND_VERTEX_BUFFER;
							break;
						case aegis::BindFlag::Indexbuffer:
							flag |= D3D11_BIND_INDEX_BUFFER;
							break;
						case aegis::BindFlag::Constantbuffer:
							flag |= D3D11_BIND_CONSTANT_BUFFER;
							break;
						case aegis::BindFlag::ShaderResource:
							flag |= D3D11_BIND_SHADER_RESOURCE;
							break;
						case aegis::BindFlag::StreamOutput:
							flag |= D3D11_BIND_STREAM_OUTPUT;
							break;
						case aegis::BindFlag::RenderTarget:
							flag |= D3D11_BIND_RENDER_TARGET;
							break;
						case aegis::BindFlag::DepthStencil:
							flag |= D3D11_BIND_DEPTH_STENCIL;
							break;
						case aegis::BindFlag::UnorderedAccess:
							flag |= D3D11_BIND_UNORDERED_ACCESS;
							break;
						case aegis::BindFlag::Decoder:
							flag |= D3D11_BIND_DECODER;
							break;
						case aegis::BindFlag::VideoEncoder:
							flag |= D3D11_BIND_VIDEO_ENCODER;
							break;
					}
				}
			}

			return flag;
		}

		D3D11_CPU_ACCESS_FLAG Convert2CpuAccessFlag(CpuAccessFlag cpuAccessFlag)
		{
			switch (cpuAccessFlag)
			{
			case CpuAccessFlag::None:
				return static_cast< D3D11_CPU_ACCESS_FLAG>(0);
			case CpuAccessFlag::Write:
				return D3D11_CPU_ACCESS_WRITE;
			case CpuAccessFlag::Read:
				return D3D11_CPU_ACCESS_READ;
			}
		}

		D3D11_VIEWPORT Convert2ViewPort(const ViewPort viewPort)
		{
			D3D11_VIEWPORT view{};
			view.TopLeftX = viewPort.TopLeftX;
			view.TopLeftY = viewPort.TopLeftY;
			view.Width = viewPort.Width;
			view.Height = viewPort.Height;
			view.MinDepth = viewPort.MinDepth;
			view.MaxDepth = viewPort.MaxDepth;

			return view;
		}

		D3D11_FILL_MODE Convert2FillMode(const FillMode fillMode)
		{
			switch (fillMode)
			{
				case FillMode::Wireframe:
					return D3D11_FILL_WIREFRAME;
				case FillMode::Solid:
					return D3D11_FILL_SOLID;
			}
		}

		D3D11_CULL_MODE Convert2CullMode(const CullMode cullMode)
		{
			switch (cullMode)
			{
				case CullMode::None:
					return D3D11_CULL_NONE;
				case CullMode::Front:
					return D3D11_CULL_FRONT;
				case CullMode::Back:
					return D3D11_CULL_BACK;
			}
		}

		D3D11_RASTERIZER_DESC Convert2RasterizeState(const RasterizeState rasterizeState)
		{
			D3D11_RASTERIZER_DESC desc{};
			desc.FillMode = Details::Convert2FillMode(rasterizeState.FillMode);
			desc.CullMode = Details::Convert2CullMode(rasterizeState.CullMode);
			desc.FrontCounterClockwise = rasterizeState.FrontCounterClockwise;
			desc.DepthBias = rasterizeState.DepthBias;
			desc.DepthBiasClamp = rasterizeState.DepthBiasClamp;
			desc.SlopeScaledDepthBias = rasterizeState.SlopeScaledDepthBias;
			desc.DepthClipEnable = rasterizeState.DepthClipEnable;
			desc.ScissorEnable = rasterizeState.ScissorEnable;
			desc.MultisampleEnable = rasterizeState.MultisampleEnable;
			desc.AntialiasedLineEnable = rasterizeState.AntialiasedLineEnable;

			return desc;
		}

		D3D11_FILTER Convert2Filter(const Filter filter)
		{
			switch (filter)
			{
				case Filter::Point:
					return D3D11_FILTER_MIN_MAG_MIP_POINT;
				case Filter::Linear:
					return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				case Filter::Anisotropic:
					return D3D11_FILTER_ANISOTROPIC;
			}
		}

		D3D11_TEXTURE_ADDRESS_MODE Convert2AddressMode(const AddressMode Address)
		{
			switch (Address)
			{
				case AddressMode::Wrap:
					return D3D11_TEXTURE_ADDRESS_WRAP;
				case AddressMode::Mirror:
					return D3D11_TEXTURE_ADDRESS_MIRROR;
				case AddressMode::Clamp:
					return D3D11_TEXTURE_ADDRESS_CLAMP;
				case AddressMode::Border:
					return D3D11_TEXTURE_ADDRESS_BORDER;
				case AddressMode::MirrorOnce:
					return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
			}
		}

		D3D11_COMPARISON_FUNC Convert2ComparisonFunc(const ComparisonFunc comparisonFunc)
		{
			switch (comparisonFunc)
			{
				case ComparisonFunc::Never:
					return D3D11_COMPARISON_NEVER;
				case ComparisonFunc::Less:
					return D3D11_COMPARISON_LESS;
				case ComparisonFunc::Equal:
					return D3D11_COMPARISON_EQUAL;
				case ComparisonFunc::LessEqual:
					return D3D11_COMPARISON_LESS_EQUAL;
				case ComparisonFunc::Greater:
					return D3D11_COMPARISON_GREATER;
				case ComparisonFunc::NotEqual:
					return D3D11_COMPARISON_NOT_EQUAL;
				case ComparisonFunc::GreaterEqual:
					return D3D11_COMPARISON_GREATER_EQUAL;
				case ComparisonFunc::Always:
					return D3D11_COMPARISON_ALWAYS;
			}
		}

		DXGI_SAMPLE_DESC Convert2SampleDesc(const SampleDesc desc)
		{
			DXGI_SAMPLE_DESC sampleDesc{};
			sampleDesc.Count = desc.Count;
			sampleDesc.Quality = desc.Quality;

			return sampleDesc;
		}

		DXGI_FORMAT Convert2Format(Format format)
		{
			switch (format)
			{
			case aegis::Format::Unknown:
			case aegis::Format::RGBA32Typeless:
			case aegis::Format::RGBA32Float:
			case aegis::Format::RGBA32Uint:
			case aegis::Format::RGBA32Int:
			case aegis::Format::RGB32Typeless:
			case aegis::Format::RGB32Float:
			case aegis::Format::RGB32Uint:
			case aegis::Format::RGB32Int:
			case aegis::Format::RGBA16Typeless:
			case aegis::Format::RGBA16Float:
			case aegis::Format::RGBA16Unorm:
			case aegis::Format::RGBA16Uint:
			case aegis::Format::RGBA16Snorm:
			case aegis::Format::RGBA16Int:
			case aegis::Format::RG32Typeless:
			case aegis::Format::RG32Float:
			case aegis::Format::RG32Uint:
			case aegis::Format::RG32Int:
			case aegis::Format::R32G8X24Typeless:
			case aegis::Format::D32FloatS8X24Uint:
			case aegis::Format::R32FloatX8X24Typeless:
			case aegis::Format::X32TypelessG8X24Uint:
			case aegis::Format::RGB10A2Typeless:
			case aegis::Format::RGB10A2Unorm:
			case aegis::Format::RGB10A2Uint:
			case aegis::Format::R11G11B10_Float:
			case aegis::Format::RGBA8Typeless:
			case aegis::Format::RGBA8Unorm:
			case aegis::Format::RGBA8UnormSrgb:
			case aegis::Format::RGBA8Uint:
			case aegis::Format::RGBA8Snorm:
			case aegis::Format::RGBA8Int:
			case aegis::Format::RG16Typeless:
			case aegis::Format::RG16Float:
			case aegis::Format::RG16Unorm:
			case aegis::Format::RG16Uint:
			case aegis::Format::RG16Snorm:
			case aegis::Format::RG16Int:
			case aegis::Format::R32Typeless:
			case aegis::Format::D32Float:
			case aegis::Format::R32Float:
			case aegis::Format::R32Uint:
			case aegis::Format::R32Int:
				return static_cast<DXGI_FORMAT>(format);

			case aegis::Format::RG8Typeless:
			case aegis::Format::RG8Unorm:
			case aegis::Format::RG8Uint:
			case aegis::Format::RG8Snorm:
			case aegis::Format::RG8Int:
			case aegis::Format::R16Typeless:
			case aegis::Format::R16Float:
			case aegis::Format::D16Unorm:
			case aegis::Format::R16Unorm:
			case aegis::Format::R16Uint:
			case aegis::Format::R16Snorm:
			case aegis::Format::R16Int:
			case aegis::Format::R8Typeless:
			case aegis::Format::R8Unorm:
			case aegis::Format::R8Uint:
			case aegis::Format::R8Snorm:
			case aegis::Format::R8Int:
			case aegis::Format::A8Unorm:
				return static_cast<DXGI_FORMAT>(static_cast<int32>(format) + 4);
			}
		}

		void Convert2ShaderResourceViewDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& srvDesc, const ShaderResourceViewDesc desc)
		{
			srvDesc.Format = Convert2Format(desc.Format);

			switch (desc.ViewDimension)
			{
				case SrvDimension::Unknown:
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_UNKNOWN;
					break;
				case SrvDimension::Buffer:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
					srvDesc.Buffer.FirstElement = desc.Buffer.FirstElement;
					srvDesc.Buffer.ElementOffset = desc.Buffer.ElementOffset;
					srvDesc.Buffer.NumElements = desc.Buffer.NumElements;
					srvDesc.Buffer.ElementWidth = desc.Buffer.ElementWidth;
				}
				break;
				case SrvDimension::Texture1D:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
					srvDesc.Texture1D.MostDetailedMip = desc.Texture1D.MostDetailedMip;
					srvDesc.Texture1D.MipLevels = desc.Texture1D.MipLevels;
				}
				break;
				case SrvDimension::Texture1DArray:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
					srvDesc.Texture1DArray.MostDetailedMip = desc.Texture1DArray.MostDetailedMip;
					srvDesc.Texture1DArray.MipLevels = desc.Texture1DArray.MipLevels;
					srvDesc.Texture1DArray.FirstArraySlice = desc.Texture1DArray.FirstArraySlice;
					srvDesc.Texture1DArray.ArraySize = desc.Texture1DArray.ArraySize;
				}
				break;
				case SrvDimension::Texture2D:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MostDetailedMip = desc.Texture2D.MostDetailedMip;
					srvDesc.Texture2D.MipLevels = desc.Texture2D.MipLevels;
				}
				break;
				case SrvDimension::Texture2DArray:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					srvDesc.Texture2DArray.MostDetailedMip = desc.Texture2DArray.MostDetailedMip;
					srvDesc.Texture2DArray.MipLevels = desc.Texture2DArray.MipLevels;
					srvDesc.Texture2DArray.FirstArraySlice = desc.Texture2DArray.FirstArraySlice;
					srvDesc.Texture2DArray.ArraySize = desc.Texture2DArray.ArraySize;
				}
				break;
				case SrvDimension::Texture2DMS:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
					srvDesc.Texture2DMS.UnusedField_NothingToDefine = desc.Texture2DMS.UnusedField_NothingToDefine;
				}
				break;
				case SrvDimension::Texture2DMSArray:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
					srvDesc.Texture2DMSArray.FirstArraySlice = desc.Texture2DMSArray.FirstArraySlice;
					srvDesc.Texture2DMSArray.ArraySize = desc.Texture2DMSArray.ArraySize;
				}
				break;
				case SrvDimension::Texture3D:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
					srvDesc.Texture3D.MostDetailedMip = desc.Texture3D.MostDetailedMip;
					srvDesc.Texture3D.MipLevels = desc.Texture3D.MipLevels;
				}
				break;
				case SrvDimension::TextureCube:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
					srvDesc.TextureCube.MostDetailedMip = desc.TextureCube.MostDetailedMip;
					srvDesc.TextureCube.MipLevels = desc.TextureCube.MipLevels;
				}
				break;
				case SrvDimension::TextureCubeArray:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
					srvDesc.TextureCubeArray.MostDetailedMip = desc.TextureCubeArray.MostDetailedMip;
					srvDesc.TextureCubeArray.MipLevels = desc.TextureCubeArray.MipLevels;
					srvDesc.TextureCubeArray.First2DArrayFace = desc.TextureCubeArray.First2DArrayFace;
					srvDesc.TextureCubeArray.NumCubes = desc.TextureCubeArray.NumCubes;
				}
				break;
				case SrvDimension::BufferEx:
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
					srvDesc.BufferEx.FirstElement = desc.BufferEx.FirstElement;
					srvDesc.BufferEx.NumElements = desc.BufferEx.NumElements;
					srvDesc.BufferEx.Flags = desc.BufferEx.Flags;
				}
				break;
			}
		}

		void Convert2DepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC& desc, const DepthStencilViewDesc depthStencilViewDesc)
		{
			desc.Format = Convert2Format(depthStencilViewDesc.Format);

			switch (depthStencilViewDesc.ViewDimension)
			{
				case DsvDimension::Unknown:
					desc.ViewDimension = D3D11_DSV_DIMENSION_UNKNOWN;
					break;
				case DsvDimension::Texture1D:
				{
					desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
					desc.Texture1D.MipSlice = depthStencilViewDesc.Texture1D.MipSlice;
				}
				break;
				case DsvDimension::Texture1DArray:
				{
					desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
					desc.Texture1DArray.MipSlice = depthStencilViewDesc.Texture1DArray.MipSlice;
					desc.Texture1DArray.FirstArraySlice = depthStencilViewDesc.Texture1DArray.FirstArraySlice;
					desc.Texture1DArray.ArraySize = depthStencilViewDesc.Texture1DArray.ArraySize;
				}
				break;
				case DsvDimension::Texture2D:
				{
					desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					desc.Texture2D.MipSlice = depthStencilViewDesc.Texture2D.MipSlice;
				}
				break;
				case DsvDimension::Texture2DArray:
				{
					desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
					desc.Texture2DArray.MipSlice = depthStencilViewDesc.Texture2DArray.MipSlice;
					desc.Texture2DArray.FirstArraySlice = depthStencilViewDesc.Texture2DArray.FirstArraySlice;
					desc.Texture2DArray.ArraySize = depthStencilViewDesc.Texture2DArray.ArraySize;
				}
				break;
				case DsvDimension::Texture2DMS:
				{
					desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
					desc.Texture2DMS.UnusedField_NothingToDefine = depthStencilViewDesc.Texture2DMS.UnusedField_NothingToDefine;
				}
				break;
				case DsvDimension::Texture2DMSArray:
				{
					desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
					desc.Texture2DMSArray.FirstArraySlice = depthStencilViewDesc.Texture2DMSArray.FirstArraySlice;
					desc.Texture2DMSArray.ArraySize = depthStencilViewDesc.Texture2DMSArray.ArraySize;
				}
				break;
			}
		}

		void Convert2UnorderedAccessViewDesc(D3D11_UNORDERED_ACCESS_VIEW_DESC& desc, const UnorderedAccessViewDesc unorderedAccessViewDesc)
		{
			desc.Format = Convert2Format(unorderedAccessViewDesc.Format);

			switch (unorderedAccessViewDesc.ViewDimension)
			{
			case UavDimension::Unknown:
				desc.ViewDimension = D3D11_UAV_DIMENSION_UNKNOWN;
				break;
			case UavDimension::Buffer:
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				desc.Buffer.FirstElement = unorderedAccessViewDesc.Buffer.FirstElement;
				desc.Buffer.NumElements = unorderedAccessViewDesc.Buffer.NumElements;
				desc.Buffer.Flags = unorderedAccessViewDesc.Buffer.Flags;
			}
			break;
			case UavDimension::Texture1D:
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = unorderedAccessViewDesc.Texture1D.MipSlice;
			}
			break;
			case UavDimension::Texture1DArray:
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipSlice = unorderedAccessViewDesc.Texture1DArray.MipSlice;
				desc.Texture1DArray.FirstArraySlice = unorderedAccessViewDesc.Texture1DArray.FirstArraySlice;
				desc.Texture1DArray.ArraySize = unorderedAccessViewDesc.Texture1DArray.ArraySize;
			}
			break;
			case UavDimension::Texture2D:
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = unorderedAccessViewDesc.Texture2D.MipSlice;
			}
			break;
			case UavDimension::Texture2DArray:
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = unorderedAccessViewDesc.Texture2DArray.MipSlice;
				desc.Texture2DArray.FirstArraySlice = unorderedAccessViewDesc.Texture2DArray.FirstArraySlice;
				desc.Texture2DArray.ArraySize = unorderedAccessViewDesc.Texture2DArray.ArraySize;
			}
			break;
			case UavDimension::Texture3D:
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = unorderedAccessViewDesc.Texture3D.MipSlice;
				desc.Texture3D.FirstWSlice = unorderedAccessViewDesc.Texture3D.FirstWSlice;
				desc.Texture3D.WSize = unorderedAccessViewDesc.Texture3D.WSize;
			}
			break;
			}
		}
	}

	bool RenderTargetViewDX11::IsEmpty() const
	{
		return nullptr == RenderTargetView;
	}
	void RenderTargetViewDX11::Release()
	{
		//if (RenderTargetView)
		//	RenderTargetView->Release();
	}

	bool DepthStencilViewDX11::IsEmpty() const
	{
		return nullptr == DepthStencilView;
	}
	void DepthStencilViewDX11::Release()
	{
		//if (DepthStencilView)
		//	DepthStencilView->Release();
	}

	bool RasterizerStateDX11::IsEmpty() const
	{
		return nullptr == RasterizerState;
	}
	void RasterizerStateDX11::Release()
	{
		//if(RasterizerState)
		//	RasterizerState->Release();
	}

	bool BufferDX11::IsEmpty() const
	{
		return nullptr == Buffer;
	}
	void BufferDX11::Release()
	{
		//if(Buffer)
		//	Buffer->Release();

		if (Buffer)
		{
			ComPtr<ID3D11Buffer> temp;

			Buffer.Swap(temp);
		}
	}

	bool Texture1DDX11::IsEmpty() const
	{
		return nullptr == Texture;
	}
	void Texture1DDX11::Release()
	{
		//if (Texture)
		//	Texture->Release();
	}

	bool Texture2DDX11::IsEmpty() const
	{
		return nullptr == Texture;
	}
	void Texture2DDX11::Release()
	{
		//if (Texture)
		//	Texture->Release();
	}

	bool Texture3DDX11::IsEmpty() const
	{
		return nullptr == Texture;
	}
	void Texture3DDX11::Release()
	{
		//if (Texture)
		//	Texture->Release();
	}

	bool ShaderResourceViewDX11::IsEmpty() const
	{
		return nullptr == ShaderResourceView;
	}
	void ShaderResourceViewDX11::Release()
	{
		//if (ShaderResourceView)
		//	ShaderResourceView->Release();

		//ShaderResourceView = nullptr;

		if (ShaderResourceView)
		{
			ComPtr<ID3D11ShaderResourceView> temp;

			ShaderResourceView.Swap(temp);
		}
	}

	bool SamplerStateDX11::IsEmpty() const
	{
		return nullptr == Sampler;
	}
	void SamplerStateDX11::Release()
	{
		//if (Sampler)
		//	Sampler->Release();
	}

	bool UnorderedAccessViewDX11::IsEmpty() const
	{
		return nullptr == UnorderedAccessView;
	}
	void UnorderedAccessViewDX11::Release()
	{
		//if (UnorderedAccessView)
		//	UnorderedAccessView->Release();
	}

	bool PixelShaderDX11::IsEmpty() const
	{
		return nullptr == Shader;
	}
	void PixelShaderDX11::Release()
	{
		//if (Shader)
		//	Shader->Release();
	}

	bool VertexShaderDX11::IsEmpty() const
	{
		return nullptr == Shader;
	}
	void VertexShaderDX11::Release()
	{
		//if (Shader)
		//	Shader->Release();
	}

	bool ComputeShaderDX11::IsEmpty() const
	{
		return nullptr == Shader;
	}
	void ComputeShaderDX11::Release()
	{
		//if (Shader)
		//	Shader->Release();
	}



	RenderDX11::RenderDX11()
	{
		mType = RenderApiType::DirectX11;
	}

	RenderDX11::~RenderDX11() {}

	bool RenderDX11::Init()
	{
		HRESULT hr = S_OK;

		if (false == Init3D())
		{
			return false;
		}

		if (false == Init2D())
		{
			return false;
		}

		// 頂点シェーダ生成
		{
			// 入力レイアウト生成
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			UINT numElements = ARRAYSIZE(layout);


			// 頂点シェーダ生成 デフォルト
			{
				FILE* file;
				long int fsize;

				file = fopen("VertexShader.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreateVertexShader(buffer, fsize, nullptr, &mVertexShader[aegis::SHADER_INDEX_V::DEFAULT]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				hr = mD3DDevice->CreateInputLayout(layout, numElements, buffer, fsize, &mVertexLayout[aegis::INPUTLAYOUT::DEFAULT]);
				delete[] buffer;

				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}
			}

			// 頂点シェーダ生成 スカイボックス
			{
				FILE* file;
				long int fsize;

				file = fopen("VertexShader_Skybox.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreateVertexShader(buffer, fsize, nullptr, &mVertexShader[aegis::SHADER_INDEX_V::SKYBOX]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				delete[] buffer;
			}

			// 頂点シェーダ生成 シャドウマップ
			{
				FILE* file;
				long int fsize;

				file = fopen("VertexShader_ShadowMap.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreateVertexShader(buffer, fsize, nullptr, &mVertexShader[aegis::SHADER_INDEX_V::SHADOW_MAP]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				delete[] buffer;
			}

			// 頂点シェーダ生成 (Depth Pre Pass)
			{
				FILE* file;
				long int fsize;

				file = fopen("VertexShader_Depth_Pre_Pass.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreateVertexShader(buffer, fsize, nullptr, &mVertexShader[aegis::SHADER_INDEX_V::DEPTH_PRE]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				delete[] buffer;
			}
		}

		// 頂点シェーダ生成 アニメーション
		{
		// 入力レイアウト生成
			D3D11_INPUT_ELEMENT_DESC animation_layout[] =
			{
				{ "POSITION",	 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",		 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",		 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",	 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BLENDINDICE", 0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			UINT numElements = ARRAYSIZE(animation_layout);

			// 頂点シェーダ生成 アニメーション
			{
				FILE* file;
				long int fsize;

				file = fopen("VertexShader_Animation.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreateVertexShader(buffer, fsize, nullptr, &mVertexShader[aegis::SHADER_INDEX_V::ANIMATION]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				hr = mD3DDevice->CreateInputLayout(animation_layout, numElements, buffer, fsize, &mVertexLayout[aegis::INPUTLAYOUT::ANIMATION]);
				delete[] buffer;

				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}
			}

			// 頂点シェーダ生成 アニメーション(シャドウマップ付き)
			{
				FILE* file;
				long int fsize;

				file = fopen("VertexShader_ShadowMap_Anime.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreateVertexShader(buffer, fsize, nullptr, &mVertexShader[aegis::SHADER_INDEX_V::SHADOW_MAP_ANIMATION]);
				delete[] buffer;

				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}
			}

			// 頂点シェーダ生成 アニメーション(Depth Pre Pass)
			{
				FILE* file;
				long int fsize;

				file = fopen("VertexShader_Animation_Depth_Pre_Pass.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreateVertexShader(buffer, fsize, nullptr, &mVertexShader[aegis::SHADER_INDEX_V::DEPTH_PRE_ANIME]);
				delete[] buffer;

				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}
			}
		}

		// 頂点シェーダ生成 インスタンシング
		{
			// 入力レイアウト生成
			D3D11_INPUT_ELEMENT_DESC animation_layout[] =
			{
				{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
				{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
				{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
				{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },

				{ "MATRIX",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
				{ "MATRIX",		1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
				{ "MATRIX",		2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
				{ "MATRIX",		3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
			};

			UINT numElements = ARRAYSIZE(animation_layout);

			{
				FILE* file;
				long int fsize;

				file = fopen("VertexShader_Instancing.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreateVertexShader(buffer, fsize, nullptr, &mVertexShader[aegis::SHADER_INDEX_V::INSTANCING]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				hr = mD3DDevice->CreateInputLayout(animation_layout, numElements, buffer, fsize, &mVertexLayout[aegis::INPUTLAYOUT::INSTANCING]);
				delete[] buffer;

				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}
			}
		}

		// ピクセルシェーダ生成
		{
			// 標準
			{
				FILE* file;
				long int fsize;

				file = fopen("PixelShader.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreatePixelShader(buffer, fsize, nullptr, &mPixelShader[aegis::SHADER_INDEX_P::DEFAULT]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				delete[] buffer;
			}

			// テクスチャなし
			{
				FILE* file;
				long int fsize;

				file = fopen("PixelShader_No_Texture.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreatePixelShader(buffer, fsize, nullptr, &mPixelShader[aegis::SHADER_INDEX_P::NO_TEXTURE]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				delete[] buffer;
			}

			// ライティングなし
			{
				FILE* file;
				long int fsize;

				file = fopen("PixelShader_No_Light.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreatePixelShader(buffer, fsize, nullptr, &mPixelShader[aegis::SHADER_INDEX_P::NO_LIGHT]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				delete[] buffer;
			}

			// スカイボックス
			{
				FILE* file;
				long int fsize;

				file = fopen("PixelShader_Skybox.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreatePixelShader(buffer, fsize, nullptr, &mPixelShader[aegis::SHADER_INDEX_P::SKYBOX]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				delete[] buffer;
			}

			// シャドウマップ
			{
				FILE* file;
				long int fsize;

				file = fopen("PixelShader_ShadowMap.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreatePixelShader(buffer, fsize, nullptr, &mPixelShader[aegis::SHADER_INDEX_P::SHADOW_MAP]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				delete[] buffer;
			}

			// ポストエフェクト
			{
				FILE* file;
				long int fsize;

				file = fopen("pixelShader _PostEffect.cso", "rb");
				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				hr = mD3DDevice->CreatePixelShader(buffer, fsize, nullptr, &mPixelShader[aegis::SHADER_INDEX_P::POST_EFFECT]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				delete[] buffer;
			}
		}

		mVertexShader[aegis::SHADER_INDEX_V::MAX] = nullptr;
		mPixelShader[aegis::SHADER_INDEX_P::MAX] = nullptr;

		// 定数バッファ生成
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(aegis::MATERIAL);

		hr = mD3DDevice->CreateBuffer(&hBufferDesc, nullptr, &mMaterialBuffer);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		mImmediateContext->VSSetConstantBuffers(3, 1, mMaterialBuffer.GetAddressOf());
		mImmediateContext->PSSetConstantBuffers(3, 1, mMaterialBuffer.GetAddressOf());

		hBufferDesc.ByteWidth = sizeof(aegis::LIGHT);

		hr = mD3DDevice->CreateBuffer(&hBufferDesc, nullptr, &mLightBuffer);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		mImmediateContext->VSSetConstantBuffers(4, 1, mLightBuffer.GetAddressOf());
		mImmediateContext->PSSetConstantBuffers(4, 1, mLightBuffer.GetAddressOf());

		hBufferDesc.ByteWidth = sizeof(aegis::CONSTANT);
		hr = mD3DDevice->CreateBuffer(&hBufferDesc, nullptr, &mConstantBuffer);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		mImmediateContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
		mImmediateContext->PSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

		hBufferDesc.ByteWidth = sizeof(aegis::CONSTANT_02);
		hr = mD3DDevice->CreateBuffer(&hBufferDesc, nullptr, &mConstantBuffer_02);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		mImmediateContext->VSSetConstantBuffers(5, 1, mConstantBuffer_02.GetAddressOf());
		mImmediateContext->PSSetConstantBuffers(5, 1, mConstantBuffer_02.GetAddressOf());

		// 入力レイアウト設定
		mImmediateContext->IASetInputLayout(mVertexLayout[0].Get());

		// シェーダ設定
		mImmediateContext->VSSetShader(mVertexShader[aegis::SHADER_INDEX_V::DEFAULT].Get(), nullptr, 0);
		mImmediateContext->PSSetShader(mPixelShader[aegis::SHADER_INDEX_P::DEFAULT].Get(), nullptr, 0);

		// ライト
		SetLight(GetLight());

		// マテリアル初期化
		aegis::MATERIAL material = {};
		material.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
		material.Ambient = COLOR(0.5f, 0.5f, 0.5f, 1.0f);
		material.Specular = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		return true;
	}

	void RenderDX11::Uninit()
	{
		BOOL FullScreen = FALSE;
		if (nullptr != mSwapChain)
		{
			mSwapChain->GetFullscreenState(&FullScreen, nullptr);

			// フルスクリーンの時
			if (FullScreen == TRUE)
			{
				mSwapChain->SetFullscreenState(FALSE, nullptr);
			}
		}

		//mD3DDevice->Release();

		mImmediateContext->Release();
		mSwapChain->Release();
		mRenderTargetView->Release();

		mRenderTargetView_16bit->Release();
		mShaderResourceView_16bit->Release();

		for (uint32 i = 0; i < 3; i++)
		{
			if (mRenderTargetViews[i])
			{
				mRenderTargetViews[i]->Release();
			}

			if (mShaderResourceViews[i])
			{
				mShaderResourceViews[i]->Release();
			}
		}
		
		mDepthStencilView->Release();

		mD2DDevice->Release();
		mD2DDeviceContext->Release();
		mD2DTargetBitmap->Release();
		mDxgiDev->Release();

		mDwriteTextFormat->Release();
		mTextLayout->Release();
		mDwriteFactory->Release();

		for (auto& shader : mVertexShader)
		{
			if (shader.second)
			{
				shader.second->Release();
			}
		}

		for (auto& shader : mPixelShader)
		{
			if (shader.second)
			{
				shader.second->Release();
			}
		}

		mDepthStateEnable->Release();
		mDepthStateDisable->Release();
		mRasterizerState->Release();

		mSamplerState->Release();

		for (uint32 i = 0; i < 3; i++)
		{
			if (mVertexLayout[i])
			{
				mVertexLayout[i]->Release();
			}
		}

		mMaterialBuffer->Release();
		mLightBuffer->Release();
		mConstantBuffer->Release();
		mConstantBuffer_02->Release();

		//{
		//	// 作成
		//	HRESULT hr = mD3DDevice->QueryInterface(IID_PPV_ARGS(&gpD3dDebug));
		//	
		//	mD3DDevice->Release();
		//	
		//	if (FAILED(hr))
		//	{
		//		return;
		//	}
		//}
	}

	void RenderDX11::Begin()
	{
		auto render_target = mRenderTargetView_16bit.Get();

		// バックバッファクリア
		float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		//mImmediateContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
		mImmediateContext->OMSetRenderTargets(1, &render_target, mDepthStencilView.Get());//
		//mImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), ClearColor);
		mImmediateContext->ClearRenderTargetView(render_target, ClearColor);//
		mImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void RenderDX11::End()
	{
		if (false == mStandByEnable)
		{
			// 画面に描画する
			HRESULT hr = mSwapChain->Present(0, 0);

			if (DXGI_STATUS_OCCLUDED == hr)
			{
				mStandByEnable = true;		// スタンバイモードに入る
				return;
			}

			// デバイスの消失
			{
				hr = mD3DDevice->GetDeviceRemovedReason();

				switch (hr)
				{
				case S_OK:
					break;

					// リセット
				case DXGI_ERROR_DEVICE_HUNG:
				case DXGI_ERROR_DEVICE_RESET:
					///CManager::Get_Instance()->GameEnd();
					break;

					// エラー 終了
				case DXGI_ERROR_DEVICE_REMOVED:
				case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
				case DXGI_ERROR_INVALID_CALL:
				default:
					///CManager::Get_Instance()->GameEnd();
					break;
				}
			}
		}
		else
		{
			// 描画しない
			HRESULT hr = mSwapChain->Present(0, DXGI_PRESENT_TEST);

			if (DXGI_STATUS_OCCLUDED != hr)
			{
				// スタンバイモードを解除する
				mStandByEnable = false;
			}
		}
	}

	void RenderDX11::SetBlendState(BlendDesc* blendState, bool flag)
	{
		if (flag)
		{
			D3D11_BLEND_DESC desc = Details::Convert2BlendDesc(*blendState);

			// ブレンドステート設定
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			ID3D11BlendState* blendState = nullptr;
			mD3DDevice->CreateBlendState(&desc, &blendState);
			mImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
		}
		else
		{
			// ブレンドステート設定 ( 初期設定 )
			D3D11_BLEND_DESC blendDesc = {};
			blendDesc.AlphaToCoverageEnable = FALSE;
			blendDesc.IndependentBlendEnable = FALSE;
			for (char i = 0; i < 3; i++)
			{
				blendDesc.RenderTarget[i].BlendEnable = TRUE;
				blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
				blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
				blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			}

			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			ID3D11BlendState* blendState = nullptr;
			mD3DDevice->CreateBlendState(&blendDesc, &blendState);
			mImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
		}
	}

	void RenderDX11::GetAddBlendState(BlendDesc& blendState)
	{
		blendState.AlphaToCoverageEnable = false;
		blendState.IndependentBlendEnable = false;
		for (char i = 0; i < 3; i++)
		{
			blendState.RenderTarget[i].BlendEnable = true;
			blendState.RenderTarget[i].SrcBlend = Blend::BlendInvSrcAlpha;
			blendState.RenderTarget[i].DestBlend = Blend::BlendOne;
			blendState.RenderTarget[i].BlendOp = BlendOp::BlendOpAdd;
			blendState.RenderTarget[i].SrcBlendAlpha = Blend::BlendOne;
			blendState.RenderTarget[i].DestBlendAlpha = Blend::BlendZero;
			blendState.RenderTarget[i].BlendOpAlpha = BlendOp::BlendOpAdd;
			blendState.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
	}

	void RenderDX11::GetSubtractBlendState(BlendDesc& blendState)
	{
		blendState.AlphaToCoverageEnable = false;
		blendState.IndependentBlendEnable = false;
		for (char i = 0; i < 3; i++)
		{
			blendState.RenderTarget[i].BlendEnable = true;
			blendState.RenderTarget[i].SrcBlend = Blend::BlendSrcAlpha;
			blendState.RenderTarget[i].DestBlend = Blend::BlendOne;
			blendState.RenderTarget[i].BlendOp = BlendOp::BlendOpRevSubtract;
			blendState.RenderTarget[i].SrcBlendAlpha = Blend::BlendZero;
			blendState.RenderTarget[i].DestBlendAlpha = Blend::BlendOne;
			blendState.RenderTarget[i].BlendOpAlpha = BlendOp::BlendOpAdd;
			blendState.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
	}

	void RenderDX11::Get2DBlendState(BlendDesc& blendState)
	{
		blendState.AlphaToCoverageEnable = false;
		blendState.IndependentBlendEnable = false;
		for (char i = 0; i < 3; i++)
		{
			blendState.RenderTarget[i].BlendEnable = true;
			blendState.RenderTarget[i].SrcBlend = Blend::BlendInvSrcAlpha;
			blendState.RenderTarget[i].DestBlend = Blend::BlendOne;
			blendState.RenderTarget[i].BlendOp = BlendOp::BlendOpAdd;
			blendState.RenderTarget[i].SrcBlendAlpha = Blend::BlendOne;
			blendState.RenderTarget[i].DestBlendAlpha = Blend::BlendZero;
			blendState.RenderTarget[i].BlendOpAlpha = BlendOp::BlendOpAdd;
			blendState.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
	}

	void RenderDX11::SetDepthEnable(bool Enable)
	{
		if (Enable)
			mImmediateContext->OMSetDepthStencilState(mDepthStateEnable.Get(), NULL);
		else
			mImmediateContext->OMSetDepthStencilState(mDepthStateDisable.Get(), NULL);
	}

	void RenderDX11::Change_Window_Mode()
	{
		BOOL FullScreen;

		// GetFullscreenState
		mSwapChain->GetFullscreenState(&FullScreen, nullptr);

		// SetFullscreenState
		mSwapChain->SetFullscreenState(!FullScreen, nullptr);

		// 初期起動をフルスクリーンモードにした場合、ウィンドウモードに変更するとウィンドウがアクティブにならないので表示させる
		ShowWindow(GetWindow(), SW_SHOW);
	}

	bool RenderDX11::Init3D()
	{
		HRESULT hr = S_OK;

		// デバイス、スワップチェーン、コンテキスト生成
		DXGI_SWAP_CHAIN_DESC1 sc = {};
		sc.Width = SCREEN_WIDTH;
		sc.Height = SCREEN_HEIGHT;
		sc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		// スケール方法
		sc.Scaling = DXGI_SCALING_STRETCH;
		// 立体視
		sc.Stereo = 0;
		// 半透明モード
		sc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		// 使用方法
		sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sc.BufferCount = 1;
		// 描画後のバッファの扱い
		sc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
		// MSAA
		sc.SampleDesc.Count = 1;	// MSAA用 2 4 8 が使用可能(多分これだけ)
		sc.SampleDesc.Quality = 0;
		// フラグ
		sc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 解像度変更が有効

		// フラグ
		UINT d3dFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // BGRA テクスチャ有効(Direct2Dには必ず必要)

#if defined(_DEBUG)
		d3dFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// Direct3Dの作成
		{
			// アダプターの列挙
			vector<IDXGIAdapter*> Adapters;
			{
				IDXGIFactory1* pDXGIFactory = nullptr;
				IDXGIAdapter* pAdapter = nullptr;

				DXGI_ADAPTER_DESC desc;
				std::wstring str;

				//ファクトリの作成
				hr = CreateDXGIFactory1(IID_PPV_ARGS(&pDXGIFactory));
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				for (UINT index = 0; ; index++)
				{
					hr = pDXGIFactory->EnumAdapters(index, &pAdapter);
					if (FAILED(hr))
						break;

					if (SUCCEEDED(pAdapter->GetDesc(&desc)))
					{
						str = std::wstring(desc.Description);

						if (std::wstring::npos == str.find(L"Microsoft"))
						{
							Adapters.emplace_back(pAdapter);
						}
						else
						{
							SAFE_RELEASE(pAdapter);
						}
					}
				}

				SAFE_RELEASE(pDXGIFactory);
			}

			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

			// 内蔵GPUじゃないGPUがある
			if (false == Adapters.empty())
			{
				// Direct3Dの作成
				hr = D3D11CreateDevice(Adapters.front(), D3D_DRIVER_TYPE_UNKNOWN, 0, d3dFlags, &featureLevel, 1, D3D11_SDK_VERSION, mD3DDevice.ReleaseAndGetAddressOf(), nullptr, mImmediateContext.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}
			}
			else// 内蔵GPUしかない
			{
				// Direct3Dの作成
				hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, d3dFlags, &featureLevel, 1, D3D11_SDK_VERSION, mD3DDevice.ReleaseAndGetAddressOf(), nullptr, mImmediateContext.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}
			}

			for (auto& adap : Adapters)
			{
				adap->Release();
			}
			Adapters.clear();
		}

		SetName(mD3DDevice.Get(), "D3D11_Device");
		SetName(mImmediateContext.Get(), "D3D11_DeviceContext");

		//// MSAA用
		//for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1)
		//{
		//	UINT Quality;
		//	if (SUCCEEDED(mD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality)))
		//	{
		//		if (0 < Quality)
		//		{
		//			sc.SampleDesc.Count = i;
		//			sc.SampleDesc.Quality = Quality - 1;
		//		}
		//	}
		//}


		// DXGIデバイスの作成
		hr = mD3DDevice->QueryInterface<IDXGIDevice1>(&mDxgiDev);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		SetName(mDxgiDev.Get(), "DXGI_Device");
		

		// キューに格納されていく描画コマンドをスワップ時に全てフラッシュする
		mDxgiDev->SetMaximumFrameLatency(1);

		// DXGIアダプタ（GPU）の取得
		IDXGIAdapter* adapter = nullptr;
		hr = mDxgiDev->GetAdapter(&adapter);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		{
			IDXGIAdapter3* pAdapter = nullptr;
			DXGI_QUERY_VIDEO_MEMORY_INFO info;

			// DXGIアダプタ（GPU）の取得
			hr = mDxgiDev->GetAdapter((IDXGIAdapter**)&pAdapter);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			pAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info);
		}

		// DXGIのファクトリの作成
		IDXGIFactory2* factory = nullptr;
		hr = adapter->GetParent(IID_PPV_ARGS(&factory));
		adapter->Release();
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// スワップチェインをHWNDから作成
		hr = factory->CreateSwapChainForHwnd(mD3DDevice.Get(), GetWindow(), &sc, nullptr, nullptr, &mSwapChain);
		factory->Release();
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		SetName(mSwapChain.Get(), "SwapChain");

		// レンダーターゲットの取得（D3D11）
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// レンダーターゲットビューの作成
		hr = mD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		SetName(mRenderTargetView.Get(), "MainOutput");

		{
			ID3D11Texture2D* pTex = nullptr;

			// テクスチャの作成
			D3D11_TEXTURE2D_DESC td;
			td.Width = SCREEN_WIDTH;
			td.Height = SCREEN_HEIGHT;
			td.MipLevels = 1;
			td.ArraySize = 1;
			td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			td.SampleDesc.Count = 1;
			td.SampleDesc.Quality = 0;
			td.Usage = D3D11_USAGE_DEFAULT;
			td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			td.CPUAccessFlags = 0;
			td.MiscFlags = 0;

			hr = mD3DDevice->CreateTexture2D(&td, nullptr, &pTex);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			// レンダーターゲットビュー設定
			{
				hr = mD3DDevice->CreateRenderTargetView(pTex, nullptr, &mRenderTargetViews[0]);
				if (FAILED(hr))
				{
					return false;
				}
			}

			// シェーダーリソースビュー設定
			{
				hr = mD3DDevice->CreateShaderResourceView(pTex, nullptr, &mShaderResourceViews[0]);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}
			}

			SetName(mRenderTargetViews[0].Get(), "RenderTargetView01");
			SetName(mShaderResourceViews[0].Get(), "ShaderResourceView01");
		}

		{
			// アルベドテクスチャの作成
			{
				ID3D11Texture2D* pTex = nullptr;

				// テクスチャの作成
				D3D11_TEXTURE2D_DESC td;
				td.Width = SCREEN_WIDTH;
				td.Height = SCREEN_HEIGHT;
				td.MipLevels = 1;
				td.ArraySize = 1;
				td.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
				td.SampleDesc.Count = 1;
				td.SampleDesc.Quality = 0;
				td.Usage = D3D11_USAGE_DEFAULT;
				td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				td.CPUAccessFlags = 0;
				td.MiscFlags = 0;

				hr = mD3DDevice->CreateTexture2D(&td, nullptr, &pTex);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}

				// レンダーターゲットビュー設定
				{
					hr = mD3DDevice->CreateRenderTargetView(pTex, nullptr, &mRenderTargetView_16bit);
					if (FAILED(hr))
					{
						return false;
					}
				}

				// シェーダーリソースビュー設定
				{
					hr = mD3DDevice->CreateShaderResourceView(pTex, nullptr, &mShaderResourceView_16bit);
					if (FAILED(hr))
					{
						FAILDE_ASSERT;
						return false;
					}
				}
			}

			SetName(mRenderTargetView_16bit.Get(), "RenderTargetView");
			SetName(mShaderResourceView_16bit.Get(), "ShaderResourceView");
		}

		//ステンシル用テクスチャー作成
		ID3D11Texture2D* depthTexture = nullptr;
		D3D11_TEXTURE2D_DESC td = {};
		td.Width = sc.Width;
		td.Height = sc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R32_TYPELESS;
		td.SampleDesc = sc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		hr = mD3DDevice->CreateTexture2D(&td, nullptr, &depthTexture);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		//ステンシルターゲット作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		// MSAA用
		//dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		dsvd.Texture2D.MipSlice = 0;
		dsvd.Flags = 0;
		hr = mD3DDevice->CreateDepthStencilView(depthTexture, &dsvd, &mDepthStencilView);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		mImmediateContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

		SetName(mDepthStencilView.Get(), "DepthStencilView");

		//// シェーダーリソースビュー設定
		//{
		//	ID3D11ShaderResourceView* srv = nullptr;

		//	// シェーダーリソースビューの設定
		//	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		//	desc.Format = DXGI_FORMAT_R32_FLOAT;
		//	desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		//	desc.Texture2D.MostDetailedMip = 0;
		//	desc.Texture2D.MipLevels = 1;

		//	hr = mD3DDevice->CreateShaderResourceView(depthTexture, &desc, &srv);
		//	if (FAILED(hr))
		//	{
		//		FAILDE_ASSERT;
		//		return false;
		//	}
		//	ShaderResourceView[0].reset(srv);
		//}


		// ビューポート設定
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		mImmediateContext->RSSetViewports(1, &vp);



		// ラスタライザステート設定
		D3D11_RASTERIZER_DESC rd = {};
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		rd.DepthClipEnable = TRUE;
		rd.MultisampleEnable = FALSE;

		mD3DDevice->CreateRasterizerState(&rd, &mRasterizerState);

		mImmediateContext->RSSetState(mRasterizerState.Get());



		// ブレンドステート設定
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		for (char i = 0; i < 3; i++)
		{
			blendDesc.RenderTarget[i].BlendEnable = TRUE;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		ID3D11BlendState* blendState = nullptr;
		mD3DDevice->CreateBlendState(&blendDesc, &blendState);
		mImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);



		// 深度ステンシルステート設定
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.StencilEnable = FALSE;

		hr = mD3DDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStateEnable);//豺ｱ蠎ｦ譛牙柑繧ｹ繝・・繝・
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		depthStencilDesc.DepthEnable = FALSE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		mD3DDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStateDisable);//豺ｱ蠎ｦ辟｡蜉ｹ繧ｹ繝・・繝・

		mImmediateContext->OMSetDepthStencilState(mDepthStateEnable.Get(), NULL);



		// サンプラーステート設定
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		ID3D11SamplerState* samplerState = nullptr;
		mD3DDevice->CreateSamplerState(&samplerDesc, mSamplerState.GetAddressOf());

		mImmediateContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());

		{
			// サンプラーステート設定
			D3D11_SAMPLER_DESC samplerDesc = {};
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.MipLODBias = 0;
			samplerDesc.MaxAnisotropy = 0;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.MinLOD = -D3D11_FLOAT32_MAX;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			ID3D11SamplerState* samplerState = nullptr;
			mD3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

			mImmediateContext->PSSetSamplers(1, 1, &samplerState);
		}

		return true;
	}

	bool RenderDX11::Init2D()
	{
		HRESULT hr = S_OK;

		// Direct2Dのファクトリーの作成
		ID2D1Factory1* d2dFactory = nullptr;
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), nullptr, reinterpret_cast<void**>(&d2dFactory));
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}


		// Direct2Dデバイスの作成
		hr = d2dFactory->CreateDevice(mDxgiDev.Get(), &mD2DDevice);
		d2dFactory->Release();
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// Direct2Dデバイスコンテクストの作成
		hr = mD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &mD2DDeviceContext);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// DPIの取得
		float dx, dy;
		mD2DDeviceContext->GetDpi(&dx, &dy);

		// レンダーターゲットの取得（DXGI）
		IDXGISurface* surf = nullptr;
		hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&surf));
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// Direct2Dの描画先となるビットマップを作成
		D2D1_BITMAP_PROPERTIES1 d2dProp =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				dx,
				dy);

		hr = mD2DDeviceContext->CreateBitmapFromDxgiSurface(surf, &d2dProp, &mD2DTargetBitmap);
		surf->Release();
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// 描画するDirect2Dビットマップの設定
		mD2DDeviceContext->SetTarget(mD2DTargetBitmap.Get());

		// DirectWriteのファクトリの作成
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(mDwriteFactory), &mDwriteFactory);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// テキストフォーマットの作成
		{
			TEXT_FOMAT fomat;
			fomat.FontName = "メイリオ";

			hr = CreateTextFormat(fomat);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		// テキストレイアウトを作成
		{
			TEXT_LAYOUT layout;
			layout.Text = "Hello HELL World!!!\n地球の未来にご奉仕するにゃん！";
			layout.Width = 400;
			layout.Height = 50;

			hr = CreateTextLayout(layout);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		// 文字の位置の設定
		hr = mDwriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// パラグラフの指定
		hr = mDwriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		return true;
	}

	void RenderDX11::SetWorldViewProjection2D(const XMFLOAT3& scaling, const XMFLOAT3& rotation)
	{
		XMMATRIX world = XMMatrixIdentity();

		world = XMMatrixScaling(scaling.x, scaling.y, 1.0f);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));

		XMMATRIX view;
		view = XMMatrixIdentity();

		XMMATRIX projection;
		projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);

		CONSTANT constant;
		constant.WorldMatrix = XMMatrixTranspose(world);
		constant.ViewMatrix = XMMatrixTranspose(view);
		constant.ProjectionMatrix = XMMatrixTranspose(projection);

		mImmediateContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &constant, 0, 0);
	}

	void RenderDX11::Set_MatrixBuffer(const XMMATRIX world, const XMMATRIX view, const XMMATRIX projection)
	{
		CONSTANT constant;
		constant.WorldMatrix = XMMatrixTranspose(world);
		constant.ViewMatrix = XMMatrixTranspose(view);
		constant.ProjectionMatrix = XMMatrixTranspose(projection);

		mImmediateContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &constant, 0, 0);
	}

	void RenderDX11::Set_MatrixBuffer01(const XMVECTOR camera_pos)
	{
		CONSTANT_02 cons;
		XMFLOAT4 pos;

		XMStoreFloat4(&pos, camera_pos);

		cons.Camera_Pos.x = pos.x;
		cons.Camera_Pos.y = pos.y;
		cons.Camera_Pos.z = pos.z;
		cons.Camera_Pos.w = 0.0f;

		mImmediateContext->UpdateSubresource(mConstantBuffer_02.Get(), 0, NULL, &cons, 0, 0);

		mImmediateContext->VSSetConstantBuffers(5, 1, mConstantBuffer_02.GetAddressOf());
		mImmediateContext->PSSetConstantBuffers(5, 1, mConstantBuffer_02.GetAddressOf());
	}

	void RenderDX11::SetMaterial(aegis::MATERIAL Material)
	{
		mImmediateContext->UpdateSubresource(mMaterialBuffer.Get(), 0, nullptr, &Material, 0, 0);
	}

	void RenderDX11::SetLight(aegis::LIGHT* Light)
	{
		XMVECTOR vec = XMLoadFloat4(&Light->Direction);
		vec = XMVector4Normalize(vec);
		XMStoreFloat4(&Light->Direction, vec);

		mImmediateContext->UpdateSubresource(mLightBuffer.Get(), 0, nullptr, Light, 0, 0);
	}

	void RenderDX11::Light_Identity()
	{
		// ライト初期化
		LIGHT light;
		light.Direction = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
		light.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Ambient = COLOR(0.5f, 0.5f, 0.5f, 1.0f);
		light.Specular = COLOR(1.0f, 1.0f, 1.0f, 1.0f);

		mImmediateContext->UpdateSubresource(mLightBuffer.Get(), 0, nullptr, &light, 0, 0);
	}

	void RenderDX11::Set_RasterizerState()
	{
		mImmediateContext->RSSetState(mRasterizerState.Get());
	}

	DepthStencilView* RenderDX11::CreateDepthStencilView(Texture2D* texture, DepthStencilViewDesc depthStencilViewDesc) const
	{
		ComPtr<ID3D11DepthStencilView> depthStencilView;

		D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
		Details::Convert2DepthStencilViewDesc(desc, depthStencilViewDesc);

		HRESULT hr = S_OK;
		if (texture != nullptr)
		{
			hr = mD3DDevice->CreateDepthStencilView(static_cast<Texture2DDX11*>(texture)->Texture.Get(), &desc, &depthStencilView);
		}
		else
		{
			hr = mD3DDevice->CreateDepthStencilView(nullptr ,&desc, depthStencilView.GetAddressOf());
		}

		if (FAILED(hr))
			FAILDE_ASSERT;

		DepthStencilViewDX11* buf = aegis::allocatorWrapper::allocate<DepthStencilViewDX11>();
		buf->DepthStencilView = depthStencilView;

		return buf;
	}

	void RenderDX11::SetRenderTargets(uint32 numViews, aegis::RenderTargetView* const* renderTargetViews, DepthStencilView* depthStencilView) const
	{
		ID3D11DepthStencilView* depthView = nullptr;
		if (depthStencilView != nullptr)
		{
			depthView = static_cast<DepthStencilViewDX11*>(depthStencilView)->DepthStencilView.Get();
		}

		if (renderTargetViews != nullptr)
		{
			const uint32 size = sizeof(renderTargetViews) / sizeof(renderTargetViews[0]);
			aegis::array<ID3D11RenderTargetView*, size> datas;
			datas.fill(nullptr);

			for (uint32 i = 0; i < size; i++)
			{
				if (renderTargetViews[i] != nullptr)
				{
					datas[i] = static_cast<RenderTargetViewDX11*>(renderTargetViews[i])->RenderTargetView.Get();
				}
			}

			mImmediateContext->OMSetRenderTargets(numViews, datas.data(), depthView);
		}
		else
		{
			ID3D11RenderTargetView* rtv = nullptr;
			mImmediateContext->OMSetRenderTargets(numViews, &rtv, depthView);
		}
	}

	void RenderDX11::ClearRenderTargetView(aegis::RenderTargetView* renderTargetView, float32 color[4]) const
	{
		mImmediateContext->ClearRenderTargetView(static_cast<RenderTargetViewDX11*>(renderTargetView)->RenderTargetView.Get(), color);
	}

	void RenderDX11::ClearDepthStencilView(DepthStencilView* depthStencilView, ClearFlag clearFlags, float32 depth, uint8 stencil) const
	{
		mImmediateContext->ClearDepthStencilView(static_cast<DepthStencilViewDX11*>(depthStencilView)->DepthStencilView.Get(), Details::Convert2ClearFlag(clearFlags), depth, stencil);
	}

	void RenderDX11::SetViewports(uint32 numViewports, ViewPort* const* viewports) const
	{
		assert(nullptr != viewports);

		const uint32 size = sizeof(viewports) / sizeof(viewports[0]);
		aegis::array<D3D11_VIEWPORT, size> datas;

		for (uint32 i = 0; i < size; i++)
		{
			datas[i] = Details::Convert2ViewPort(*viewports[i]);
		}

		mImmediateContext->RSSetViewports(numViewports, datas.data());
	}

	RasterizerState* RenderDX11::CreateRasterizeState(RasterizeState rasterizeState) const
	{
		ComPtr<ID3D11RasterizerState> rasterizerState;

		D3D11_RASTERIZER_DESC desc{};
		desc = Details::Convert2RasterizeState(rasterizeState);

		HRESULT hr = mD3DDevice->CreateRasterizerState(&desc, &rasterizerState);
		if (FAILED(hr))
			FAILDE_ASSERT;

		RasterizerStateDX11* buf = aegis::allocatorWrapper::allocate<RasterizerStateDX11>();
		buf->RasterizerState = rasterizerState;

		return buf;
	}

	void RenderDX11::SetRasterizeState(RasterizerState* rasterizerState) const
	{
		assert(nullptr != rasterizerState);

		mImmediateContext->RSSetState(static_cast<RasterizerStateDX11*>(rasterizerState)->RasterizerState.Get());
	}

	Buffer* RenderDX11::CreateBuffer(const BufferDesc& desc, const SubresourceData initData)
	{
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.Usage = static_cast<D3D11_USAGE>(desc.Usage);
		bufferDesc.ByteWidth = desc.ByteWidth;
		bufferDesc.BindFlags = Details::Convert2BindFlag(desc.BindFlags);
		bufferDesc.CPUAccessFlags = Details::Convert2CpuAccessFlag(desc.CPUAccessFlags);
		bufferDesc.MiscFlags = desc.MiscFlags;
		bufferDesc.StructureByteStride = desc.StructureByteStride;

		ComPtr<ID3D11Buffer> buffer;

		if (initData.pSysMem != nullptr)
		{
			D3D11_SUBRESOURCE_DATA subData{};
			subData.pSysMem = initData.pSysMem;
			subData.SysMemPitch = initData.SysMemPitch;
			subData.SysMemSlicePitch = initData.SysMemSlicePitch;

			HRESULT hr = mD3DDevice->CreateBuffer(&bufferDesc, &subData, &buffer);
			if (FAILED(hr))
				FAILDE_ASSERT;
		}
		else
		{
			HRESULT hr = mD3DDevice->CreateBuffer(&bufferDesc, nullptr, &buffer);
			if (FAILED(hr))
				FAILDE_ASSERT;
		}

		BufferDX11* buf = aegis::allocatorWrapper::allocate<BufferDX11>();
		buf->Buffer = buffer;

		return buf;
	}

	Texture1D* RenderDX11::CreateTexture1D(Texture1DDesc desc, SubresourceData* initialData) const
	{
		return nullptr;
	}

	Texture2D* RenderDX11::CreateTexture2D(Texture2DDesc desc, SubresourceData* initialData) const
	{
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = desc.Width;
		textureDesc.Height = desc.Height;
		textureDesc.MipLevels = desc.MipLevels;
		textureDesc.ArraySize = desc.ArraySize;
		textureDesc.Format = Details::Convert2Format(desc.Format);
		textureDesc.SampleDesc = Details::Convert2SampleDesc(desc.SampleDesc);
		textureDesc.Usage = static_cast<D3D11_USAGE>(desc.Usage);
		textureDesc.BindFlags = Details::Convert2BindFlag(desc.BindFlags);
		textureDesc.CPUAccessFlags = Details::Convert2CpuAccessFlag(desc.CPUAccessFlags);
		textureDesc.MiscFlags = desc.MiscFlags;

		ComPtr<ID3D11Texture2D> texture;

		if (initialData != nullptr)
		{
			D3D11_SUBRESOURCE_DATA data{};
			data.pSysMem = initialData->pSysMem;
			data.SysMemPitch = initialData->SysMemPitch;
			data.SysMemSlicePitch = initialData->SysMemSlicePitch;

			HRESULT hr = mD3DDevice->CreateTexture2D(&textureDesc, &data, &texture);
			if (FAILED(hr))
				FAILDE_ASSERT;
		}
		else
		{
			HRESULT hr = mD3DDevice->CreateTexture2D(&textureDesc, nullptr, &texture);
			if (FAILED(hr))
				FAILDE_ASSERT;
		}

		Texture2DDX11* buf = aegis::allocatorWrapper::allocate<Texture2DDX11>();
		buf->Texture = texture;

		return buf;
	}

	Texture3D* RenderDX11::CreateTexture3D(Texture3DDesc desc, SubresourceData* initialData) const
	{
		D3D11_TEXTURE3D_DESC textureDesc{};
		textureDesc.Width = desc.Width;
		textureDesc.Height = desc.Height;
		textureDesc.Depth = desc.Depth;
		textureDesc.MipLevels = desc.MipLevels;
		textureDesc.Format = Details::Convert2Format(desc.Format);
		textureDesc.Usage = static_cast<D3D11_USAGE>(desc.Usage);
		textureDesc.BindFlags = Details::Convert2BindFlag(desc.BindFlags);
		textureDesc.CPUAccessFlags = Details::Convert2CpuAccessFlag(desc.CPUAccessFlags);
		textureDesc.MiscFlags = desc.MiscFlags;

		ComPtr<ID3D11Texture3D> texture;

		if (initialData != nullptr)
		{
			D3D11_SUBRESOURCE_DATA data{};
			data.pSysMem = initialData->pSysMem;
			data.SysMemPitch = initialData->SysMemPitch;
			data.SysMemSlicePitch = initialData->SysMemSlicePitch;

			HRESULT hr = mD3DDevice->CreateTexture3D(&textureDesc, &data, &texture);
			if (FAILED(hr))
				FAILDE_ASSERT;
		}
		else
		{
			HRESULT hr = mD3DDevice->CreateTexture3D(&textureDesc, nullptr, &texture);
			if (FAILED(hr))
				FAILDE_ASSERT;
		}

		Texture3DDX11* buf = aegis::allocatorWrapper::allocate<Texture3DDX11>();
		buf->Texture = texture;

		return buf;
	}

	ShaderResourceView* RenderDX11::CreateShaderResourceView(Texture* texture, ShaderResourceViewDesc srvDesc) const
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		Details::Convert2ShaderResourceViewDesc(desc, srvDesc);

		HRESULT hr = S_OK;
		ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		Texture1DDX11* tex1d = nullptr;
		Texture2DDX11* tex2d = nullptr;
		Texture3DDX11* tex3d = nullptr;

		switch (texture->Type)
		{
			case TextureType::TEX1D:
				tex1d = static_cast<Texture1DDX11*>(texture);
				hr = mD3DDevice->CreateShaderResourceView(tex1d->Texture.Get(), &desc, &shaderResourceView);
				
				break;
			case TextureType::TEX2D:
				tex2d = static_cast<Texture2DDX11*>(texture);
				hr = mD3DDevice->CreateShaderResourceView(tex2d->Texture.Get(), &desc, &shaderResourceView);
				break;
			case TextureType::TEX3D:
				tex3d = static_cast<Texture3DDX11*>(texture);
				hr = mD3DDevice->CreateShaderResourceView(tex3d->Texture.Get(), &desc, &shaderResourceView);
				break;
		}

		if (FAILED(hr))
			FAILDE_ASSERT;

		ShaderResourceViewDX11* buf = aegis::allocatorWrapper::allocate<ShaderResourceViewDX11>();
		buf->ShaderResourceView = shaderResourceView;

		return buf;
	}

	UnorderedAccessView* RenderDX11::CreateUnorderedAccessView(Texture* texture, UnorderedAccessViewDesc* unorderedAccessViewDesc) const
	{
		ComPtr<ID3D11UnorderedAccessView> unorderedAccessView;
		Texture1DDX11* tex1d = nullptr;
		Texture2DDX11* tex2d = nullptr;
		Texture3DDX11* tex3d = nullptr;

		HRESULT hr = S_OK;

		if (unorderedAccessViewDesc != nullptr)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc{};
			Details::Convert2UnorderedAccessViewDesc(desc, *unorderedAccessViewDesc);

			switch (texture->Type)
			{
			case TextureType::TEX1D:
				tex1d = static_cast<Texture1DDX11*>(texture);
				hr = mD3DDevice->CreateUnorderedAccessView(tex1d->Texture.Get(), &desc, &unorderedAccessView);

				break;
			case TextureType::TEX2D:
				tex2d = static_cast<Texture2DDX11*>(texture);
				hr = mD3DDevice->CreateUnorderedAccessView(tex2d->Texture.Get(), &desc, &unorderedAccessView);
				break;
			case TextureType::TEX3D:
				tex3d = static_cast<Texture3DDX11*>(texture);
				hr = mD3DDevice->CreateUnorderedAccessView(tex3d->Texture.Get(), &desc, &unorderedAccessView);
				break;
			}

			if (FAILED(hr))
				FAILDE_ASSERT;
		}
		else
		{
			switch (texture->Type)
			{
			case TextureType::TEX1D:
				tex1d = static_cast<Texture1DDX11*>(texture);
				hr = mD3DDevice->CreateUnorderedAccessView(tex1d->Texture.Get(), nullptr, &unorderedAccessView);

				break;
			case TextureType::TEX2D:
				tex2d = static_cast<Texture2DDX11*>(texture);
				hr = mD3DDevice->CreateUnorderedAccessView(tex2d->Texture.Get(), nullptr, &unorderedAccessView);
				break;
			case TextureType::TEX3D:
				tex3d = static_cast<Texture3DDX11*>(texture);
				hr = mD3DDevice->CreateUnorderedAccessView(tex3d->Texture.Get(), nullptr, &unorderedAccessView);
				break;

				if (FAILED(hr))
					FAILDE_ASSERT;
			}
		}

		UnorderedAccessViewDX11* buf = aegis::allocatorWrapper::allocate<UnorderedAccessViewDX11>();
		buf->UnorderedAccessView = unorderedAccessView;

		return buf;
	}

	void RenderDX11::ClearUnorderedAccessView(UnorderedAccessView* unorderedAccessView, float32 clearValues[4]) const
	{
		if (unorderedAccessView != nullptr)
		{
			mImmediateContext->ClearUnorderedAccessViewFloat(static_cast<UnorderedAccessViewDX11*>(unorderedAccessView)->UnorderedAccessView.Get(), clearValues);
		}
	}

	void RenderDX11::ClearUnorderedAccessView(UnorderedAccessView* unorderedAccessView, uint32 clearValues[4]) const
	{
		if (unorderedAccessView != nullptr)
		{
			mImmediateContext->ClearUnorderedAccessViewUint(static_cast<UnorderedAccessViewDX11*>(unorderedAccessView)->UnorderedAccessView.Get(), clearValues);
		}
	}

	void RenderDX11::CSSetUnorderedAccessViews(uint32 startSlot, uint32 numBuffers, UnorderedAccessView* const* unorderedAccessViews) const
	{
		if (unorderedAccessViews != nullptr)
		{
			const uint32 size = sizeof(unorderedAccessViews) / sizeof(unorderedAccessViews[0]);
			aegis::array<ID3D11UnorderedAccessView*, size> datas;

			for (uint32 i = 0; i < size; i++)
			{
				datas[i] = static_cast<UnorderedAccessViewDX11*>(unorderedAccessViews[i])->UnorderedAccessView.Get();
			}

			mImmediateContext->CSSetUnorderedAccessViews(startSlot, numBuffers, datas.data(), nullptr);
		}
		else
		{
			ID3D11UnorderedAccessView* uav = nullptr;
			mImmediateContext->CSSetUnorderedAccessViews(startSlot, numBuffers, &uav, nullptr);
		}
	}

	void RenderDX11::SetVertexBuffers(Buffer* vertexBuffer, uint32 stride)
	{
		const uint32 offset = 0;
		ID3D11Buffer* const vb[1] = { static_cast<BufferDX11*>(vertexBuffer)->Buffer.Get()};
		mImmediateContext->IASetVertexBuffers(0, 1, vb, &stride, &offset);
	}

	void RenderDX11::SetVertexBuffers(Buffer* indexBuffer, Buffer* instancingBuffer, UINT size)
	{
		uint32 stride[2] = { size, sizeof(XMMATRIX) };
		uint32 offset[2] = { 0, 0 };
		ID3D11Buffer* vb[2] = { static_cast<BufferDX11*>(indexBuffer)->Buffer.Get(), static_cast<BufferDX11*>(instancingBuffer)->Buffer.Get() };
		mImmediateContext->IASetVertexBuffers(0, 2, vb, stride, offset);
	}

	void RenderDX11::SetIndexBuffer(Buffer* indexBuffer, Format format) const
	{
		mImmediateContext->IASetIndexBuffer(static_cast<BufferDX11*>(indexBuffer)->Buffer.Get(), Details::Convert2Format(format), 0);
	}

	PixelShader* RenderDX11::CreatePixelShader(void* shaderData, uint32 size) const
	{
		ComPtr<ID3D11PixelShader> shader;

		HRESULT hr = mD3DDevice->CreatePixelShader(shaderData, size, nullptr, &shader);
		if (FAILED(hr))
			FAILDE_ASSERT;

		PixelShaderDX11* buf = aegis::allocatorWrapper::allocate<PixelShaderDX11>();
		buf->Shader = shader;

		return buf;
	}

	VertexShader* RenderDX11::CreateVertexShader(void* shaderData, uint32 size) const
	{
		ComPtr<ID3D11VertexShader> shader;

		HRESULT hr = mD3DDevice->CreateVertexShader(shaderData, size, nullptr, &shader);
		if (FAILED(hr))
			FAILDE_ASSERT;

		VertexShaderDX11* buf = aegis::allocatorWrapper::allocate<VertexShaderDX11>();
		buf->Shader = shader;

		return buf;
	}

	ComputeShader* RenderDX11::CreateComputeShader(void* shaderData, uint32 size) const
	{
		ComPtr<ID3D11ComputeShader> shader;

		HRESULT hr = mD3DDevice->CreateComputeShader(shaderData, size, nullptr, &shader);
		if (FAILED(hr))
			FAILDE_ASSERT;

		ComputeShaderDX11* buf = aegis::allocatorWrapper::allocate<ComputeShaderDX11>();
		buf->Shader = shader;

		return buf;
	}

	void RenderDX11::Draw(uint32 indexCount, uint32 startVertexLocation)
	{
		mImmediateContext->Draw(indexCount, startVertexLocation);
	}

	void RenderDX11::DrawIndexed(uint32 indexCount, uint32 startIndexLocation, int32 baseVertexLocation)
	{
		mImmediateContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
	}

	void RenderDX11::DrawIndexedInstanced(int32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32 baseVertexLocation, uint32 startInstanceLocation)
	{
		mImmediateContext->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
	}

	void RenderDX11::Dispatch(uint32 threadGroupCountX, uint32 threadGroupCountY, uint32 threadGroupCountZ) const
	{
		mImmediateContext->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
	}

	void RenderDX11::CSSetShader(ComputeShader* pShader) const
	{
		if (pShader != nullptr)
		{
			mImmediateContext->CSSetShader(static_cast<ComputeShaderDX11*>(pShader)->Shader.Get(), nullptr, 0);
		}
		else
		{
			mImmediateContext->CSSetShader(nullptr, nullptr, 0);
		}
	}

	void RenderDX11::VSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers)
	{
		if (pBuffers != nullptr)
		{
			const uint32 size = sizeof(pBuffers) / sizeof(pBuffers[0]);
			aegis::array<ID3D11Buffer*, size> datas;

			for (uint32 i = 0; i < size; i++)
			{
				datas[i] = static_cast<BufferDX11*>(pBuffers[i])->Buffer.Get();
			}

			mImmediateContext->VSSetConstantBuffers(startSlot, numBuffers, datas.data());
		}
		else
		{
			ID3D11Buffer* buffer = nullptr;
			mImmediateContext->VSSetConstantBuffers(startSlot, numBuffers, &buffer);
		}
	}

	void RenderDX11::PSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers)
	{
		assert(nullptr != pBuffers);

		if (pBuffers != nullptr)
		{
			const uint32 size = sizeof(pBuffers) / sizeof(pBuffers[0]);
			aegis::array<ID3D11Buffer*, size> datas;

			for (uint32 i = 0; i < size; i++)
			{
				datas[i] = static_cast<BufferDX11*>(pBuffers[i])->Buffer.Get();
			}

			mImmediateContext->PSSetConstantBuffers(startSlot, numBuffers, datas.data());
		}
		else
		{
			ID3D11Buffer* buffer = nullptr;
			mImmediateContext->PSSetConstantBuffers(startSlot, numBuffers, &buffer);
		}
	}

	void RenderDX11::CSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers)
	{
		assert(nullptr != pBuffers);

		if (pBuffers[0] != nullptr)
		{
			const uint32 size = sizeof(pBuffers) / sizeof(pBuffers[0]);
			aegis::array<ID3D11Buffer*, size> datas;

			for (uint32 i = 0; i < size; i++)
			{
				datas[i] = static_cast<BufferDX11*>(pBuffers[i])->Buffer.Get();
			}

			mImmediateContext->CSSetConstantBuffers(startSlot, numBuffers, datas.data());
		}
		else
		{
			ID3D11Buffer* buffer = nullptr;
			mImmediateContext->CSSetConstantBuffers(startSlot, numBuffers, &buffer);
		}
	}

	void RenderDX11::PSSetShaderResources(uint32 startSlot, uint32 numBuffers, aegis::ShaderResourceView* const* shaderResourceViews) const
	{
		if (shaderResourceViews != nullptr)
		{
			const uint32 size = sizeof(shaderResourceViews) / sizeof(shaderResourceViews[0]);
			aegis::array<ID3D11ShaderResourceView*, size> datas;

			for (uint32 i = 0; i < size; i++)
			{
				datas[i] = static_cast<ShaderResourceViewDX11*>(shaderResourceViews[i])->ShaderResourceView.Get();
			}

			mImmediateContext->PSSetShaderResources(startSlot, numBuffers, datas.data());
		}
		else
		{
			ID3D11ShaderResourceView* srv = nullptr;
			mImmediateContext->PSSetShaderResources(startSlot, numBuffers, &srv);
		}
	}

	void RenderDX11::CSSetShaderResources(uint32 startSlot, uint32 numBuffers, aegis::ShaderResourceView* const* shaderResourceViews) const
	{
		if (shaderResourceViews != nullptr)
		{
			const uint32 size = sizeof(shaderResourceViews) / sizeof(shaderResourceViews[0]);
			aegis::array<ID3D11ShaderResourceView*, size> datas;

			for (uint32 i = 0; i < size; i++)
			{
				datas[i] = static_cast<ShaderResourceViewDX11*>(shaderResourceViews[i])->ShaderResourceView.Get();
			}

			mImmediateContext->CSSetShaderResources(startSlot, numBuffers, datas.data());
		}
		else
		{
			ID3D11ShaderResourceView* srv = nullptr;
			mImmediateContext->CSSetShaderResources(startSlot, numBuffers, &srv);
		}
	}

	SamplerState* RenderDX11::CreateSampler(const SamplerDesc desc) const
	{
		D3D11_SAMPLER_DESC samDesc{};
		samDesc.Filter = Details::Convert2Filter(desc.Filter);
		samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = Details::Convert2AddressMode(desc.Address);
		samDesc.MaxAnisotropy = desc.MaxAnisotropy;
		samDesc.ComparisonFunc = Details::Convert2ComparisonFunc(desc.ComparisonFunc);
		samDesc.MaxLOD = aegis::Math::Float32Max;

		ComPtr<ID3D11SamplerState> samplerState;

		HRESULT hr = mD3DDevice->CreateSamplerState(&samDesc, &samplerState);
		if (FAILED(hr))
			FAILDE_ASSERT;

		SamplerStateDX11* buf = aegis::allocatorWrapper::allocate<SamplerStateDX11>();
		buf->Sampler = samplerState;

		return buf;
	}

	void RenderDX11::PSSetSamplers(UINT startSlot, UINT numSamplers, SamplerState* const* ppSamplers)
	{
		assert(nullptr != ppSamplers);

		const uint32 size = sizeof(ppSamplers) / sizeof(ppSamplers[0]);
		aegis::array<ID3D11SamplerState*, size> datas;

		for (uint32 i = 0; i < size; i++)
		{
			datas[i] = static_cast<SamplerStateDX11*>(ppSamplers[i])->Sampler.Get();
		}

		mImmediateContext->PSSetSamplers(startSlot, numSamplers, datas.data());
	}

	void RenderDX11::CSSetSamplers(UINT startSlot, UINT numSamplers, SamplerState* const* ppSamplers)
	{
		assert(nullptr != ppSamplers);
		const uint32 size = sizeof(ppSamplers) / sizeof(ppSamplers[0]);
		aegis::array<ID3D11SamplerState*, size> datas;

		for (uint32 i = 0; i < size; i++)
		{
			datas[i] = static_cast<SamplerStateDX11*>(ppSamplers[i])->Sampler.Get();
		}

		mImmediateContext->CSSetSamplers(startSlot, numSamplers, datas.data());
	}

	void RenderDX11::SetPrimitiveTopology(PrimitiveTopology primitiveTopology)
	{
		mImmediateContext->IASetPrimitiveTopology(Details::Convert2Topology(primitiveTopology));
	}

	void RenderDX11::UpdateSubresource(Buffer* buffer, void* pData)
	{
		assert(nullptr != pData);

		mImmediateContext->UpdateSubresource(static_cast<BufferDX11*>(buffer)->Buffer.Get(), 0, nullptr, pData, 0, 0);
	}

	void RenderDX11::Map(Buffer* buffer, void* pData, uint32 dataSize)
	{
		assert(nullptr != pData);

		D3D11_MAPPED_SUBRESOURCE msr;

		switch (buffer->mResourceType)
		{
			case GpuResourceType::Buffer:
				mImmediateContext->Map(static_cast<BufferDX11*>(buffer)->Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				break;
			case GpuResourceType::Texture1D:
				mImmediateContext->Map(static_cast<Texture1DDX11*>(buffer)->Texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				break;
			case GpuResourceType::Texture2D:
				mImmediateContext->Map(static_cast<Texture2DDX11*>(buffer)->Texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				break;
			case GpuResourceType::Texture3D:
				mImmediateContext->Map(static_cast<Texture3DDX11*>(buffer)->Texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				break;

			default:
				assert(false, "GpuResourceType is Unknown Type");
				break;
		}

		std::memcpy(msr.pData, pData, dataSize);
	}

	MappedSubresource RenderDX11::Map(Buffer* buffer)
	{
		D3D11_MAPPED_SUBRESOURCE msr{};
		switch (buffer->mResourceType)
		{
			case GpuResourceType::Buffer:
				mImmediateContext->Map(static_cast<BufferDX11*>(buffer)->Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				break;
			case GpuResourceType::Texture1D:
				mImmediateContext->Map(static_cast<Texture1DDX11*>(buffer)->Texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				break;
			case GpuResourceType::Texture2D:
				mImmediateContext->Map(static_cast<Texture2DDX11*>(buffer)->Texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				break;
			case GpuResourceType::Texture3D:
				mImmediateContext->Map(static_cast<Texture3DDX11*>(buffer)->Texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				break;

			default:
				assert(false, "GpuResourceType is Unknown Type");
				break;
		}

		MappedSubresource sub{};
		sub.pData = msr.pData;
		sub.RowPitch = msr.RowPitch;
		sub.DepthPitch = msr.DepthPitch;

		return sub;
	}

	void RenderDX11::Unmap(Buffer* buffer)
	{
		switch (buffer->mResourceType)
		{
			case GpuResourceType::Buffer:
				mImmediateContext->Unmap(static_cast<BufferDX11*>(buffer)->Buffer.Get(), 0);
				break;
			case GpuResourceType::Texture1D:
				mImmediateContext->Unmap(static_cast<Texture1DDX11*>(buffer)->Texture.Get(), 0);
				break;
			case GpuResourceType::Texture2D:
				mImmediateContext->Unmap(static_cast<Texture2DDX11*>(buffer)->Texture.Get(), 0);
				break;
			case GpuResourceType::Texture3D:
				mImmediateContext->Unmap(static_cast<Texture3DDX11*>(buffer)->Texture.Get(), 0);
				break;

			default:
				assert(false, "GpuResourceType is Unknown Type");
				break;
		}
	}

	void RenderDX11::Set_Shader(const aegis::SHADER_INDEX_V v_index, const aegis::SHADER_INDEX_P p_index)
	{
		mImmediateContext->VSSetShader(mVertexShader[v_index].Get(), nullptr, 0);

		mImmediateContext->PSSetShader(mPixelShader[p_index].Get(), nullptr, 0);
	}

	void RenderDX11::Set_InputLayout(const INPUTLAYOUT InputLayout)
	{
		// 入力レイアウト設定
		switch ((int)InputLayout)
		{
			case (int)INPUTLAYOUT::DEFAULT:
				mImmediateContext->IASetInputLayout(mVertexLayout[(int)INPUTLAYOUT::DEFAULT].Get());
				break;

			case (int)INPUTLAYOUT::ANIMATION:
				mImmediateContext->IASetInputLayout(mVertexLayout[(int)INPUTLAYOUT::ANIMATION].Get());
				break;

			case (int)INPUTLAYOUT::INSTANCING:
				mImmediateContext->IASetInputLayout(mVertexLayout[(int)INPUTLAYOUT::INSTANCING].Get());
				break;
		}
	}

	void RenderDX11::SetPassRendring()
	{
		float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

		// デフォルトのレンダーターゲットビューに切り替え
		// デフォルトのレンダーターゲットビューに切り替え
		//auto render_target = RenderTargetView[0].Get();
		//mImmediateContext->OMSetRenderTargets(1, &render_target, mDepthStencilView.Get());
		//mImmediateContext->ClearRenderTargetView(render_target, clearColor);
		//
		////mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
		////mImmediateContext->ClearRenderTargetView(mRenderTargetView, clearColor);
		//mImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		{
			auto render_target = mRenderTargetView_16bit.Get();

			mImmediateContext->OMSetRenderTargets(1, &render_target, mDepthStencilView.Get());//
			mImmediateContext->ClearRenderTargetView(mRenderTargetView_16bit.Get(), clearColor);//
			mImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		{
			// ビューポート設定
			D3D11_VIEWPORT dxViewport;
			dxViewport.Width = (float)SCREEN_WIDTH;
			dxViewport.Height = (float)SCREEN_HEIGHT;
			dxViewport.MinDepth = 0.0f;
			dxViewport.MaxDepth = 1.0f;
			dxViewport.TopLeftX = 0.0f;
			dxViewport.TopLeftY = 0.0f;

			mImmediateContext->RSSetViewports(1, &dxViewport);
		}

		Set_Shader();
		Set_RasterizerState();

		SamplerState* sampler = CManager::Get_Instance()->Get_ShadowMap()->getSampler();

		ID3D11SamplerState* pSS[] = { mSamplerState.Get(), static_cast<SamplerStateDX11*>(sampler)->Sampler.Get()};
		mImmediateContext->PSSetSamplers(0, 2, pSS);

		SetDepthEnable(true);
	}

	void RenderDX11::SetPassGeometry()
	{
		float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

		ID3D11RenderTargetView* rtv[3] = { mRenderTargetViews[0].Get(), mRenderTargetViews[1].Get(), mRenderTargetViews[2].Get() };

		// デフォルトのレンダーターゲットビューに切り替え
		mImmediateContext->OMSetRenderTargets(3, rtv, mDepthStencilView.Get());
		mImmediateContext->ClearRenderTargetView(rtv[0], clearColor);
		mImmediateContext->ClearRenderTargetView(rtv[1], clearColor);
		mImmediateContext->ClearRenderTargetView(rtv[2], clearColor);
		mImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		{
			// ビューポート設定
			D3D11_VIEWPORT dxViewport;
			dxViewport.Width = (float)SCREEN_WIDTH;
			dxViewport.Height = (float)SCREEN_HEIGHT;
			dxViewport.MinDepth = 0.0f;
			dxViewport.MaxDepth = 1.0f;
			dxViewport.TopLeftX = 0.0f;
			dxViewport.TopLeftY = 0.0f;

			D3D11_VIEWPORT viewport[3] = { dxViewport, dxViewport, dxViewport };

			mImmediateContext->RSSetViewports(3, viewport);
		}

		Set_Shader();
		Set_RasterizerState();
	}

	void RenderDX11::End_Draw()
	{
		std::call_once(mOnceFlag, [this]
			{
				sprite = std::make_unique<SPRITE>();

				sprite.get()->SetPosition(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
				sprite.get()->SetSize(Vector4(SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f));

				ShaderResourceViewDX11* buf = aegis::allocatorWrapper::allocate<ShaderResourceViewDX11>();
				buf->ShaderResourceView = mShaderResourceView_16bit;

				sprite.get()->Set(buf);

				sprite.get()->flag = false;

				sprite.get()->Init();
			}
		);

		float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		mImmediateContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
		mImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), ClearColor);//
		mImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	
		sprite.get()->Draw();
	}

	HRESULT RenderDX11::CreateTextFormat(const TEXT_FOMAT& fomat)
	{
		SAFE_RELEASE(mDwriteTextFormat)

			std::wstring font_name = stringTowstring(fomat.FontName);

		// テキストフォーマットの作成
		HRESULT hr = mDwriteFactory->CreateTextFormat(
			font_name.c_str(),
			nullptr,
			fomat.Weight,
			fomat.Style,
			fomat.Stretch,
			fomat.FontSize,
			L"",
			&mDwriteTextFormat
		);

		return hr;
	}

	HRESULT RenderDX11::CreateTextLayout(const TEXT_LAYOUT& layout)
	{
		SAFE_RELEASE(mTextLayout)

		// テキストレイアウトを作成
		std::wstring drawText;

		drawText = stringTowstring(layout.Text);

		HRESULT hr = mDwriteFactory->CreateTextLayout(
			drawText.c_str(),
			drawText.size(),
			mDwriteTextFormat.Get(),
			layout.Width,
			layout.Height,
			&mTextLayout
		);

		return hr;
	}
}
