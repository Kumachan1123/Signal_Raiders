/*
*	@brief	スカイクラス
*	@file	Sky.h
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <memory>
#include <locale> 
#include <codecvt>
#include <unordered_map>
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include "DeviceResources.h"
// 外部ライブラリ
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
// 前方宣言
class CommonResources;
class Sky
{
public:// public関数
	Sky(int StageID);// コンストラクタ
	~Sky();// デストラクタ
	void Initialize(CommonResources* resources);// 初期化
	void Render(DirectX::SimpleMath::Matrix view,// 描画
		DirectX::SimpleMath::Matrix proj,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Vector3 pos
	);
private:
	std::wstring ConvertToWString(const std::string& str);// 文字列変換
private:// private定数
	const std::unordered_map<int, const std::string> m_texturePathMap =// ステージ番号に応じた空のモデルのパス
	{
		{0, "Resources/models/sky/sky.cmo"},// ステージ１
		{1, "Resources/models/sky/CloudySky.cmo"},// ステージ２
		{2, "Resources/models/sky/EveningSky.cmo"},// ステージ３
		{3, "Resources/models/sky/NightSky.cmo"},// ステージ４
		{4, "Resources/models/sky/MidNightSky.cmo"}// ステージ５
	};
private:// private変数
	CommonResources* m_commonResources;// 共通リソース
	std::unique_ptr<DirectX::Model> m_model;// モデル
	wchar_t m_texturePath[256];// テクスチャパス
	int m_stageID;// ステージID
};