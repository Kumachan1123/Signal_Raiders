/*
	@file	Sky.h
	@brief	スカイクラス
*/
#pragma once
#include <unordered_map>
#include <string>
// 前方宣言
class CommonResources;
class Sky
{
private:

	// 共通リソース
	CommonResources* m_commonResources;

	// モデルの描画で使用する
	std::unique_ptr<DirectX::Model> m_model;	// モデル
	const std::unordered_map<int, const std::string> m_texturePathMap =
	{
		{0, "Resources/models/sky/sky.cmo"},
		{1, "Resources/models/sky/CloudySky.cmo"},
		{2, "Resources/models/sky/EveningSky.cmo"},
		{3, "Resources/models/sky/NightSky.cmo"},
		{4, "Resources/models/sky/MidNightSky.cmo"}
	};
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
private:
	std::wstring ConvertToWString(const std::string& str);
};