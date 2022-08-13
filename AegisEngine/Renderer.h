#pragma once

#ifndef RENDER_H
#define RENDER_H

//#include "main.h"
#include "RendererTypeDefine.h"

namespace aegis
{
	//! 頂点構造体
	struct VERTEX_3D
	{
		Vector3 Position;
		Vector3 Normal;
		Vector4 Diffuse;
		Vector2 TexCoord;

		VERTEX_3D() : Position(Vector3(0.f, 0.f, 0.f)), Normal(Vector3(0.f, 0.f, 0.f)), Diffuse(Vector4(0.f, 0.f, 0.f, 1.0f)), TexCoord(Vector2(0.f, 0.f)) {}

		VERTEX_3D(XMFLOAT3 v, XMFLOAT3 n, XMFLOAT4 d, XMFLOAT2 t) : Position(v.x, v.y, v.z), Normal(n), Diffuse(d), TexCoord(t) {}

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("Position", Position),
				cereal::make_nvp("Normal", Normal),
				cereal::make_nvp("Diffuse", Diffuse),
				cereal::make_nvp("TexCoord", TexCoord)
			);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("Position", Position),
				cereal::make_nvp("Normal", Normal),
				cereal::make_nvp("Diffuse", Diffuse),
				cereal::make_nvp("TexCoord", TexCoord)
			);
		}
	};

	//! 頂点構造体
	struct VERTEX_3D_NORMAL
	{
		Vector3 Position;
		Vector3 Normal;
		Vector3 Binormal;
		Vector3 Tangent;
		Vector4 Diffuse;
		Vector2 TexCoord;

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("Position", Position),
				cereal::make_nvp("Normal", Normal),
				cereal::make_nvp("Binormal", Binormal),
				cereal::make_nvp("Tangent", Tangent),
				cereal::make_nvp("Diffuse", Diffuse),
				cereal::make_nvp("TexCoord", TexCoord)
			);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("Position", Position),
				cereal::make_nvp("Normal", Normal),
				cereal::make_nvp("Binormal", Binormal),
				cereal::make_nvp("Tangent", Tangent),
				cereal::make_nvp("Diffuse", Diffuse),
				cereal::make_nvp("TexCoord", TexCoord)
			);
		}
	};

	//! マテリアル構造体
	struct MATERIAL
	{
		COLOR Ambient;
		COLOR Diffuse;
		COLOR Specular;
		COLOR Emission;
		float Shininess;
		float Dummy[3];//16bit境界用
	};




	//! マテリアル構造体
	struct DX11_MODEL_MATERIAL
	{
		MATERIAL		Material;
		//class CTexture*	Texture;
	};


	//! 描画サブセット構造体
	struct DX11_SUBSET
	{
		unsigned short	StartIndex;
		unsigned short	IndexNum;
		DX11_MODEL_MATERIAL	Material;
	};

	struct LIGHT
	{
		Vector4 Direction;
		Vector4 Position;
		COLOR Diffuse;
		COLOR Ambient;
		COLOR Specular;

		LIGHT() : Position(Vector4(20.0f, 20.0f, -20.0f, 0.0f)), Diffuse(COLOR(1.0f, 1.0f, 1.0f, 1.0f)), Ambient(COLOR(1.0f, 1.0f, 1.0f, 1.0f)), Specular(COLOR(1.0f, 1.0f, 1.0f, 1.0f))
		{
			Direction = (this->Position * -1.0f);
			Direction.Normalize();
		}
	};

	//struct TEXT_FOMAT{
	//	string FontName;
	//	float FontSize = 32.0f;
	//	DWRITE_FONT_WEIGHT Weight = DWRITE_FONT_WEIGHT_NORMAL;
	//	DWRITE_FONT_STYLE Style = DWRITE_FONT_STYLE_NORMAL;
	//	DWRITE_FONT_STRETCH Stretch = DWRITE_FONT_STRETCH_NORMAL;
	//};
	//
	//struct TEXT_LAYOUT {
	//	string Text;
	//	float Width;
	//	float Height;
	//};

	enum class SHADER_INDEX_V {
		DEFAULT,
		ANIMATION,
		SKYBOX,
		SHADOW_MAP,
		SHADOW_MAP_ANIMATION,
		INSTANCING,
		DEPTH_PRE,
		DEPTH_PRE_ANIME,
		MAX,
	};

	enum class SHADER_INDEX_P {
		DEFAULT,
		NO_TEXTURE,
		NO_LIGHT,
		SKYBOX,
		SHADOW_MAP,
		POST_EFFECT,
		MAX
	};

	struct CONSTANT {
		XMMATRIX WorldMatrix;
		XMMATRIX ViewMatrix;
		XMMATRIX ProjectionMatrix;
	};

	struct CONSTANT_02 {
		XMFLOAT4 Camera_Pos;	// カメラの座標
	};

	enum INPUTLAYOUT {
		DEFAULT = 0,
		ANIMATION,
		INSTANCING,
		MAX
	};

	/**
	* @brief Directxのデバイスクラス
	* @details Directx11のデバイスクラス
	*/
	class CRenderer {
	public:

		CRenderer() {}
		virtual ~CRenderer() {}

		static CRenderer* getInstance();

		virtual bool Init() = 0;
		virtual void Uninit() = 0;
		virtual void Begin() = 0;
		virtual void End() = 0;

		// 自前
		virtual void SetBlendState(BlendDesc* blendState = nullptr, bool flag = false) = 0;
		virtual void GetAddBlendState(BlendDesc& blendState) = 0;
		virtual void GetSubtractBlendState(BlendDesc& blendState) = 0;
		virtual void Get2DBlendState(BlendDesc& blendState) = 0;
		virtual void SetDepthEnable(bool Enable) = 0;
		void Change_Window_Mode();
		
		//**
		/* @brief 2D用のワールドマトリックス設定
		/* @param[in] scaling　拡大縮小のマトリックス
		/* @param[in] rotation　回転のマトリックス
		/* @details 2D用のワールドマトリックス設定を設定する
		/*/
		virtual void SetWorldViewProjection2D(const XMFLOAT3& scaling = XMFLOAT3(1.0f, 1.0f, 1.0f), const XMFLOAT3& rotation = XMFLOAT3(0.0f, 0.0f, 0.0f)) = 0;
		
		//**
		/* @brief コンスタントバッファ設定
		/* @param[in] world　ワールドマトリックス
		/* @param[in] view　ビューマトリックス
		/* @param[in] projection　プロジェクションマトリックス
		/* @details コンスタントバッファを設定する
		/*/
		virtual void Set_MatrixBuffer(const XMMATRIX world, const XMMATRIX view, const XMMATRIX projection) = 0;
		
		virtual void Set_MatrixBuffer01(const XMVECTOR camera_pos) = 0;
		
		//**
		/* @brief マテリアル設定
		/* @param[in] Material　マテリアル
		/* @details マテリアル設定を設定する
		/*/
		virtual void SetMaterial(MATERIAL Material) = 0;
		
		//**
		/* @brief ディレクショナルライト設定
		/* @param[in] Light　ディレクショナルライト
		/* @details ディレクショナルライト設定を設定する
		/*/
		virtual void SetLight(LIGHT* Light) = 0;
		
		//**
		/* @brief ディレクショナルライトの初期化
		/* @details ディレクショナルライト設定を初期化する
		/*/
		virtual void Light_Identity() = 0;
		
		//**
		/* @brief ラスタライズステートの設定
		/* @details 
		/*/
		virtual void Set_RasterizerState() = 0;
		
		virtual DepthStencilView* CreateDepthStencilView(Texture2D* texture, DepthStencilViewDesc depthStencilViewDesc) const = 0;

		virtual void SetRenderTargets(uint32 numViews, RenderTargetView* const* renderTargetViews, DepthStencilView* depthStencilView) const = 0;

		virtual void ClearRenderTargetView(RenderTargetView* renderTargetView, float32 color[4]) const = 0;

		virtual void ClearDepthStencilView(DepthStencilView* depthStencilView, ClearFlag clearFlags, float32 depth, uint8 stencil) const = 0;

		virtual void SetViewports(uint32 numViewports, ViewPort* const* viewports) const = 0;

		virtual RasterizerState* CreateRasterizeState(RasterizeState rasterizeState) const = 0;

		virtual void SetRasterizeState(RasterizerState* rasterizerState) const = 0;

		//**
		/* @brief バッファ作成
		/* @param[in] desc　バッファリソース定義
		/* @param[in] initData　初期化データ
		/* @return Buffer* 作成されたバッファ
		/* @details バッファリソース定義に基づいたバッファを作成する
		/*/
		virtual Buffer* CreateBuffer(const BufferDesc& desc, const SubresourceData initData) = 0;

		//**
		/* @brief 1Dテクスチャの配列を作成
		/* @param[in] desc　1Dテクスチャリソース定義
		/* @param[in] initData　初期化データ
		/* @return Texture1D 作成された1Dテクスチャリソース
		/* @details 1Dテクスチャの配列を作成します
		/*/
		virtual Texture1D* CreateTexture1D(Texture1DDesc desc, SubresourceData* initialData) const = 0;

		//**
		/* @brief 2Dテクスチャの配列を作成
		/* @param[in] desc　2Dテクスチャリソース定義
		/* @param[in] initData　初期化データ
		/* @return Texture2D 作成された2Dテクスチャリソース
		/* @details 2Dテクスチャの配列を作成します
		/*/
		virtual Texture2D* CreateTexture2D(Texture2DDesc desc, SubresourceData* initialData) const = 0;

		//**
		/* @brief 3Dテクスチャの配列を作成
		/* @param[in] desc　3Dテクスチャリソース定義
		/* @param[in] initData　初期化データ
		/* @return Texture2D 作成された3Dテクスチャリソース
		/* @details 3Dテクスチャの配列を作成します
		/*/
		virtual Texture3D* CreateTexture3D(Texture3DDesc desc, SubresourceData* initialData) const = 0;

		virtual ShaderResourceView* CreateShaderResourceView(Texture* texture, ShaderResourceViewDesc srvDesc) const = 0;

		virtual UnorderedAccessView* CreateUnorderedAccessView(Texture* texture, UnorderedAccessViewDesc* unorderedAccessViewDesc) const = 0;

		virtual void ClearUnorderedAccessView(UnorderedAccessView* unorderedAccessView, float32 clearValues[4]) const = 0;
		virtual void ClearUnorderedAccessView(UnorderedAccessView* unorderedAccessView, uint32 clearValues[4]) const = 0;

		virtual void CSSetUnorderedAccessViews(uint32 startSlot, uint32 numBuffers, UnorderedAccessView* const* unorderedAccessViews) const = 0;

		//**
		/* @brief 頂点バッファの設定
		/* @details 頂点バッファ設定を設定する
		/*/
		virtual void SetVertexBuffers(Buffer* vertexBuffer, uint32 stride = sizeof(VERTEX_3D)) = 0;
		
		//**
		/* @brief 頂点バッファの設定(インスタンシング用)
		/* @details インスタンシング用の頂点バッファ設定を設定する
		/*/
		virtual void SetVertexBuffers(Buffer* indexBuffer, Buffer* instancingBuffer, UINT size) = 0;
		
		//**
		/* @brief インデックスバッファの設定
		/* @details インデックスバッファ設定を設定する
		/*/
		virtual void SetIndexBuffer(Buffer* indexBuffer, Format format = Format::R16Uint) const = 0;
		
		virtual PixelShader* CreatePixelShader(void* shaderData, uint32 size) const = 0;
		virtual VertexShader* CreateVertexShader(void* shaderData, uint32 size) const = 0;
		virtual ComputeShader* CreateComputeShader(void* shaderData, uint32 size) const = 0;

		virtual void Draw(uint32 indexCount, uint32 startVertexLocation) = 0;
		virtual void DrawIndexed(uint32 indexCount, uint32 startIndexLocation, int32 baseVertexLocation ) = 0;
		virtual void DrawIndexedInstanced(int32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32 baseVertexLocation, uint32 startInstanceLocation) = 0;

		virtual void Dispatch(uint32 threadGroupCountX, uint32 threadGroupCountY, uint32 threadGroupCountZ) const = 0;

		virtual void CSSetShader(ComputeShader* pShader) const = 0;

		//**
		/* @brief 頂点シェーダーで使用される定数バッファーを設定
		/* @param[in] startSlot 0から始まる配列にインデックスを付けて、定数バッファーの設定します
		/* @param[in] numBuffers 設定するバッファーの数
		/* @param[in] pBuffers 定数バッファーリソースの配列
		/* @details 頂点シェーダーパイプラインステージで使用される定数バッファーを設定する
		/*/
		virtual void VSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers) = 0;

		//**
		/* @brief ピクセルシェーダーで使用される定数バッファーを設定
		/* @param[in] startSlot 0から始まる配列にインデックスを付けて、定数バッファーの設定します
		/* @param[in] numBuffers 設定するバッファーの数
		/* @param[in] pBuffers 定数バッファーリソースの配列
		/* @details ピクセルシェーダーパイプラインステージで使用される定数バッファーを設定する
		/*/
		virtual void PSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers) = 0;

		//**
		/* @brief コンピュートシェーダーで使用される定数バッファーを設定
		/* @param[in] startSlot 0から始まる配列にインデックスを付けて、定数バッファーの設定します
		/* @param[in] numBuffers 設定するバッファーの数
		/* @param[in] pBuffers 定数バッファーリソースの配列
		/* @details コンピュートシェーダーパイプラインステージで使用される定数バッファーを設定する
		/*/
		virtual void CSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers) = 0;

		virtual void PSSetShaderResources(uint32 startSlot, uint32 numBuffers, ShaderResourceView* const* shaderResourceViews) const = 0;

		virtual void CSSetShaderResources(uint32 startSlot, uint32 numBuffers, ShaderResourceView* const* shaderResourceViews) const = 0;

		virtual SamplerState* CreateSampler(const SamplerDesc desc) const = 0;

		//**
		/* @brief サンプラー状態の配列をピクセルシェーダーに設定します
		/* @param[in] startSlot 0から始まる配列にインデックスを付けて、サンプラーの設定します
		/* @param[in] numBuffers 設定するサンプラーの数
		/* @param[in] pBuffers サンプラーの配列
		/* @details サンプラー状態の配列をピクセルシェーダーパイプラインステージに設定します
		/*/
		virtual void PSSetSamplers(UINT startSlot, UINT numSamplers, SamplerState* const* ppSamplers) = 0;

		//**
		/* @brief サンプラー状態の配列をコンピュートシェーダーに設定します
		/* @param[in] startSlot 0から始まる配列にインデックスを付けて、サンプラーの設定します
		/* @param[in] numBuffers 設定するサンプラーの数
		/* @param[in] pBuffers サンプラーの配列
		/* @details サンプラー状態の配列をコンピュートシェーダーパイプラインステージに設定します
		/*/
		virtual void CSSetSamplers(UINT startSlot, UINT numSamplers, SamplerState* const* ppSamplers) = 0;

		//**
		/* @brief プリミティブトポロジーの設定
		/* @param[in] primitiveTopology プリミティブトポロジー
		/* @details プリミティブ型に関する情報の設定
		/*/
		virtual void SetPrimitiveTopology(PrimitiveTopology primitiveTopology) = 0;
		
		//**
		/* @brief リソースに対するCPUデータでの更新を行う
		/* @param[in] bufferリソース
		/* @param[in] pData 更新（上書き）するCPUデータポインタ
		/* @details リソースに対するCPUデータでの更新（上書き）を行う
		/*/
		virtual void UpdateSubresource(Buffer* buffer, void* pData) = 0;

		//**
		/* @brief リソースに対するCPUデータでの上書きを行う
		/* @param[in] buffer リソース
		/* @param[in] pData 上書きするCPUデータポインタ
		/* @param[in] dataSize 上書きするCPUデータサイズ
		/* @details リソースに対する、ロックをしてCPUデータでの上書きを行う
		/*/
		virtual void Map(Buffer* buffer, void* pData, uint32 dataSize) = 0;

		//**
		/* @brief リソースに対する上書きのためのロックをする
		/* @param[in] buffer リソース
		/* @return MappedSubresource デバイスコンテキストのポインタ
		/* @details リソースに対する、CPUデータでの上書きのためのロックを行う
		/*/
		virtual MappedSubresource Map(Buffer* buffer) = 0;

		//**
		/* @brief リソースに対する書き込み・読み込みロックを解除する
		/* @param[in] buffer　ロックを解除するリソース
		/* @details リソースに対する書き込み・読み込みロックを解除する
		/*/
		virtual void Unmap(Buffer* buffer) = 0;
		
		virtual void Set_Shader(const SHADER_INDEX_V v_index = SHADER_INDEX_V::DEFAULT, const SHADER_INDEX_P p_index = SHADER_INDEX_P::DEFAULT) = 0;
		
		virtual void Set_InputLayout(const INPUTLAYOUT v_index = INPUTLAYOUT::DEFAULT) = 0;
		
		virtual void SetPassRendring() = 0;
		virtual void SetPassGeometry() = 0;
		
		virtual void End_Draw() = 0;

		//ID3D11ShaderResourceView* Get() {
		//	return ShaderResourceView[0].Get();
		//}
		//
		//ID3D11ShaderResourceView* Get2() {
		//	return ShaderResourceView[1].Get();
		//}
		//
		//ID3D11ShaderResourceView* Get3() {
		//	return ShaderResourceView[2].Get();
		//}
		//
		//ID3D11ShaderResourceView* Get_Texture() {
		//	return ShaderResourceView_16bit.Get();
		//}
		
		LIGHT* GetLight() { return &m_Light; };

		RenderApiType GetRenderApiType() const { return mType; }

	protected:
		RenderApiType mType;

	private:

		// ライト
		LIGHT	m_Light;

		/**
		* @brief Direct3Dの初期化
		* @return bool 成功なら true 失敗なら false
		* @details Direct3Dの初期化する
		*/
		virtual bool Init3D() = 0;

		/**
		* @brief Direct2Dの初期化
		* @return bool 成功なら true 失敗なら false
		* @details Direct2Dの初期化する
		*/
		virtual bool Init2D() = 0;
	};

	void* Cast(aegis::ShaderResourceView* shaderResourceView);
}

#endif // !RENDER_H