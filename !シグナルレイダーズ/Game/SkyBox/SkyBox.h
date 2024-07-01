/*
	@file	SkyBox.h
	@brief	スカイボックスクラス
	作成者：くまち
*/
#pragma once

// 前方宣言
class CommonResources;
class SkyBox
{
private:
	 
	// 共通リソース
	CommonResources* m_commonResources;
 
	// モデルの描画で使用する
	std::unique_ptr<DirectX::Model> m_model;	// モデル
public:
	SkyBox();
	~SkyBox();

	void Initialize(CommonResources* resources) ;

	void Render(DirectX::SimpleMath::Matrix view,
				DirectX::SimpleMath::Matrix proj,
				DirectX::SimpleMath::Matrix world,
				DirectX::SimpleMath::Vector3 pos
	);
	//	関数
};