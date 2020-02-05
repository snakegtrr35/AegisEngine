#include	"Sprite.h"
#include	"manager.h"
#include	"ShadowMap.h"
#include	"Scene.h"

unique_ptr<ID3D11Buffer, Release> SPRITE::pIndexBuffer;		// �C���f�b�N�X�o�b�t�@

SPRITE::SPRITE()
{
	Texture = nullptr;

	Position = XMFLOAT2(0.0f, 0.0f);

	Size = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	Color = COLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̐ݒ�
	Texture.reset(new TEXTURE(string("number.png")));
}

SPRITE::SPRITE(XMFLOAT2 position, XMFLOAT4 size)
{
	Texture = nullptr;

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	Position = position;

	Size = size;

	Color = COLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̐ݒ�
	Texture.reset(new TEXTURE(string("number.png")));
}

SPRITE::~SPRITE()
{
	Uninit();
}

void SPRITE::Init(void)
{
	// ���_�o�b�t�@�̐ݒ�
	if (nullptr == pVertexBuffer.get())
	{
		HRESULT hr;
		ID3D11Buffer* pVB = nullptr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// �T�u���\�[�X�̐ݒ�
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// ���_�o�b�t�@�̐���
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVB);

		if (FAILED(hr))
		{
			return;
		}

		pVertexBuffer.reset(pVB);
	}

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	if (nullptr == pIndexBuffer.get())
	{
		HRESULT hr;
		ID3D11Buffer* pIB = nullptr;

		const WORD index[] = {
			0, 1, 2,
			1, 3, 2,
		};

		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(WORD) * 6;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData;
		irData.pSysMem = index;
		irData.SysMemPitch = 0;
		irData.SysMemSlicePitch = 0;

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &pIB);
		if (FAILED(hr))
		{
			FAILDE_ASSERT
		}

		pIndexBuffer.reset(pIB);
	}
}

void SPRITE::Draw(void)
{
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		if (Enable)
		{
			Vertex[0].Position = XMFLOAT3(Position.x - Size.w, Position.y - Size.x, 0.0f);
			Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			Vertex[0].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

			Vertex[1].Position = XMFLOAT3(Position.x + Size.y, Position.y - Size.x, 0.0f);
			Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			Vertex[1].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

			Vertex[2].Position = XMFLOAT3(Position.x - Size.w, Position.y + Size.z, 0.0f);
			Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			Vertex[2].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

			Vertex[3].Position = XMFLOAT3(Position.x + Size.y, Position.y + Size.z, 0.0f);
			Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			Vertex[3].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

			// ���_�o�b�t�@�̏�������
			{
				D3D11_MAPPED_SUBRESOURCE msr;
				CRenderer::GetDeviceContext()->Map(pVertexBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4);
				CRenderer::GetDeviceContext()->Unmap(pVertexBuffer.get(), 0);
			}

			// ���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�
			CRenderer::SetVertexBuffers(pVertexBuffer.get());

			// ���̓A�Z���u���ɃC���f�b�N�X�o�b�t�@��ݒ�
			CRenderer::SetIndexBuffer(pIndexBuffer.get());

			if (nullptr == ShaderResourceView)
			{
				// �e�N�X�`���̐ݒ�
				Texture->Set_Texture();
			}
			else
			{
				CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &ShaderResourceView);
			}

			// 2D�}�g���b�N�X�ݒ�
			CRenderer::SetWorldViewProjection2D(Scaling);

			CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_LIGHT);

			CRenderer::DrawIndexed(6, 0, 0);
		}

		Draw_Child();

		CRenderer::Set_Shader();
	}
}

