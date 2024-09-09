/*
	@file	ClearScene.h
	@brief	クリアシーンクラス
*/
#pragma once
#include "IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/KumachiLib/AudioManager.h"
// 前方宣言
class CommonResources;
class Fade;
class BackGround;
namespace mylib
{
	class DebugCamera;
	class GridFloor;
}

class ClearScene final :
	public IScene
{
private:
	// 共通リソース
	CommonResources* m_commonResources;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// タイトル画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_clearTexture;
	// 指示
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pressKeyTexture;
	// テクスチャの半分の大きさ
	DirectX::SimpleMath::Vector2 m_clearTexCenter;
	DirectX::SimpleMath::Vector2 m_pressKeyTexCenter;

	// シーンチェンジフラグ
	bool m_isChangeScene;
	// オーディオマネージャー
	AudioManager* m_audioManager;

	// 空の行列
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	// フェードで使用する変数
	bool m_isFade;		// フェードフラグ
	float m_volume;		// ボリューム
	int m_counter;		// フェードカウンタ
	float m_time = 0.0f;// 拡縮に使う時間
	float m_size = 0.0f;// 画像サイズ
	float m_pressKeySize = 0.0f;

	// 画面遷移フェード
	std::unique_ptr<Fade> m_fade;
	// 背景
	std::unique_ptr<BackGround> m_backGround;

public:
	ClearScene();
	~ClearScene() override;

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	void InitializeFMOD();
private:
	void DrawSpace();// スペースキー押してってやつ描画
};
