/*
	@file	AreaAttackerModel.cpp
	@brief	範囲攻撃敵モデルクラス
*/
#include "pch.h"
#include "Game/Enemy/AreaAttacker/AreaAttacker.h"
#include "Game/Enemy/AreaAttackerModel/AreaAttackerModel.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <SimpleMath.h>

// コンストラクタ
AreaAttackerModel::AreaAttackerModel()
	:
	m_commonResources{},
	m_bodyModel{},
	m_attackFaceModel{},
	m_idlingFaceModel{},
	m_nowState{ IState::EnemyState::IDLING }
{}

// デストラクタ
AreaAttackerModel::~AreaAttackerModel() {}

// 初期化する
void AreaAttackerModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/AreaAttacker");
	// モデルを読み込む（ 頭、表情差分）
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/AreaAttacker/AreaAttacker.cmo", *fx);
	fx->SetDirectory(L"Resources/Models/Enemy");
	m_attackFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Face.cmo", *fx);
	m_idlingFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *fx);
	m_angryFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *fx);
	m_damageFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *fx);


}
void AreaAttackerModel::Update(float elapsedTime, IState::EnemyState State)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_nowState = State;
}

void AreaAttackerModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_bodyModel->Draw(context, *states, world, view, proj);// 頭
	// 表情差分
	switch (m_nowState)
	{
	case IState::EnemyState::IDLING:// アイドリング
		m_idlingFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::ATTACK:// 攻撃
		m_attackFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::ANGRY:// 怒り
		m_angryFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::DAMAGE:// 被弾
		m_damageFaceModel->Draw(context, *states, world, view, proj);
		break;

	}
}