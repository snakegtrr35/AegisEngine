#pragma once

#ifndef  _INCLEDE_RENDER_RENDERERTYPEDEFINE_H_
#define _INCLEDE_RENDER_RENDERERTYPEDEFINE_H_

namespace aegis
{
    enum class RenderApiType
    {
        Unknown,
        DirectX11,
        DirectX12,
        Vulkan,
        Metal,
    };

    //enum class GpuResourceType
    //{
    //    RenderTargetView,
    //
    //};

    enum class ShaderType
    {
        Pixel,
        Vertex,
        Compute,
    };

    enum class FillMode
    {
        Wireframe,
        Solid,
    };

    enum class CullMode
    {
        None,
        Front,
        Back,
    };

    enum class ClearFlag
    {
        Depth,
        Stencil,
    };

    enum class PrimitiveTopology
    {
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
    };

    enum class Blend
    {
        BlendZero,
        BlendOne,
        BlendSrcColor,
        BlendInvSrcColor,
        BlendSrcAlpha,
        BlendInvSrcAlpha,
        BlendDestAlpha,
        BlendInvDestAlpha,
        BlendDestColor,
        BlendInvDestColor,
        BlendSrcAlphaSAT,
        BlendBlendFactor,
        BlendInvBlendFactor,
        BlendSrc1Color,
        BlendInvSrc1Color,
        BlendSrc1Alpha,
        BlendInvSrc1Alpha,
    };

    enum class BlendOp
    {
        BlendOpAdd,
        BlendOpSubtract,
        BlendOpRevSubtract,
        BlendOpMin,
        BlendOpMax,
    };

    struct RenderTargetBlendDesc
    {
        bool BlendEnable;
        Blend SrcBlend;
        Blend DestBlend;
        BlendOp BlendOp;
        Blend SrcBlendAlpha;
        Blend DestBlendAlpha;
        aegis::BlendOp BlendOpAlpha;
        uint8 RenderTargetWriteMask;
    };

	struct BlendDesc
	{
		bool AlphaToCoverageEnable;
		bool IndependentBlendEnable;
        RenderTargetBlendDesc RenderTarget[8];
	};

    enum class Usage
    {
        Default,
        Immutable,
        Dynamic,
        Staging,
    };

    enum class BindFlag
    {
        Vertexbuffer    = 1 << 0,
        Indexbuffer     = 1 << 1,
        Constantbuffer  = 1 << 2,
        ShaderResource  = 1 << 3,
        StreamOutput    = 1 << 4,
        RenderTarget    = 1 << 5,
        DepthStencil    = 1 << 6,
        UnorderedAccess = 1 << 7,
        Decoder         = 1 << 8,
        VideoEncoder    = 1 << 9,
    };

    inline BindFlag begin(BindFlag) { return BindFlag::Vertexbuffer; }
    inline BindFlag end(BindFlag) { return BindFlag::VideoEncoder; }
    inline BindFlag operator*(BindFlag bindFlag) { return bindFlag; }
    inline BindFlag operator++(BindFlag& bindFlag) {
        return bindFlag = BindFlag(std::underlying_type<BindFlag>::type(bindFlag) + 1);
    }

    enum class CpuAccessFlag
    {
        None,
        Write,
        Read,
    };

    struct BufferDesc
    {
        uint32 ByteWidth;
        Usage Usage;
        BindFlag BindFlags;
        CpuAccessFlag CPUAccessFlags;
        uint32 MiscFlags;
        uint32 StructureByteStride;
    };

    struct SubresourceData
    {
        const void* pSysMem = nullptr;
        uint32 SysMemPitch;
        uint32 SysMemSlicePitch;
    };

    struct ViewPort
    {
        float32 TopLeftX;
        float32 TopLeftY;
        float32 Width;
        float32 Height;
        float32 MinDepth;
        float32 MaxDepth;
    };

    struct RasterizeState
    {
        FillMode FillMode;
        CullMode CullMode;
        bool FrontCounterClockwise;
        int32 DepthBias;
        float32 DepthBiasClamp;
        float32 SlopeScaledDepthBias;
        bool DepthClipEnable;
        bool ScissorEnable;
        bool MultisampleEnable;
        bool AntialiasedLineEnable;
    };

    enum class Filter
    {
        Point,
        Linear,
        Anisotropic,
    };

    enum class AddressMode
    {
        Wrap,
        Mirror,
        Clamp,
        Border,
        MirrorOnce,
    };

    enum class ComparisonFunc
    {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always,
    };

    struct SamplerDesc
    {
        Filter Filter;
        //AddressMode AddressU;
        //AddressMode AddressV;
        //AddressMode AddressW;
        AddressMode Address;
        float32 MipLODBias;
        uint32 MaxAnisotropy;
        ComparisonFunc ComparisonFunc;
        float32 BorderColor[4];
        float32 MinLOD;
        float32 MaxLOD;
    };
    
    struct MappedSubresource
    {
        void* pData = nullptr;
        uint32 RowPitch;
        uint32 DepthPitch;
    };