void SPRITE::Draw_DPP()
{
	if (Enable)
	{
		Vertex[0].Position = XMFLOAT3(Position.x - Size.w, Position.y - Size.x, 0.0f);
		Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		Vertex[0].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		Vertex[1].Position = XMFLOAT3(Position.x + Size.y, Position.y - Size.x, 0.0f);
		Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		Vertex[1].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

		Vertex[2].Position = XMFLOAT3(Position.x - Size.w, Position.y + Size.z, 0.0f);
		Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		Vertex[2].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

		Vertex[3].Position = XMFLOAT3(Position.x + Size.y, Position.y + Size.z, 0.0f);
		Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		Vertex[3].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

		// ���_�o�b�t�@�̏�������
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			CRenderer::GetDeviceContext()->Map(pVertexBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4);
			CRenderer::GetDeviceContext()->Unmap(pVertexBuffer.get(), 0);
		}

		// ���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�
		CRenderer::SetVertexBuffers(pVertexBuffer.get());

		// ���̓A�Z���u���ɃC���f�b�N�X�o�b�t�@��ݒ�
		CRenderer::SetIndexBuffer(pIndexBuffer.get());


		// 2D�}�g���b�N�X�ݒ�
		CRenderer::SetWorldViewProjection2D(Scaling);

		CRenderer::DrawIndexed(6, 0, 0);
	}

	Draw_DPP_Child();

	CRenderer::Set_Shader();
}

void SPRITE::Update(float delta_time)
{
}

void SPRITE::Uninit(void)
{	
	Texture.reset(nullptr);

	pVertexBuffer.reset(nullptr);

	for (CHILD_DATE child : Children)	
	{
		child.Child->Uninit();
		SAFE_DELETE(child.Child);
	}
	Children.clear();
}

void SPRITE::SetPosition(const XMFLOAT2& position)
{
	Position = position;
}

XMFLOAT2* const SPRITE::GetPosition()
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

void SPRITE::SetSize(const XMFLOAT4& size)
{
	Size = size;
}

XMFLOAT4* const SPRITE::GetSize()
{
	return &Size;
}

void SPRITE::SetTexture(const string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}

TEXTURE* const SPRITE::GetTexture()
{
	return Texture.get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int SPRITE_ANIMATION::FrameCount = 0;

SPRITE_ANIMATION::SPRITE_ANIMATION()
{
	WaitFrame = 0.f;
	CreatCount = 0;
	PatternCount = 0;
	Age = 0;

	Ty = Tx = 0.f;
	Tw = Th = 0.f;

	Pattern_Max_X = 0;
	Pattern_Max_Y = 0;

	Ty_Param = Tx_Param = -1.0f;

	SetTexture(string("Explosion.png"));
}

SPRITE_ANIMATION::SPRITE_ANIMATION(XMFLOAT2 position, XMFLOAT4 size)
{
	WaitFrame = 0.f;
	CreatCount = 0;
	PatternCount = 0;
	Age = 0;

	Ty = Tx = 0.f;
	Tw = Th = 0.f;

	Pattern_Max_X = 0;
	Pattern_Max_Y = 0;

	Position = position;

	Size = size;

	Ty_Param = Tx_Param = -1.0f;

	SetTexture(string("Explosion.png"));
}

SPRITE_ANIMATION::~SPRITE_ANIMATION()
{
	Uninit();
}

void SPRITE_ANIMATION::Init(void)
{
	SPRITE::Init();
}

void SPRITE_ANIMATION::Draw(void)
{
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		Draw2(Tx_Param, Ty_Param);
	}
}

void SPRITE_ANIMATION::Draw_DPP(void)
{
	Draw_DPP2(Tx_Param, Ty_Param);
}

