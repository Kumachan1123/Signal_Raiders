/*
*	@file IModel.h
*	@brief モデルのインターフェースクラス
*/
#pragma once
#ifndef IMODEL_DEFINED
#define IMODEL_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
#include <Model.h>
// 自作ヘッダーファイル
#include "Game/Interface/IState.h"

// 前方宣言
class CommonResources;
class IState;

// モデルのインターフェースクラス
class IModel
{
public:
	// public関数
	// デストラクタ
	virtual ~IModel() = default;
	// 初期化
	virtual void Initialize(CommonResources* resources) = 0;
	// ステート設定
	virtual	void SetState(IState::EnemyState State) = 0;
	// 描画
	virtual void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		const DirectX::SimpleMath::Matrix& world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj) = 0;
};
#endif//IMODEL_DEFINED