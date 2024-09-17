/*
	@file	SkyBox.cpp
	@brief	スカイボックスクラス
	作成者：くまち
*/
#include "pch.h"
#include "Game/SkyBox/SkyBox.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
SkyBox::SkyBox()
	:
	m_commonResources{},
	m_model{}
{
}
//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
SkyBox::~SkyBox()
{
	// do nothing.
}
//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void SkyBox::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(resources);
	m_commonResources = resources;

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	// モデルを読み込む準備
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/sky");

	// モデルを読み込む
	m_model = Model::CreateFromCMO(device, L"Resources/Models/sky/sky.cmo", *fx);

}
//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void SkyBox::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
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
								   //// ライトを有効化する
								   //basicEffect->SetLightingEnabled(true);
								   /*
									   ・モデルクラスではデフォルトで内部的にライトが有効化されている
									   ・この設定が無くてもライトは有効化されている
								   */

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