void SPRITE_ANIMATION::Draw2(float tx, float ty)
{
	if (-1.0f == tx)
	{
		Tx = (float)(Tw * (PatternCount % Pattern_Max_X));
	}
	else
	{
		Tx = tx;
	}


	if (-1.0f == tx)
	{
		Ty = (float)(Th * (PatternCount / Pattern_Max_Y));
	}
	else
	{
		Ty = ty;
	}

	XMINT2* wh = Texture->Get_WH();

	// UV���W�v�Z
	float u[2], v[2];
	u[0] = (float)(Tx / wh->x);
	v[0] = (float)(Ty / wh->y);
	u[1] = (float)((Tx + Tw) / wh->x);
	v[1] = (float)((Ty + Th) / wh->y);

	Vertex[0].Position = XMFLOAT3(Position.x - Size.w, Position.y - Size.x, 0.0f);
	Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[0].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
	Vertex[0].TexCoord = XMFLOAT2(u[0], v[0]);

	Vertex[1].Position = XMFLOAT3(Position.x + Size.y, Position.y - Size.x, 0.0f);
	Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[1].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
	Vertex[1].TexCoord = XMFLOAT2(u[1], v[0]);

	Vertex[2].Position = XMFLOAT3(Position.x - Size.w, Position.y + Size.z, 0.0f);
	Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[2].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
	Vertex[2].TexCoord = XMFLOAT2(u[0], v[1]);

	Vertex[3].Position = XMFLOAT3(Position.x + Size.y, Position.y + Size.z, 0.0f);
	Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[3].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
	Vertex[3].TexCoord = XMFLOAT2(u[1], v[1]);

	// ���_�o�b�t�@�̏�������
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(pVertexBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4); // 4���_���R�s�[
		CRenderer::GetDeviceContext()->Unmap(pVertexBuffer.get(), 0);
	}

	// ���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�
	CRenderer::SetVertexBuffers(pVertexBuffer.get());

	// ���̓A�Z���u���ɃC���f�b�N�X�o�b�t�@��ݒ�
	CRenderer::SetIndexBuffer(pIndexBuffer.get());

	// �e�N�X�`���̐ݒ�
	Texture->Set_Texture();

	// 2D�}�g���b�N�X�ݒ�
	CRenderer::SetWorldViewProjection2D(Scaling);

	CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_LIGHT);

	CRenderer::DrawIndexed(6, 0, 0);

	CRenderer::Set_Shader();
}

void SPRITE_ANIMATION::Draw_DPP2(float tx, float ty)
{
	if (-1.0f == tx)
	{
		Tx = (float)(Tw * (PatternCount % Pattern_Max_X));
	}
	else
	{
		Tx = tx;
	}


	if (-1.0f == tx)
	{
		Ty = (float)(Th * (PatternCount / Pattern_Max_Y));
	}
	else
	{
		Ty = ty;
	}

	XMINT2* wh = Texture->Get_WH();

	// UV���W�v�Z
	float u[2], v[2];
	u[0] = (float)(Tx / wh->x);
	v[0] = (float)(Ty / wh->y);
	u[1] = (float)((Tx + Tw) / wh->x);
	v[1] = (float)((Ty + Th) / wh->y);

	Vertex[0].Position = XMFLOAT3(Position.x - Size.w, Position.y - Size.x, 0.0f);
	Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[0].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
	Vertex[0].TexCoord = XMFLOAT2(u[0], v[1]);

	Vertex[1].Position = XMFLOAT3(Position.x + Size.y, Position.y - Size.x, 0.0f);
	Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[1].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
	Vertex[1].TexCoord = XMFLOAT2(u[1], v[1]);

	Vertex[2].Position = XMFLOAT3(Position.x - Size.w, Position.y + Size.z, 0.0f);
	Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[2].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
	Vertex[2].TexCoord = XMFLOAT2(u[0], v[0]);

	Vertex[3].Position = XMFLOAT3(Position.x + Size.y, Position.y + Size.z, 0.0f);
	Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[3].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
	Vertex[3].TexCoord = XMFLOAT2(u[1], v[0]);

	// ���_�o�b�t�@�̏�������
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(pVertexBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4); // 4���_���R�s�[
		CRenderer::GetDeviceContext()->Unmap(pVertexBuffer.get(), 0);
	}

	// ���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�
	CRenderer::SetVertexBuffers(pVertexBuffer.get());

	// ���̓A�Z���u���ɃC���f�b�N�X�o�b�t�@��ݒ�
	CRenderer::SetIndexBuffer(pIndexBuffer.get());

	// 2D�}�g���b�N�X�ݒ�
	CRenderer::SetWorldViewProjection2D(Scaling);

	CRenderer::DrawIndexed(6, 0, 0);
}

