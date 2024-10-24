/*
	@file	Sky.h
	@brief	スカイクラス
	作成者：くまち
*/
#pragma once

// 前方宣言
class CommonResources;
class Sky
{
private:

	// 共通リソース
	CommonResources* m_commonResources;

	// モデルの描画で使用する
	std::unique_ptr<DirectX::Model> m_model;	// モデル

	// テクスチャパス
	wchar_t m_texturePath[256];

	// ステージID
	int m_stageID;
public:
	Sky(int StageID);
	~Sky();

	void Initialize(CommonResources* resources);

	void Render(DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Vector3 pos
	);
	//	関数
};