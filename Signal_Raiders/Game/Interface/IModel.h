#pragma once
#ifndef IMODEL_DEFINED
#define IMODEL_DEFINED

#include <SimpleMath.h>
#include "Game/Interface/IState.h"
#include <Model.h>
class CommonResources;
class IState;
class IModel
{
public:

	// デストラクタ
	virtual ~IModel() = default;

	// 初期化
	virtual void Initialize(CommonResources* resources) = 0;
	// 描画
	virtual	void SetState(IState::EnemyState State) = 0;
	virtual void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj) = 0;
};
#endif//IMODEL_DEFINED