void SPRITE_ANIMATION::Update(float delta_time)
{
	Age++;

	PatternCount = (int)(Age / WaitFrame);

	if ((Pattern_Max_X * Pattern_Max_Y) <= PatternCount)
	{
		//SCENE::Destroy_Game_Object(this);
	}
}

void SPRITE_ANIMATION::Uninit(void)
{
}

// �e�N�X�`���A�j���[�V�����̃p�����[�^�[�̐ݒ�
//
// ����:wait_frame ... �҂��t���[��
//      tw ... �e�N�X�`���؂��萔�ix�j
//      th ... �e�N�X�`���؂��萔�iy�j
void SPRITE_ANIMATION::SetParam(const float& wait_frame, const unsigned char& x, const unsigned char& y)
{
	WaitFrame = wait_frame;
	Pattern_Max_X = x;
	Pattern_Max_Y = y;

	XMINT2* wh = Texture->Get_WH();

	Tw = (float)(wh->x / Pattern_Max_X);
	Th = (float)(wh->y / Pattern_Max_Y);
}

// �e�N�X�`���A�j���[�V�����̃e�N�X�`���؂�����W�̐ݒ�
//
// ����:tx_param ... �e�N�X�`���؂��蕝�i�蓮�j
//      ty_param ... �e�N�X�`���؂��荂���i�蓮�j
void SPRITE_ANIMATION::Set_Param_Txy(const float& tx_param , const float& ty_param)
{
	Tx_Param = tx_param;
	Ty_Param = ty_param;
}




TEXTS::TEXTS()
{
	Text = "";
}

TEXTS::~TEXTS()
{
	Uninit();
}

void TEXTS::Init(void)
{
	SPRITE::Init();
}

void TEXTS::Draw(void)
{
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		Text_Draw(Text);

		Draw_Child();
	}
}

void TEXTS::Draw_DPP(void)
{
	Text_Draw(Text);

	Draw_DPP_Child();
}

void TEXTS::Update(float delta_time)
{
}

void TEXTS::Uninit(void)
{
}

void TEXTS::Edit(const string& text)
{
	Text = text;
}

void TEXTS::Text_Draw(const string& text)
{
	if (Enable)
	{
		SPRITE sprite;
		ID3D11ShaderResourceView* shader_resource_view = nullptr;
		wstring font;
		short i = 0;

		sprite.Init();

		sprite.SetSize(Size);

		sprite.SetColor(Color);

		string ctext = text;
		wstring wtext = stringTowstring(ctext);

		for (auto itr : wtext)
		{
			font.push_back(itr);
			shader_resource_view = FONT::Get_Font_Resource(font);

			sprite.Set(shader_resource_view);

			sprite.SetPosition(XMFLOAT2(Position.x + (Size.w * i * 2), Position.y));

			sprite.Draw();

			i++;

			font.pop_back();
		}
	}
}

void TEXTS::Text_Draw_DPP(const string& text)
{
	SPRITE sprite;
	ID3D11ShaderResourceView* shader_resource_view = nullptr;
	wstring font;
	short i = 0;

	sprite.Init();

	sprite.SetSize(Size);

	sprite.SetColor(Color);

	string ctext = text;
	wstring wtext = stringTowstring(ctext);

	for (auto itr : wtext)
	{
		font.push_back(itr);
		shader_resource_view = FONT::Get_Font_Resource(font);

		sprite.Set(shader_resource_view);

		sprite.SetPosition(XMFLOAT2(Position.x + (Size.w * i * 2), Position.y));

		sprite.Draw_DPP();

		i++;

		font.pop_back();
	}
}