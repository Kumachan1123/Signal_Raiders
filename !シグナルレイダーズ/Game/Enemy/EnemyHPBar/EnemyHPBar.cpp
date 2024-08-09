/*
	@file	EnemyHPBar.cpp
	@brief	敵HPBarクラス
	作成者：くまち
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Game/FPS_Camera/FPS_Camera.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/Template/Template.h"
// コンストラクタ
EnemyHPBar::EnemyHPBar()
	: m_maxHP(100), m_currentHP(100), m_displayedHP(100), m_lerpSpeed(5.0f), m_isDead(false)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ビルボードの頂点情報を設定する
	m_hpbarVert[0] = { Vector3(-1.0f,3.25f,0.0f),Vector2(0.0f,0.0f) };	// 左上
	m_hpbarVert[1] = { Vector3(1.0f,3.25f,0.0f),Vector2(1.0f,0.0f) };	// 右上
	m_hpbarVert[2] = { Vector3(-1.0f,3.0f,0.0f),Vector2(0.0f,1.0f) };	// 左下
	m_hpbarVert[3] = { Vector3(1.0f,3.0f,0.0f),Vector2(1.0f,1.0f) };	// 右下
	// ビルボードの頂点情報を設定する
	m_hpbarBackVert[0] = { Vector3(-1.10f,3.27f,0.0f),Vector2(0.0f,0.0f) };	// 左上
	m_hpbarBackVert[1] = { Vector3(1.10f,3.27f,0.0f),Vector2(1.0f,0.0f) };	// 右上
	m_hpbarBackVert[2] = { Vector3(-1.10f,2.98f,0.0f),Vector2(0.0f,1.0f) };	// 左下
	m_hpbarBackVert[3] = { Vector3(1.10f,2.98f,0.0f),Vector2(1.0f,1.0f) };	// 右下

}
// デストラクタ
EnemyHPBar::~EnemyHPBar() {}
//---------------------------------------------------------
//// 初期化する
////---------------------------------------------------------
void EnemyHPBar::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

	/*
		ビルボードの描画に関連するオブジェクトを設定する
	*/
	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetTextureEnabled(true);	// テクスチャを使用する
	/*
		・ベーシックエフェクトの「Set○○Enabled」は必要なものだけ「true」に設定する
		・不要なものは指定しなくてよい（デフォルトで「false」に設定される）
	*/

	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);
	// 画像をロードする
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/EnemyHPBar.png",
			nullptr,
			m_gaugeTexture.ReleaseAndGetAddressOf()
		)
	);
	m_displayedHP = m_maxHP;
}
// 描画
void EnemyHPBar::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();

	// ビルボード行列を作成
	Matrix billboardMatrix = view.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;

	// ビルボードをアフィン変換
	Matrix worldBillboard{};
	worldBillboard = billboardMatrix;
	worldBillboard *= Matrix::CreateTranslation(Vector3(0.0f, -2.0f, 0.0f));
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(-rot.y * 2.0f));	// ③
	worldBillboard *= Matrix::CreateTranslation(pos);
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(rot.y * 2.0f));	// ③


	// ポリゴンを描画するための設定
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	context->RSSetState(states->CullCounterClockwise());
	context->IASetInputLayout(m_inputLayout.Get());
	// HPbar(背面)描画///////////////////////////////////////////////////////////////////////////////
	m_basicEffect->SetWorld(worldBillboard);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->SetTexture(m_gaugeTexture.Get());
	m_basicEffect->SetColorAndAlpha(DirectX::Colors::Gray);
	m_basicEffect->Apply(context);

	// ビルボードを描画
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_hpbarBackVert[0], m_hpbarBackVert[1], m_hpbarBackVert[3], m_hpbarBackVert[2]);
	m_primitiveBatch->End();
	// HPbar(緑)描画///////////////////////////////////////////////////////////////////////////////
	m_basicEffect->SetColorAndAlpha(DirectX::Colors::Lime);
	m_basicEffect->Apply(context);

	// ビルボードを描画
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_hpbarVert[0], m_hpbarVert[1], m_hpbarVert[3], m_hpbarVert[2]);
	m_primitiveBatch->End();
}
// 更新
void EnemyHPBar::Update(float elapsedTime, int currentHP)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// 現在のHPを更新
	m_currentHP = currentHP;

	// HPを滑らかに更新するための線形補間
	m_displayedHP = Lerp(m_displayedHP, static_cast<float>(m_currentHP), elapsedTime * m_lerpSpeed);

	// 表示HPに基づいてHP割合を計算
	float hpPercentage = m_displayedHP / static_cast<float>(m_maxHP);

	// HPバーの幅を計算 (例として、最大幅を2.0fとする)
	float maxBarWidth = 2.0f;
	float currentBarWidth = maxBarWidth * hpPercentage;

	// HPバーの頂点を設定 (バーの左端を -1.0f として、右端を 1.0f にする)
	m_hpbarVert[0].position.x = -1.0f;                    // 左上
	m_hpbarVert[1].position.x = -1.0f + currentBarWidth;  // 右上
	m_hpbarVert[2].position.x = -1.0f;                    // 左下
	m_hpbarVert[3].position.x = -1.0f + currentBarWidth;  // 右下

	if (m_hpbarVert[1].position.x <= -1.0f)
	{
		m_isDead = true;
	}

}
