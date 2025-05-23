/*
*	@file Stage.h
*	@brief ステージクラス
*/
#pragma once

// 前方宣言
class CommonResources;
class Stage
{
public:// 定数
	static const float STAGE_SIZE;	// ステージの広さ
	static const float STAGE_HEIGHT;	// ステージの高さ
private:

	// 共通リソース
	CommonResources* m_commonResources;

	// モデルの描画で使用する
	std::unique_ptr<DirectX::Model> m_model;	// モデル

	// テクスチャパス
	wchar_t m_texturePath[256];
public:
	Stage();
	~Stage();

	void Initialize(CommonResources* resources);

	void Render(DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Vector3 pos
	);
	//	関数
};