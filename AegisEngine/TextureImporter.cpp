#include "TextureImporter.h"

#include "RenderDX11.h"

#include "external\DirectXTex\WICTextureLoader.h"
#include "external\DirectXTex\DDSTextureLoader.h"

#include "include/engine/core/AllocatorWrapper.h"

namespace aegis
{
    TextureImporter* TextureImporter::getInstance()
    {
        static std::unique_ptr<TextureImporter> Instance = nullptr;

        if (nullptr == Instance)
        {
            Instance.reset(new TextureImporter());
        }

        return Instance.get();
    }

    TextureImporter::TextureImporter()
    {
    }

    TextureImporter::~TextureImporter()
    {
    }



    ShaderResourceView* TextureImporter::GetShaderResourceView(aegis::wstring fileName) const
    {
        CRenderer* render = CRenderer::getInstance();

        RenderDX11* renderDX11 = nullptr;

        switch (render->GetRenderApiType())
        {
            case RenderApiType::DirectX11:
                renderDX11 = static_cast<RenderDX11*>(render);
                break;
        }

        ComPtr<ID3D11ShaderResourceView> shaderResourceView;

        uint64 pos;
        aegis::wstring extension;

        pos = fileName.find_last_of(L".");
        extension = fileName.substr(pos + 1);

        if (L"dds" == extension)	// dds
        {
            HRESULT hr = CreateDDSTextureFromFile(renderDX11->GetDevice(), renderDX11->GetDeviceContext(), fileName.c_str(), nullptr, &shaderResourceView);
            if (FAILED(hr))
                FAILDE_ASSERT
        }
        else
        {
            HRESULT hr = CreateWICTextureFromFile(renderDX11->GetDevice(), renderDX11->GetDeviceContext(), fileName.c_str(), nullptr, &shaderResourceView);
            if (FAILED(hr))
                FAILDE_ASSERT
        }

        ShaderResourceViewDX11* buf = aegis::allocatorWrapper::allocate<ShaderResourceViewDX11>();

        buf->ShaderResourceView = shaderResourceView;

        return buf;
    }

    ShaderResourceView* TextureImporter::GetShaderResourceView(void* textureDara, uint32 size) const
    {
        CRenderer* render = CRenderer::getInstance();

        RenderDX11* renderDX11 = nullptr;

        switch (render->GetRenderApiType())
        {
        case RenderApiType::DirectX11:
            renderDX11 = static_cast<RenderDX11*>(render);
            break;
        }

        ComPtr<ID3D11ShaderResourceView> shaderResourceView;

        HRESULT hr = CreateWICTextureFromMemory(renderDX11->GetDevice(), renderDX11->GetDeviceContext(), reinterpret_cast<unsigned char*>(textureDara), size, nullptr, &shaderResourceView);
        if (FAILED(hr))
            FAILDE_ASSERT

        ShaderResourceViewDX11* buf = aegis::allocatorWrapper::allocate<ShaderResourceViewDX11>();

        buf->ShaderResourceView = shaderResourceView;

        return buf;
    }
    void TextureImporter::GetSize(aegis::wstring fileName, int32* width, int32* hieght) const
    {
        CRenderer* render = CRenderer::getInstance();

        RenderDX11* renderDX11 = nullptr;

        switch (render->GetRenderApiType())
        {
        case RenderApiType::DirectX11:
            renderDX11 = static_cast<RenderDX11*>(render);
            break;
        }

        uint64 pos;
        aegis::wstring extension;

        pos = fileName.find_last_of(L".");
        extension = fileName.substr(pos + 1);

        DirectX::GetSize(fileName.c_str(), width, hieght);
    }
}
