/**
 * @file Sprite.h
 * @brief スプライト関係のヘッダーファイル
 */

#pragma once

#ifndef  SPRITE_H
#define SPRITE_H

#include	"GameObject.h"

#include	"texture.h"
#include	"Menu_Component.h"
#include	"Renderer.h"

/**
 * 子スプライト情報
 */
struct CHILD_DATE {
	//! 子スプライトのポインタ
	std::unique_ptr<SPRITE> Child;

	////! オフセット位置
	//aegis::Vector3 Offset;

	//! 子スプライトの名前
	aegis::string Name;

	CHILD_DATE() : /*Offset(aegis::Vector3(0.f, 0.f, 0.f)),*/ Name("") {
		Child.reset(nullptr);
	}

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("Child", Child),
				cereal::make_nvp("Name", std::string(Name))
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("Child", Child));

		std::string s;
		archive(cereal::make_nvp("Name", s));
		Name.reserve(s.size());
		Name = s;
	}

};

/**
* @brief スプライトクラス
* @details スプライトクラス
*/
class SPRITE : public GameObject {

	OBJECT_TYPE_INFO(SPRITE)

private:
protected:
	//! 頂点バッファ
	ComPtr<ID3D11Buffer> pVertexBuffer;

	//! インデックスバッファ
	static ComPtr<ID3D11Buffer> pIndexBuffer;

	//! 頂点データ
	VERTEX_3D Vertex[4];
	//! テクスチャ
	std::unique_ptr<TEXTURE> Texture;

	//! ポジション
	aegis::Vector2 Position;

	/**
	* サイズ
	* x : 上
	* y : 右
	* z : 下
	* w : 左
	*/
	aegis::Vector4 Size;

	//!< カラー
	aegis::COLOR Color;

	//!< 子スプライトのリスト
	aegis::vector< std::unique_ptr<CHILD_DATE> > Children;

	//!< メニューイベント(リスト)
	aegis::list<MENU_COMPONENT*> MenuEvents;

	//!< スプライトの有効無効フラグ(デフォルトは true )
	bool Enable;


	ID3D11ShaderResourceView* ShaderResourceView = nullptr;

	/**
	* @brief 子スプライトの描画関数
	* @details 子スプライトの描画する関数
	*/
	void Draw_Child();

	/**
	* @brief 子スプライトの描画関数
	* @details 子スプライトの描画する関数
	*/
	void Draw_DPP_Child();

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
	SPRITE(aegis::Vector2 position, aegis::Vector4 size);

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~SPRITE();

	/**
	* @brief 初期化関数
	* @details 初期化する関数
	*/
	void Init();

	/**
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw();

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	void Draw_DPP();

	/**
	* @brief 更新関数
	* @details 更新する関数
	*/
	void Update(float delta_time);

	/**
	* @brief 終了処理関数
	* @details 終了処理をする関数
	*/
	void Uninit();

	/**
	* @brief 座標を設定する関数
	* @param position 座標(二次元座標)
	* @details 座標(二次元座標)を設定する関数
	*/
	void SetPosition(const aegis::Vector2& position);

	/**
	* @brief 座標を取得する関数
	* @return aegis::Vector2* 座標(二次元座標)(ポインタ)
	* @details 座標(二次元座標)を取得する関数
	*/
	aegis::Vector2* const GetPosition();

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
	void SetColor(aegis::COLOR& const color);

	/**
	* @brief 頂点カラーを取得する関数
	* @return COLOR 頂点カラー(カラー構造体)
	* @details 頂点カラー(カラー構造体)を取得する関数
	*/
	aegis::COLOR& const GetColor();

	/**
	* @brief サイズを設定する関数
	* @param size x:上 y:右 z:下 w:左
	* @details サイズ(4方向)を設定する関数
	*/
	void SetSize(const aegis::Vector4& size);

	/**
	* @brief サイズを取得する関数
	* @return aegis::Vector4 サイズ(4方向)(x:上 y:右 z:下 w:左)
	* @details サイズ(4方向)をを取得する関数
	*/
	aegis::Vector4* const GetSize();

	/**
	* @brief テクスチャを設定する関数
	* @param file_name 使用するテクスチャ名
	* @details テクスチャ(クラス)を設定する関数
	*/
	void SetTexture(const aegis::string& const file_name);

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
	SPRITE* Add_Child_Sptite(const aegis::string& name);

	/**
	* @brief 子スプライトを取得する関数
	* @return list<CHILD_DATE> 子スプライトのリスト
	* @details 子スプライトのリストを取得する関数
	*/
	aegis::vector< std::unique_ptr<CHILD_DATE> >* const Get_Child_Sptite();

	/**
	* @brief 子スプライトを取得する関数
	* @return list<CHILD_DATE> 子スプライトのリスト
	* @details 子スプライトのリストを取得する関数
	*/
	CHILD_DATE* const Get_Child_Sptite(const aegis::string& name);

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
	* @brief 子スプライトの座標を設定する関数
	* @param name 設定する子スプライトの名前
	* @param position 親の座標(二次元座標)
	* @param offset オフセット位置(二次元座標)
	* @details 特定の子スプライトの座標(二次元座標、親スプライトからオフセットされる)を設定する関数
	*/
	void Set_Position_Child(const aegis::string& const name, const aegis::Vector2& position, const aegis::Vector2& offset);

	/**
	* @brief 子スプライトの描画の有効無効を設定する関数
	* @param flag 描画の有効無効のフラグ
	* @details 全ての子スプライトの描画の有効無効を設定する関数
	*/
	void Set_Enable_Child(const bool flag);

	/**
	* @brief 子スプライトの描画の有効無効を設定する関数
	* @param name 設定する子スプライトの名前
	* @param flag 描画の有効無効のフラグ
	* @details 特定の子スプライトの描画の有効無効を設定する関数
	*/
	void Set_Enable_Child(const aegis::string& const name, const bool flag);

	/**
	* @brief 子スプライトの描画の有効無効を取得する関数
	* @param name 子スプライトの名前
	* @return bool 描画の有効無効のフラグ
	* @details 特定の子スプライトの描画の有効無効を取得する関数
	*/
	const bool Get_Enable_Child(const aegis::string& const name, aegis::vector< std::unique_ptr<CHILD_DATE> >* const children);
	
	void Set(ID3D11ShaderResourceView* shader_resource_view) {
		ShaderResourceView = shader_resource_view;
	};

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

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));

		archive(cereal::make_nvp("Children", Children));
		archive(cereal::make_nvp("Texture", Texture));
		archive(cereal::make_nvp("Position", SPRITE::Position));
		archive(cereal::make_nvp("Size", Size));
		archive(cereal::make_nvp("Color", Color));
		archive(cereal::make_nvp("Enable", Enable));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
		
		archive(cereal::make_nvp("Children", Children));
		archive(cereal::make_nvp("Texture", Texture));
		archive(cereal::make_nvp("Position", SPRITE::Position));
		archive(cereal::make_nvp("Size", Size));
		archive(cereal::make_nvp("Color", Color));
		archive(cereal::make_nvp("Enable", Enable));
	}

	bool flag = true;
};

CEREAL_REGISTER_TYPE(SPRITE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, SPRITE)

#endif // ! SPRITE_H