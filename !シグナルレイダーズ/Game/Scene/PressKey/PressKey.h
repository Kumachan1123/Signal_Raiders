/*
	@file	PressKey.h
	@brief	指示クラス
*/
#pragma once
#include <DeviceResources.h>
// 前方宣言
class CommonResources;
class PressKey final
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pressKeyTexture;//指示画像
	// テクスチャの半分の大きさ
	DirectX::SimpleMath::Vector2 m_pressKeyTexCenter;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// 共通リソース
	CommonResources* m_commonResources;
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	float m_time = 0.0f;// 拡縮に使う時間
	float m_size = 0.0f;// 画像サイズ
public:
	PressKey(CommonResources* commonResources);//コンストラクタ
	~PressKey();//デストラクタ
	void Initialize();//初期化
	void Update(float elapsedTime);//更新
	void Render();//描画
private:
	void CreateTexture();//テクスチャの作成
};
