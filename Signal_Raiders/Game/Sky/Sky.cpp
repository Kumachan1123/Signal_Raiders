/*
	@file	Sky.cpp
	@brief	スカイクラス
*/
#include "pch.h"
#include "Game/Sky/Sky.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
#include <locale> 
#include <codecvt>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
Sky::Sky(int StageID)
	:
	m_commonResources{},
	m_model{},
	m_texturePath{},
	m_stageID{ StageID }
{
}
//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
Sky::~Sky()
{
	// do nothing.
}
//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void Sky::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(resources);
	m_commonResources = resources;

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();


	// モデルを読み込む準備
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/models/sky");


	auto it = m_texturePathMap.find(m_stageID);
	if (it != m_texturePathMap.end()) {
		std::wstring wpath = ConvertToWString(it->second);
		m_model = Model::CreateFromCMO(device, wpath.c_str(), *fx);
	}


}
//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void Sky::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Vector3 pos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	world *= Matrix::CreateTranslation(pos);
	// モデルのエフェクト情報を更新する
	m_model->UpdateEffects([](DirectX::IEffect* effect)
		{
			// ベーシックエフェクトを設定する
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// 個別のライトをすべて無効化する
				basicEffect->SetLightEnabled(0, false);
				basicEffect->SetLightEnabled(1, false);
				basicEffect->SetLightEnabled(2, false);
				// モデルを自発光させる
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);
	// モデルを描画する
	m_model->Draw(context, *states, world, view, proj);
}

std::wstring Sky::ConvertToWString(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}
