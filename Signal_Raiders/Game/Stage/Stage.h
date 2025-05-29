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
	Stage();// コンストラクタ
	~Stage();// デストラクタ
	void Initialize(CommonResources* resources);// 初期化
	void Render(DirectX::SimpleMath::Matrix view,// 描画
		DirectX::SimpleMath::Matrix proj,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Vector3 pos
	);
public:// 定数
	static const float STAGE_SIZE;	// ステージの広さ
	static const float STAGE_HEIGHT;	// ステージの高さ
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// モデルポインタ
	DirectX::Model* m_pModel;
};