/*
*	@file Stage.h
*	@brief ステージクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
// 前方宣言
class CommonResources;
class Stage
{
public:// public関数
	// コンストラクタ
	Stage();
	// デストラクタ
	~Stage();
	// 初期化
	void Initialize(CommonResources* resources);
	// 描画
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Matrix& world);
public:// 定数
	// ステージの広さ
	static const float STAGE_SIZE;
	// ステージの高さ
	static const float STAGE_HEIGHT;
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// モデルポインタ
	DirectX::Model* m_pModel;
};