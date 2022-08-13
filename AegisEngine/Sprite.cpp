#include "Sprite.h"
#include "manager.h"
#include "ShadowMap.h"
#include "Scene.h"
#include "texture.h"

IMPLEMENT_OBJECT_TYPE_INFO(GameObject, SPRITE)

using namespace aegis;

aegis::uniquePtr<aegis::Buffer> SPRITE::IndexBuffer;		// インデックスバッファ

SPRITE::SPRITE()
{
	Texture = nullptr;

	Position = Vector2(0.0f, 0.0f);

	Size = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	Color = COLOR(1.0f, 1.0f, 1.0f, 1.0f);

	Enable = true;

	// テクスチャの設定
	Texture.reset(new TEXTURE(aegis::string("number.png")));
}

SPRITE::SPRITE(Vector2 position, Vector4 size)
{
	Texture = nullptr;

	Position = position;

	Size = size;

	Color = COLOR(1.0f, 1.0f, 1.0f, 1.0f);

	Enable = true;

	// テクスチャの設定
	Texture.reset(new TEXTURE(aegis::string("number.png")));
}

SPRITE::~SPRITE()
{
	Uninit();

	GameObject::Uninit();
}

void SPRITE::Init(void)
{
	CRenderer* render = CRenderer::getInstance();

	// 頂点バッファの設定
	if (nullptr == VertexBuffer)
	{
		BufferDesc  bd{};
		bd.Usage = Usage::Dynamic;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = BindFlag::Vertexbuffer;
		bd.CPUAccessFlags = CpuAccessFlag::Write;

		// サブリソースの設定
		SubresourceData sd{};
		sd.pSysMem = Vertex;

		// 頂点バッファの生成
		VertexBuffer.reset(render->CreateBuffer(bd, sd));
	}

	// インデックスバッファの設定
	if (nullptr == IndexBuffer)
	{
		const WORD index[] =
		{
			0, 1, 2,
			1, 3, 2,
		};

		BufferDesc  bd{};
		bd.Usage = Usage::Default;
		bd.ByteWidth = sizeof(uint16) * 6;
		bd.BindFlags = BindFlag::Indexbuffer;
		bd.CPUAccessFlags = CpuAccessFlag::None;

		SubresourceData sd{};
		sd.pSysMem = index;

		IndexBuffer.reset(render->CreateBuffer(bd, sd));
	}

	for (const auto& child : Children)
	{
		child->Child->Init();
	}

	GameObject::InitEnd();
}

void SPRITE::Draw(void)
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		return;
	}

	CRenderer* render = CRenderer::getInstance();

	if (Enable)
	{
		Vertex[0].Position = Vector3(Position.x - Size.w, Position.y - Size.x, 0.0f);
		Vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[0].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
		Vertex[0].TexCoord = Vector2(0.0f, 0.0f);

		Vertex[1].Position = Vector3(Position.x + Size.y, Position.y - Size.x, 0.0f);
		Vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[1].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
		Vertex[1].TexCoord = Vector2(1.0f, 0.0f);

		Vertex[2].Position = Vector3(Position.x - Size.w, Position.y + Size.z, 0.0f);
		Vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[2].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
		Vertex[2].TexCoord = Vector2(0.0f, 1.0f);

		Vertex[3].Position = Vector3(Position.x + Size.y, Position.y + Size.z, 0.0f);
		Vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[3].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
		Vertex[3].TexCoord = Vector2(1.0f, 1.0f);

		// 頂点バッファの書き換え
		{
			render->Map(VertexBuffer.get(), Vertex, sizeof(VERTEX_3D) * 4);
			render->Unmap(VertexBuffer.get());
		}

		// 入力アセンブラに頂点バッファを設定
		render->SetVertexBuffers(VertexBuffer.get());

		// 入力アセンブラにインデックスバッファを設定
		render->SetIndexBuffer(IndexBuffer.get());

		//if (ShaderResourceView == nullptr 
		//	&& ShaderResourceView->IsEmpty())
		//{
		//	// テクスチャの設定
		//	Texture->Set_Texture();
		//}
		//else
		//{
		//	aegis::ShaderResourceView* shaderResourceViews[] = { ShaderResourceView.get()};
		//
		//	render->PSSetShaderResources(0, 1, shaderResourceViews);
		//}

		if (ShaderResourceView != nullptr
			&& !ShaderResourceView->IsEmpty())
		{
			aegis::ShaderResourceView* shaderResourceViews[] = { ShaderResourceView.get() };

			render->PSSetShaderResources(0, 1, shaderResourceViews);
		}
		else
		{
			// テクスチャの設定
			Texture->Set_Texture();
		}

		// 2Dマトリックス設定
		render->SetWorldViewProjection2D(Get_Transform().Get_Scaling());

		render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

		if (flag)
		{
			render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_LIGHT);
		}
		else
		{
			render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::POST_EFFECT);
		}

		render->DrawIndexed(6, 0, 0);

		Draw_Child();
	}

	render->Set_Shader();
}

