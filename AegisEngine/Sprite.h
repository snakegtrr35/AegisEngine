#pragma once

#ifndef  SPRITE_H
#define SPRITE_H

#include	"Game_Object.h"
#include	"Menu_Component.h"
#include	"Renderer.h"
#include	"main.h"

class TEXTURE;

typedef struct {
	//! 子スプライト
	SPRITE* Child = nullptr;

	//! 親スプライト
	SPRITE* Parent = nullptr;

	//! 子スプライトの名前
	string Name;

}CHILD_DATE;

//==============================
// スプライトクラス
//==============================
class SPRITE : public GAME_OBJECT {
private:

protected:
	//! 頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! 頂点データ
	VERTEX_3D Vertex[4];

	//! テクスチャ
	TEXTURE* Texture;

	//! ポジション
	XMFLOAT2 Position;

	// サイズ
	// x : Top
	// y : Right
	// z : Bottom
	// w : Left
	XMFLOAT4 Size;

	COLOR Color;

	//! 子スプライトのリスト
	list<CHILD_DATE> Children;

	//! メニューイベントのリスト
	list<MENU_COMPONENT*> MenuEvents;

	//! スプライトの有効フラグ
	bool Enable = true;

	// インデックスバッファ
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;

	ID3D11ShaderResourceView* ShaderResourceView = nullptr;

	void Draw_Child() {

		for (CHILD_DATE child : Children)
		{
			if (child.Child->Enable)
			{
				child.Child->Draw();

				child.Child->Draw_Child();
			}
		}
	};

public:
	SPRITE();
	//==============================
	// position : 中心座標
	// size : サイズ
	//==============================
	SPRITE(XMFLOAT2 position, XMFLOAT4 size);
	~SPRITE();

	void Init() override;
	void Draw() override;
	void Update() override;
	void Uninit() override;

	void SetPosition(const XMFLOAT2& position);
	XMFLOAT2* const GetPosition();

	void SetEnable(const bool flag);
	const bool GetEnable();

	void SetColor(COLOR& const color);
	COLOR& const GetColor();

	void SetSize(const XMFLOAT4& size);							// サイズの設定
	XMFLOAT4* const GetSize();

	void SetTexture(const string& const file_name);				// テクスチャの設定
	TEXTURE* const GetTexture();								// テクスチャの取得

	// リストへの子スプライトの追加
	SPRITE* Add_Child_Sptite(const string& name, SPRITE* const parent)
	{
		SPRITE* sprite = new SPRITE();

		sprite->Enable = parent->GetEnable();


		CHILD_DATE child;

		child.Child = sprite;

		child.Name = name;

		child.Parent = parent;

		Children.push_back(child);

		return child.Child;
	}

	// 子スプライトのリストの取得
	list<CHILD_DATE> const Get_Child_Sptite()
	{
		return Children;
	}

	// リストへのメニューイベントの追加
	template <typename T>
	T* Add_Menu_Event()
	{
		for (auto object : MenuEvents)
		{
			if (typeid(T) == typeid(*object))
			{
				return (T*)object;
			}

			T* component = new T();

			component->SetEnable(true);

			MenuEvents.push_back(component);

			return component;
		}
	}

	// リストのメニューイベントの取得
	template <typename T>
	T* Get_Menu_Event()
	{
		for (auto component : MenuEvents)
		{
			if (typeid(T) == typeid(*component))
			{
				return (T*)component;
			}
		}
	}

	// リストのメニューイベントの有効無効化
	template <typename T>
	void Menu_Event_Enable(const bool const flag)
	{
		for (auto component : MenuEvents)
		{
			if (typeid(T) == typeid(*component))
			{
				component->SetEnable(flag);
				return;
			}
		}
	}

	void Set_Position_Child(const string& const name, const XMFLOAT2 position) {

		for (CHILD_DATE child : Children)
		{
			if (name == child.Name)
			{
				XMFLOAT2 pos( child.Parent->GetPosition()->x + position.x, child.Parent->GetPosition()->y + position.y );

				child.Child->SetPosition(pos);
				return;
			}
		}
	};

	void Set_Enable_Child(const bool flag) {

		for (CHILD_DATE child : Children)
		{
			child.Child->Enable = flag;
		}
	};

	void Set_Enable_Child(const string& const name, const bool flag) {

		for (CHILD_DATE child : Children)
		{
			if (name == child.Name)
			{
				child.Child->Enable = flag;
				return;
			}
		}
	};

	const bool Get_Enable_Child(const string& const name) {

		for (CHILD_DATE child : Children)
		{
			if (name == child.Name)
			{
				return child.Child->Enable;
			}
		}
	};
	
	void Set(ID3D11ShaderResourceView* shader_resource_view) {
		ShaderResourceView = shader_resource_view;
	};
};

//////////////////////////////////////////////////

//==============================
// スプライトアニメーションクラス
//==============================
class SPRITE_ANIMATION : public SPRITE {
private:
	float WaitFrame;
	unsigned short CreatCount;
	unsigned short PatternCount;
	unsigned short Age;

	static int FrameCount;

	float Tx, Ty;	// テクスチャ切り取り座標
	float Tw, Th;	// テクスチャの切り取り幅と高さ

	unsigned char  Pattern_Max_X;
	unsigned char  Pattern_Max_Y;

	void Draw2(float tx = -1.0f, float ty = -1.0f);

protected:


public:
	float Tx_Param, Ty_Param;	// テクスチャ切り取り座標（手動）

	SPRITE_ANIMATION();
	//==============================
	// position : 中心座標
	// size : サイズ
	//==============================
	SPRITE_ANIMATION(XMFLOAT2 position, XMFLOAT4 size);
	~SPRITE_ANIMATION();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	// テクスチャアニメーションのパラメーターの設定
	//
	// 引数:wait_frame ... 待ちフレーム
	//      x ... テクスチャ切り取り幅
	//      y ... テクスチャ切り取り高さ
	void SetParam(const float& wait_frame, const unsigned char& x, const unsigned char& y);

	// テクスチャアニメーションのテクスチャ切り取り座標の設定
	//
	// 引数:tx_param ... テクスチャ切り取り幅（手動）
	//      ty_param ... テクスチャ切り取り高さ（手動）
	void Set_Param_Txy(const float& tx_param = -1.0f, const float& ty_param = -1.0f);

	const XMFLOAT2& Get_Twh() {
		return XMFLOAT2(Tw, Th);
	};
};

/**
* @brief テキストクラス
* @details 文字列を扱うためのクラス
* @details スプライトクラスの派生クラス
*/
class TEXTS :public  SPRITE {
private:
	string Text;

	void Text_Draw(const string& text);

protected:


public:
	TEXTS();
	~TEXTS();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void Edit(const string& text);
};

#endif // ! SPRITE_H