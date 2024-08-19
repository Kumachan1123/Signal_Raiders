/*
	@file	GameOverScene.h
	@brief	ゲームオーバークラス
*/
#pragma once
#include "IScene.h"

// 前方宣言
class CommonResources;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}
class GameOverScene final :
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
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gameoverTexture;
	// 指示
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_clearTexture;
	// テクスチャの半分の大きさ
	DirectX::SimpleMath::Vector2 m_titleTexCenter;
	DirectX::SimpleMath::Vector2 m_pressKeyTexCenter;

	// シーンチェンジフラグ
	bool m_isChangeScene;
	// FMODで使用する変数（ポインタ）
	FMOD::System* m_system;	// FMODのシステム
	FMOD::Sound* m_soundSE;	// SE用の音声データ
	FMOD::Sound* m_soundBGM;	// BGM用の音声データ
	FMOD::Channel* m_channelSE;	// SEを再生するチャンネル
	FMOD::Channel* m_channelBGM;// BGMを再生するチャンネル

	// フェードで使用する変数
	bool m_isFade;		// フェードフラグ
	float m_volume;		// ボリューム
	int m_counter;		// フェードカウンタ
	float m_time = 0.0f;// 拡縮に使う時間
	float m_size = 0.0f;// 画像サイズ
	float m_cleatrSize = 0.0f;
public:
	GameOverScene();
	~GameOverScene() override;

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	void InitializeFMOD();
};
