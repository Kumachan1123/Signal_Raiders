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
	// コンストラクタ
	Sky(int StageID);
	// デストラクタ
	~Sky();
	// 初期化
	void Initialize(CommonResources* resources);
	// 描画
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Matrix& world
	);
private:// private定数
	// ステージ番号に応じた空のモデルのKey
	const std::unordered_map<int, const std::string> m_keyMap =
	{
		{0, "Stage1"},// ステージ１
		{1, "Stage2"},// ステージ２
		{2, "Stage3"},// ステージ３
		{3, "Stage4"},// ステージ４
		{4, "Stage5"} // ステージ５
	};
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// モデル
	DirectX::Model* m_pModel;
	// テクスチャパス
	wchar_t m_texturePath[256];
	// ステージID
	int m_stageID;
};