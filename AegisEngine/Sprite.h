/**
 * @file Sprite.h
 * @brief スプライト関係のヘッダーファイル
 */

#pragma once

#ifndef  SPRITE_H
#define SPRITE_H

#include	"Game_Object.h"
#include	"Menu_Component.h"
#include	"Renderer.h"
#include	"main.h"

class TEXTURE;

/**
 * 子スプライト情報
 */
typedef struct {
	//! 子スプライトのポインタ
	SPRITE* Child = nullptr;

	//! 親スプライトのポインタ
	SPRITE* Parent = nullptr;

	//! 子スプライトの名前
	string Name;

}CHILD_DATE;

/**
* @brief スプライトクラス
* @details スプライトクラス
*/
class SPRITE : public GAME_OBJECT {
private:

protected:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		//!< 頂点バッファ

	VERTEX_3D Vertex[4];		//!< 頂点データ

	TEXTURE* Texture;		//!< テクスチャ

	XMFLOAT2 Position;		//!< ポジション

	/**
	* サイズ
	* x : 上
	* y : 右
	* z : 下
	* w : 左
	*/
	XMFLOAT4 Size;

	COLOR Color;		//!< カラー

	list<CHILD_DATE> Children;		//!< 子スプライトのリスト

	list<MENU_COMPONENT*> MenuEvents;		//!< メニューイベント(リスト)

	bool Enable = true;		//!< スプライトの有効無効フラグ(デフォルトは有効)

	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;		//!< インデックスバッファ

	ID3D11ShaderResourceView* ShaderResourceView = nullptr;

	/**
	* @brief 子スプライトの描画関数
	* @details 子スプライトの描画する関数
	*/
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
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	SPRITE();

	/**
	* @brief コンストラクタ
	* @param position 中心座標
	* @param size サイズ
	* @details 引数付きコンストラクタ
	*/
	SPRITE(XMFLOAT2 position, XMFLOAT4 size);

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~SPRITE();

	/**
	* @brief 初期化関数
	* @details 初期化する関数
	*/
	void Init() override;

	/**
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw() override;

	/**
	* @brief 更新関数
	* @details 更新する関数
	*/
	void Update(float delta_time) override;

	/**
	* @brief 終了処理関数
	* @details 終了処理をする関数
	*/
	void Uninit() override;

	/**
	* @brief 座標を設定する関数
	* @param position 座標(二次元座標)
	* @details 座標(二次元座標)を設定する関数
	*/
	void SetPosition(const XMFLOAT2& position);

	/**
	* @brief 座標を取得する関数
	* @return XMFLOAT2* 座標(二次元座標)(ポインタ)
	* @details 座標(二次元座標)を取得する関数
	*/
	XMFLOAT2* const GetPosition();

	/**
	* @brief 描画の有効無効を設定する関数
	* @param flag trur又はfalse
	* @details 描画の有効無効を設定する関数
	*/
	void SetEnable(const bool flag);

	/**
	* @brief 描画の有効無効を取得する関数
	* @return bool 描画の有効無効のフラグ
	* @details 描画の有効無効を取得する関数
	*/
	const bool GetEnable();

	/**
	* @brief 頂点カラーを設定する関数
	* @param color カラー構造体
	* @details 頂点カラー(カラー構造体)を設定する関数
	*/
	void SetColor(COLOR& const color);

	/**
	* @brief 頂点カラーを取得する関数
	* @return COLOR 頂点カラー(カラー構造体)
	* @details 頂点カラー(カラー構造体)を取得する関数
	*/
	COLOR& const GetColor();

	/**
	* @brief サイズを設定する関数
	* @param size x:上 y:右 z:下 w:左
	* @details サイズ(4方向)を設定する関数
	*/
	void SetSize(const XMFLOAT4& size);

	/**
	* @brief サイズを取得する関数
	* @return XMFLOAT4 サイズ(4方向)(x:上 y:右 z:下 w:左)
	* @details サイズ(4方向)をを取得する関数
	*/
	XMFLOAT4* const GetSize();

	/**
	* @brief テクスチャを設定する関数
	* @param file_name 使用するテクスチャ名
	* @details テクスチャ(クラス)を設定する関数
	*/
	void SetTexture(const string& const file_name);

	/**
	* @brief テクスチャを取得する関数
	* @return TEXTURE テクスチャクラス
	* @details テクスチャ(クラス)を取得する関数
	*/
	TEXTURE* const GetTexture();

	/**
	* @brief 子スプライトを追加する関数
	* @param name 子スプライトの名前
	* @param parent 親スプライトのポインタ
	* @return SPRITE 子スプライトのポインタ
	* @details 子スプライトを一つ追加する関数
	*/
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

	/**
	* @brief 子スプライトを取得する関数
	* @return list<CHILD_DATE> 子スプライトのリスト
	* @details 子スプライトのリストを取得する関数
	*/
	list<CHILD_DATE> const Get_Child_Sptite()
	{
		return Children;
	}

	/**
	* @brief メニューイベントを追加する関数
	* @tparam T 追加するメニューイベントのクラス名
	* @return T 追加したメニューイベントのポインタ
	* @details メニューイベントを一つ追加する関数
	*/
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

