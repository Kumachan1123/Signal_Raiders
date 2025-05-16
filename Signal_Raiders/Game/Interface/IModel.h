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
class IModel
{
public:
	// public関数
	virtual ~IModel() = default;	// デストラクタ
	virtual void Initialize(CommonResources* resources) = 0;	// 初期化
	virtual	void SetState(IState::EnemyState State) = 0;	// ステート設定
	virtual void Render(ID3D11DeviceContext1* context,	// 描画
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj) = 0;
};
#endif//IMODEL_DEFINED