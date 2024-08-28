/*
	@file	TitleScene.h
	@brief	タイトルシーンクラス
*/
#pragma once
#include "IScene.h"
#include <DeviceResources.h>

// 前方宣言
class CommonResources;
class Fade;
namespace mylib
{

	class GridFloor;
}
class FPS_Camera;
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}
class TitleScene final :
	public IScene
{
public:
	//	データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;//	ワールド行列
		DirectX::SimpleMath::Matrix		matView;	//	ビュー行列
		DirectX::SimpleMath::Matrix		matProj;	//	プロジェクション行列
		DirectX::SimpleMath::Vector4	Colors;
		float time;
		DirectX::SimpleMath::Vector3 padding;
	};
private:
	// 共通リソース
	CommonResources* m_commonResources;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	変数
	DX::DeviceResources* m_pDR;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// タイトル画像
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_titleTexture;
	// 指示
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pressKeyTexture;
	// 背景テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTexture;


	// テクスチャの半分の大きさ
	DirectX::SimpleMath::Vector2 m_titleTexCenter;
	DirectX::SimpleMath::Vector2 m_pressKeyTexCenter;


	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	// フェード
	std::unique_ptr<Fade> m_fade;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// FMODで使用する変数（ポインタ）
	FMOD::System* m_system;	// FMODのシステム
	FMOD::Sound* m_soundSE;	// SE用の音声データ
	FMOD::Sound* m_soundBGM;	// BGM用の音声データ
	FMOD::Channel* m_channelSE;	// SEを再生するチャンネル
	FMOD::Channel* m_channelBGM;// BGMを再生するチャンネル
	// FPSカメラ
	std::unique_ptr<FPS_Camera> m_camera;
	// フェードで使用する変数
	bool m_isFade;		// フェードフラグ
	float m_volume;		// ボリューム
	int m_counter;		// フェードカウンタ

	float m_time = 0.0f;// 拡縮に使う時間
	float m_size = 0.0f;// 画像サイズ
public:
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	TitleScene();
	~TitleScene() override;

	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR);

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	void InitializeFMOD();
private:

	void CreateShader();
	void DrawBackground();// 背景描画
	void DrawSpace();// スペースキー押してってやつ描画
	void DrawTitle();// タイトル描画
};