void SPRITE::Draw_DPP()
{
	CRenderer* render = CRenderer::getInstance();

	if (Enable)
	{
		Vertex[0].Position = Vector3(Position.x - Size.w, Position.y - Size.x, 0.0f);
		Vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[0].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
		Vertex[0].TexCoord = Vector2(0.0f, 0.0f);

		Vertex[1].Position = Vector3(Position.x + Size.y, Position.y - Size.x, 0.0f);
		Vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[1].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
		Vertex[1].TexCoord = Vector2(1.0f, 0.0f);

		Vertex[2].Position = Vector3(Position.x - Size.w, Position.y + Size.z, 0.0f);
		Vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[2].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
		Vertex[2].TexCoord = Vector2(0.0f, 1.0f);

		Vertex[3].Position = Vector3(Position.x + Size.y, Position.y + Size.z, 0.0f);
		Vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[3].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
		Vertex[3].TexCoord = Vector2(1.0f, 1.0f);

		// 頂点バッファの書き換え
		{
			render->Map(VertexBuffer.get(), Vertex, sizeof(VERTEX_3D) * 4);
			render->Unmap(VertexBuffer.get());
		}

		// 入力アセンブラに頂点バッファを設定
		render->SetVertexBuffers(VertexBuffer.get());

		// 入力アセンブラにインデックスバッファを設定
		render->SetIndexBuffer(IndexBuffer.get());


		// 2Dマトリックス設定
		render->SetWorldViewProjection2D(Get_Transform().Get_Scaling());

		render->DrawIndexed(6, 0, 0);
	}

	Draw_DPP_Child();

	render->Set_Shader();
}

void SPRITE::Update(float delta_time)
{
	GameObject::Update(delta_time);
}

void SPRITE::Uninit(void)
{	
	GameObject::Uninit();

	Texture.reset(nullptr);

	for (auto& child : Children)
	{
		child->Child->Uninit();
		child->Child.reset(nullptr);
	}
	Children.clear();
}

void SPRITE::SetPosition(const Vector2& position)
{
	Position = position;
}

Vector2* const SPRITE::GetPosition()
{
	return &Position;
}


void SPRITE::SetEnable(const bool flag)
{
	Enable = flag;

	//Set_Enable_Child(flag);
}

const bool SPRITE::GetEnable()
{
	return Enable;
}

void SPRITE::SetColor(COLOR& const color)
{
	Color = color;
}

COLOR& const SPRITE::GetColor()
{
	return Color;
}

void SPRITE::SetSize(const Vector4& size)
{
	Size = size;
}

Vector4* const SPRITE::GetSize()
{
	return &Size;
}

void SPRITE::SetTexture(const aegis::string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}

TEXTURE* const SPRITE::GetTexture()
{
	return Texture.get();
}



SPRITE* SPRITE::Add_Child_Sptite(const aegis::string& name)
{
	std::unique_ptr<CHILD_DATE, Delete> child;
	child.reset(new CHILD_DATE());

	child->Child.reset(new SPRITE());

	child->Name = name;

	Children.emplace_back(std::move(child));

	return (Children.end() - 1)->get()->Child.get();
}

aegis::vector< std::unique_ptr<CHILD_DATE, Delete> >* const SPRITE::Get_Child_Sptite()
{
	return &Children;
}

CHILD_DATE* const SPRITE::Get_Child_Sptite(const aegis::string& name)
{
	for (const auto& child : Children)
	{
		if (name == child->Name)
		{
			return child.get();
		}

		for (const auto& grandchild : *child->Child->Get_Child_Sptite())
		{
			if (name == grandchild->Name)
			{
				return grandchild.get();
			}
		}
	}

	return nullptr;
}

void SPRITE::Draw_Child()
{
	for (const auto& child : Children)
	{
		if (child->Child->Enable)
		{
			child->Child->Draw();

			child->Child->Draw_Child();
		}
	}
};

void SPRITE::Draw_DPP_Child()
{
	for (const auto& child : Children)
	{
		if (child->Child->Enable)
		{
			child->Child->Draw_DPP();

			child->Child->Draw_DPP_Child();
		}
	}
};

void SPRITE::Set_Position_Child(const aegis::string& const name, const Vector2& position, const Vector2& offset)
{
	for (const auto& child : Children)
	{
		if (name == child->Name)
		{
			Vector2 pos(position.x + offset.x, position.y + offset.y);

			child->Child->SetPosition(pos);
			return;
		}
	}
};

void SPRITE::Set_Enable_Child(const bool flag)
{
	for (const auto& child : Children)
	{
		child->Child->Enable = flag;

		for (auto& grandchild : *child->Child->Get_Child_Sptite())
		{
			grandchild->Child->Set_Enable_Child(flag);
		}
	}
};

void SPRITE::Set_Enable_Child(const aegis::string& const name, const bool flag)
{
	for (const auto& child : Children)
	{
		if (name == child->Name)
		{
			child->Child->Enable = flag;
			return;
		}

		for (auto& grandchild : *child->Child->Get_Child_Sptite())
		{
			grandchild->Child->Set_Enable_Child(flag);
		}
	}
};

const bool SPRITE::Get_Enable_Child(const aegis::string& const name, aegis::vector< std::unique_ptr<CHILD_DATE, Delete> >* const children)
{
	for (const auto& child : *children)
	{
		if (name == child->Name)
		{
			return child->Child->Enable;
		}

		for (auto& grandchild : *child->Child->Get_Child_Sptite())
		{
			return Get_Enable_Child( name, grandchild->Child->Get_Child_Sptite());
		}
	}

	return false;
};