    struct SampleDesc
    {
        uint32 Count;
        uint32 Quality;
    };

    enum class Format
    {
        Unknown,
        RGBA32Typeless,
        RGBA32Float,
        RGBA32Uint,
        RGBA32Int,
        RGB32Typeless,
        RGB32Float,
        RGB32Uint,
        RGB32Int,
        RGBA16Typeless,
        RGBA16Float,
        RGBA16Unorm,
        RGBA16Uint,
        RGBA16Snorm,
        RGBA16Int,
        RG32Typeless,
        RG32Float,
        RG32Uint,
        RG32Int,
        R32G8X24Typeless,
        D32FloatS8X24Uint,
        R32FloatX8X24Typeless,
        X32TypelessG8X24Uint,
        RGB10A2Typeless,
        RGB10A2Unorm,
        RGB10A2Uint,
        R11G11B10_Float,
        RGBA8Typeless,
        RGBA8Unorm ,
        RGBA8UnormSrgb,
        RGBA8Uint,
        RGBA8Snorm,
        RGBA8Int,
        RG16Typeless,
        RG16Float,
        RG16Unorm,
        RG16Uint,
        RG16Snorm,
        RG16Int,
        R32Typeless,
        D32Float,
        R32Float,
        R32Uint,
        R32Int,
        RG8Typeless,
        RG8Unorm,
        RG8Uint,
        RG8Snorm,
        RG8Int,
        R16Typeless,
        R16Float,
        D16Unorm,
        R16Unorm,
        R16Uint,
        R16Snorm,
        R16Int,
        R8Typeless,
        R8Unorm,
        R8Uint,
        R8Snorm,
        R8Int,
        A8Unorm,
    };

    struct Texture1DDesc
    {
        uint32 Width;
        uint32 MipLevels;
        uint32 ArraySize;
        Format Format;
        Usage Usage;
        BindFlag BindFlags;
        CpuAccessFlag CPUAccessFlags;
        uint32 MiscFlags;
    };

    struct Texture2DDesc
    {
        uint32 Width;
        uint32 Height;
        uint32 MipLevels;
        uint32 ArraySize;
        Format Format;
        SampleDesc SampleDesc;
        Usage Usage;
        BindFlag BindFlags;
        CpuAccessFlag CPUAccessFlags;
        uint32 MiscFlags;
    };

    struct Texture3DDesc
    {
        uint32 Width;
        uint32 Height;
        uint32 Depth;
        uint32 MipLevels;
        Format Format;
        Usage Usage;
        BindFlag BindFlags;
        CpuAccessFlag CPUAccessFlags;
        uint32 MiscFlags;
    } ;

    enum class DsvDimension
    {
        Unknown,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        Texture2DMS,
        Texture2DMSArray,
    };

    enum class SrvDimension
    {
        Unknown,
        Buffer,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        Texture2DMS,
        Texture2DMSArray,
        Texture3D,
        TextureCube,
        TextureCubeArray,
        BufferEx,
    };

    enum class UavDimension
    {
        Unknown,
        Buffer,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        Texture3D,
    };

    struct BufferSrv
    {
        union
        {
            uint32 FirstElement;
            uint32 ElementOffset;
        };
        union
        {
            uint32 NumElements;
            uint32 ElementWidth;
        };
    };

    struct Tex1DSrv
    {
        uint32 MostDetailedMip;
        uint32 MipLevels;
    };

    struct Tex1DSrvDsv
    {
        uint32 MipSlice;
    };

    struct Tex1DArraySrv
    {
        uint32 MostDetailedMip;
        uint32 MipLevels;
        uint32 FirstArraySlice;
        uint32 ArraySize;
    };

    struct Tex1DArraySrvDsv
    {
        uint32 MipSlice;
        uint32 FirstArraySlice;
        uint32 ArraySize;
    };

    struct Tex2DSrv
    {
        uint32 MostDetailedMip;
        uint32 MipLevels;
    };

    struct Tex2DSrvDsv
    {
        uint32 MipSlice;
    };

    struct Tex2DArraySrv
    {
        uint32 MostDetailedMip;
        uint32 MipLevels;
        uint32 FirstArraySlice;
        uint32 ArraySize;
    };

    struct Tex2DArraySrvDsv
    {
        uint32 MipSlice;
        uint32 FirstArraySlice;
        uint32 ArraySize;
    };

    struct Tex2DMSSrv
    {
        uint32 UnusedField_NothingToDefine;
    };

    struct Tex2DMSSrvDsv
    {
        uint32 UnusedField_NothingToDefine;
    };

    struct Tex2DMSArraySrv
    {
        uint32 FirstArraySlice;
        uint32 ArraySize;
    };

    struct Tex2DMSArraySrvDsv
    {
        uint32 FirstArraySlice;
        uint32 ArraySize;
    };

    struct Tex3DSrv
    {
        uint32 MostDetailedMip;
        uint32 MipLevels;
    };

    struct TexCubeSrv
    {
        uint32 MostDetailedMip;
        uint32 MipLevels;
    };