	/**
	* @brief メニューイベントを取得する関数
	* @tparam T 取得したいメニューイベントのクラス名
	* @return T メニューイベント
	* @details メニューイベントを取得する関数
	*/
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

	/**
	* @brief メニューイベントを有効無効する関数
	* @tparam T 有効無効化したいメニューイベントのクラス名
	* @param flag trur又はfalse
	* @details 特定のメニューイベントを有効無効化する関数
	*/
	template <typename T>
	void Menu_Event_Enable(const bool flag)
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

	/**
	* @brief 子スプライトの座標を設定する関数
	* @param name 設定する子スプライトの名前
	* @param position 座標(二次元座標)
	* @details 特定の子スプライトの座標(二次元座標、親スプライトからオフセットされる)を設定する関数
	*/
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

	/**
	* @brief 子スプライトの描画の有効無効を設定する関数
	* @param flag 描画の有効無効のフラグ
	* @details 全ての子スプライトの描画の有効無効を設定する関数
	*/
	void Set_Enable_Child(const bool flag) {

		for (CHILD_DATE child : Children)
		{
			child.Child->Enable = flag;
		}
	};

	/**
	* @brief 子スプライトの描画の有効無効を設定する関数
	* @param name 設定する子スプライトの名前
	* @param flag 描画の有効無効のフラグ
	* @details 特定の子スプライトの描画の有効無効を設定する関数
	*/
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

	/**
	* @brief 子スプライトの描画の有効無効を取得する関数
	* @param name 子スプライトの名前
	* @return bool 描画の有効無効のフラグ
	* @details 特定の子スプライトの描画の有効無効を取得する関数
	*/
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

////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief スプライトアニメーションクラス
* @details スプライトでアニメーションができるクラス
*/
class SPRITE_ANIMATION : public SPRITE {
private:
	float WaitFrame;
	unsigned short CreatCount;
	unsigned short PatternCount;
	unsigned short Age;

	static int FrameCount;

	float Tx, Ty;	//!< テクスチャ切り取り座標
	float Tw, Th;	//!< テクスチャの切り取り幅と高さ

	float Tx_Param, Ty_Param;	//!< テクスチャ切り取り座標（手動）

	unsigned char  Pattern_Max_X;	//!< テクスチャの分割数(横)
	unsigned char  Pattern_Max_Y;	//!< テクスチャの分割数(縦)

	/**
	* @brief 描画関数
	* @details 実際に描画する関数
	*/
	void Draw2(float tx = -1.0f, float ty = -1.0f);

protected:


public:

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	SPRITE_ANIMATION();

	/**
	* @brief コンストラクタ
	* @param  position 中心座標(二次元座標)
	* @param size サイズ
	* @details 引数付きコンストラクタ
	*/
	SPRITE_ANIMATION(XMFLOAT2 position, XMFLOAT4 size);

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~SPRITE_ANIMATION();

	/**
	* @brief 初期化関数
	* @details 初期化する関数
	*/
	void Init() override;

	/**
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw() override;

	/**
	* @brief 更新関数
	* @details 更新する関数
	*/
	void Update(float delta_time) override;

	/**
	* @brief 終了処理関数
	* @details 終了処理をする関数
	*/
	void Uninit() override;

	/**
	* @brief テクスチャアニメーションのパラメーターの設定する関数
	* @param wait_frame 待ちフレーム
	* @param x テクスチャ切り取り幅
	* @param y テクスチャ切り取り高さ
	* @details テクスチャアニメーションのパラメーターの設定する関数
	*/
	void SetParam(const float& wait_frame, const unsigned char& x, const unsigned char& y);

	/**
	* @brief テクスチャアニメーションのテクスチャ切り取り座標を設定する関数
	* @param tx_param テクスチャ切り取り幅（手動）
	* @param ty_param テクスチャ切り取り高さ（手動）
	* @details テクスチャアニメーションのテクスチャ切り取り座標を設定する関数
	*/
	void Set_Param_Txy(const float& tx_param = -1.0f, const float& ty_param = -1.0f);

	/**
	* @brief テクスチャの切り取り幅と高さを取得する関数
	* @return XMFLOAT2 テクスチャの切り取り幅と高さ
	* @details テクスチャの切り取り幅と高さを取得する関数
	*/
	const XMFLOAT2 Get_Twh() {
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
	string Text;	//!< 表示する文字列

	/**
	* @brief 描画関数
	* @details 実際に描画する関数
	*/
	void Text_Draw(const string& text);

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	TEXTS();

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~TEXTS();

	/**
	* @brief 初期化関数
	* @details 初期化する関数
	*/
	void Init() override;

	/**
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw() override;

	/**
	* @brief 更新関数
	* @details 更新する関数
	*/
	void Update(float delta_time) override;

	/**
	* @brief 終了処理関数
	* @details 終了処理をする関数
	*/
	void Uninit() override;

	/**
	* @brief 表示する文字列を設定する関数
	* @param text 設定する文字列
	* @details 表示する文字列を設定する関数
	*/
	void Edit(const string& text);
};

#endif // ! SPRITE_H