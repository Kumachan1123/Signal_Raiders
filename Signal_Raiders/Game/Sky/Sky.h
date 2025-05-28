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
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
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
private:// private定数
	const std::unordered_map<int, const std::string> m_keyMap =// ステージ番号に応じた空のモデルのKey
	{
		{0, "Stage1"},// ステージ１
		{1, "Stage2"},// ステージ２
		{2, "Stage3"},// ステージ３
		{3, "Stage4"},// ステージ４
		{4, "Stage5"} // ステージ５
	};
private:// private変数
	CommonResources* m_pCommonResources;// 共通リソース
	DirectX::Model* m_pModel;// モデル
	wchar_t m_texturePath[256];// テクスチャパス
	int m_stageID;// ステージID
	std::string Path;
};