    struct TexCubeArraySrv
    {
        uint32 MostDetailedMip;
        uint32 MipLevels;
        uint32 First2DArrayFace;
        uint32 NumCubes;
    };

    struct BufferExSrv
    {
        uint32 FirstElement;
        uint32 NumElements;
        uint32 Flags;
    };

    struct BufferUav
    {
        uint32 FirstElement;
        uint32 NumElements;
        uint32 Flags;
    };

    struct Tex1DUav
    {
        uint32 MipSlice;
    };

    struct Tex1DArrayUav
    {
        uint32 MipSlice;
        uint32 FirstArraySlice;
        uint32 ArraySize;
    };

    struct Tex2DUav
    {
        uint32 MipSlice;
    };

    struct Tex2DArrayUav
    {
        uint32 MipSlice;
        uint32 FirstArraySlice;
        uint32 ArraySize;
    };

    struct Tex3DUav
    {
        uint32 MipSlice;
        uint32 FirstWSlice;
        uint32 WSize;
    };

    struct DepthStencilViewDesc
    {
        Format Format;
        DsvDimension ViewDimension;
        UINT Flags;
        union
        {
            Tex1DSrvDsv Texture1D;
            Tex1DArraySrvDsv Texture1DArray;
            Tex2DSrvDsv Texture2D;
            Tex2DArraySrvDsv Texture2DArray;
            Tex2DMSSrvDsv Texture2DMS;
            Tex2DMSArraySrvDsv Texture2DMSArray;
        };
    };

    struct ShaderResourceViewDesc
    {
        Format Format;
        SrvDimension ViewDimension;
        union
        {
            BufferSrv Buffer;
            Tex1DSrv Texture1D;
            Tex1DArraySrv Texture1DArray;
            Tex2DSrv Texture2D;
            Tex2DArraySrv Texture2DArray;
            Tex2DMSSrv Texture2DMS;
            Tex2DMSArraySrv Texture2DMSArray;
            Tex3DSrv Texture3D;
            TexCubeSrv TextureCube;
            TexCubeArraySrv TextureCubeArray;
            BufferExSrv BufferEx;
        };
    };

    struct UnorderedAccessViewDesc
    {
        Format Format;
        UavDimension ViewDimension;
        union {
            BufferUav    Buffer;
            Tex1DUav            Texture1D;
            Tex1DArrayUav       Texture1DArray;
            Tex2DUav            Texture2D;
            Tex2DArrayUav       Texture2DArray;
            Tex3DUav            Texture3D;
        };
    };

    enum class GpuResourceType
    {
        Unknown,
        Buffer,
        Texture1D,
        Texture2D,
        Texture3D,
    };

    struct  IGpuResource
    {
        GpuResourceType mResourceType;

        virtual bool IsEmpty() const { return true; }

        virtual void Release() {}

        IGpuResource() : mResourceType(GpuResourceType::Unknown)
        {
        }

        virtual ~IGpuResource()
        {
        }
    };

    struct IRenderTargetView : IGpuResource
    {
    };

    struct RenderTargetView : IRenderTargetView
    {
    };

    struct IDepthStencilView : IGpuResource
    {
    };

    struct DepthStencilView : IDepthStencilView
    {
    };


    struct IRasterizerState : IGpuResource
    {
    };

    struct RasterizerState : IRasterizerState
    {
    };

    struct IShaderResourceView : IGpuResource
    {
    };

    struct ShaderResourceView : IShaderResourceView
    {
    };

    struct IUnorderedAccessView : IGpuResource
    {
    };

    struct UnorderedAccessView : IUnorderedAccessView
    {
    };

    struct ISamplerState : IGpuResource
    {
    };

    struct SamplerState : ISamplerState
    {
    };

    struct IBuffer : IGpuResource
    {
        IBuffer() = default;
    };

    struct Buffer : IBuffer
    {
        Buffer()
        {
            mResourceType = GpuResourceType::Buffer;
        }
    };

    enum class TextureType : uint8
    {
        TEX1D,
        TEX2D,
        TEX3D,
    };

    struct Texture : Buffer
    {
        TextureType Type;
    };

    struct Texture1D : Texture
    {
        Texture1D()
        {
            Type = TextureType::TEX1D;
            mResourceType = GpuResourceType::Texture1D;
        }
    };

    struct Texture2D : Texture
    {
        Texture2D()
        {
            Type = TextureType::TEX2D;
            mResourceType = GpuResourceType::Texture2D;
        }
    };

    struct Texture3D : Texture
    {
        Texture3D()
        {
            Type = TextureType::TEX3D;
            mResourceType = GpuResourceType::Texture3D;
        }
    };

    struct IShader : IGpuResource
    {
    };

    struct Shader : IShader
    {
        ShaderType mShaderType;
    };

    struct PixelShader : Shader
    {

    };

    struct VertexShader : Shader
    {

    };

    struct ComputeShader : Shader
    {

    };
}

#endif // ! _INCLEDE_RENDER_RENDERERTYPEDEFINE_H_
