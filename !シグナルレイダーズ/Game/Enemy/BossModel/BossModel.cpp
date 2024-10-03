/*
	@file	BossModel.cpp
	@brief	ボスモデルクラス
	作成者：くまち
*/
#include "pch.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossModel/BossModel.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <SimpleMath.h>


BossModel::BossModel()
	:
	m_commonResources{},
	m_headModel{},
	m_antennaModel{},
	m_attackFaceModel{},
	m_handModel{},
	m_idlingFaceModel{},
	m_angryFaceModel{},
	m_damageFaceModel{},
	m_nowState{ IState::EnemyState::IDLING }
{}

BossModel::~BossModel() {}

void BossModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/Boss");
	// モデルを読み込む（ 頭、アンテナ、手、表情差分）
	m_handModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Body.cmo", *fx);
	m_idlingFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Idling.cmo", *fx);
	m_angryFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Attack.cmo", *fx);
}
void BossModel::Update(float elapsedTime, IState::EnemyState State)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_nowState = State;
}
void BossModel::Render(ID3D11DeviceContext1* context,
					   DirectX::DX11::CommonStates* states,
					   DirectX::SimpleMath::Matrix world,
					   DirectX::SimpleMath::Matrix view,
					   DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 手
	m_handModel->Draw(context, *states, world, view, proj);
	// 表情差分
	switch (m_nowState)
	{
		case IState::EnemyState::IDLING:// アイドリング
		case IState::EnemyState::DAMAGE:// ダメージ
		case IState::EnemyState::ATTACK:// 攻撃
			m_idlingFaceModel->Draw(context, *states, world, view, proj);
			break;
		case IState::EnemyState::ANGRY:// 怒り

			m_angryFaceModel->Draw(context, *states, world, view, proj);
			break;
	}
}