/*
	@file	Radar.cpp
	@brief	レーダークラス
	作成者：くまち
*/
#include "pch.h"
#include "Radar.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
Radar::Radar(CommonResources* commonResources)
	: m_commonResources{ commonResources },
	m_pPlayer{ nullptr },
	m_pEnemies{ nullptr },
	m_radarPos{ 0.745f, -0.55f },
	m_playerSize{ 0.018f, -0.032f },
	m_enemySize{ 0.009f, -0.016f }

{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
Radar::~Radar()
{
}

//---------------------------------------------------------
// 初期化
//---------------------------------------------------------
void Radar::Initialize(Player* pPlayer, Enemies* pEnemies)
{
	m_pPlayer = pPlayer;
	m_pEnemies = pEnemies;
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_commonResources->GetDeviceResources()->GetD3DDeviceContext());
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(m_commonResources->GetDeviceResources()->GetD3DDevice());
	// エフェクトの設定
	m_basicEffect->SetVertexColorEnabled(true);
	// 入力レイアウトの作成
	// InputLayoutの作成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_commonResources->GetDeviceResources()->GetD3DDevice()->CreateInputLayout(
		VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.GetAddressOf()
	);

}

//---------------------------------------------------------
// 更新
//---------------------------------------------------------
void Radar::Update()
{
	// プレイヤーの位置を取得
	m_playerPos = m_pPlayer->GetPlayerPos();
	// 敵の位置をクリア
	m_enemyPos.clear();
	// 敵の位置を取得
	for (auto& enemy : m_pEnemies->GetEnemy())
	{
		Vector3 enemyPos = enemy->GetPosition();
		float distance = Vector3::Distance(m_playerPos, enemyPos);

		// レーダー範囲内にいる敵だけ保存
		if (distance <= m_range)
		{
			m_enemyPos.push_back(enemyPos);

		}
	}

}

//---------------------------------------------------------
// 描画
//---------------------------------------------------------
void Radar::Render()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// バッチの開始
	m_primitiveBatch->Begin();
	m_basicEffect->Apply(context);
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	context->RSSetState(states->CullNone());

	context->IASetInputLayout(m_inputLayout.Get());

	// レーダーの背景を描画
	m_primitiveBatch->DrawQuad(
		VertexPositionColor(Vector3(0.5f, -.16f, 0.0f), Vector4(0.0, 0.0, 0.0, 0.6)),
		VertexPositionColor(Vector3(1.0f, -.16f, 0.0f), Vector4(0.0, 0.0, 0.0, 0.6)),
		VertexPositionColor(Vector3(1.0f, -1.0f, 0.0f), Vector4(0.0, 0.0, 0.0, 0.6)),
		VertexPositionColor(Vector3(0.5f, -1.0f, 0.0f), Vector4(0.0, 0.0, 0.0, 0.6))
	);
	// バッチの終了
	m_primitiveBatch->End();

	// バッチの開始
	m_primitiveBatch->Begin();


	// プレイヤー位置をレーダー中心に描画
	// レーダーの背景を描画した時の値から中心を計算し、プレイヤーの位置を描画
	m_basicEffect->Apply(context);
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());

	m_primitiveBatch->DrawQuad(
		VertexPositionColor(Vector3(m_radarPos.x, m_radarPos.y, 0.0f), Colors::Lime),
		VertexPositionColor(Vector3(m_radarPos.x + m_playerSize.x, m_radarPos.y, 0.0f), Colors::Lime),
		VertexPositionColor(Vector3(m_radarPos.x + m_playerSize.x, m_radarPos.y + m_playerSize.y, 0.0f), Colors::Lime),
		VertexPositionColor(Vector3(m_radarPos.x, m_radarPos.y + m_playerSize.y, 0.0f), Colors::Lime)
	);
	// バッチの終了
	m_primitiveBatch->End();




	// 敵の位置を描画	 
	for (const auto& enemyPos : m_enemyPos)
	{
		Vector3 relativePos = enemyPos - m_playerPos;// プレイヤーから敵への相対位置を計算


		// カメラの向きを取得
		Vector3 cameraDirection = m_pPlayer->GetPlayerDir(); // カメラの向き（前方ベクトル）

		// カメラの向きに基づいて回転行列を作成
		float playerRotation = atan2(cameraDirection.x, cameraDirection.z); // カメラの前方ベクトルから回転角を計算
		Matrix rotationMatrix = Matrix::CreateRotationZ(XMConvertToRadians(180));// 回転を調整
		rotationMatrix *= Matrix::CreateRotationY(playerRotation); // Y軸回転

		// 敵の位置をプレイヤーの位置を軸にして回転
		Vector3 rotatedPos = Vector3::Transform(relativePos, rotationMatrix);
		// レーダーの中心からの相対位置を計算
		Vector2 radarPos = Vector2(
			rotatedPos.x / m_range * (1.0f - 0.5f), // ミニマップの横幅
			rotatedPos.z / m_range * (1.0f - 0.16f) // ミニマップの高さ
		);
		radarPos += m_radarPos;

		// ミニマップの範囲内にいるかを確認
		if (radarPos.x > 0.5f + m_enemySize.x && radarPos.x < 1.0f + m_enemySize.x &&
			radarPos.y > -1.0f + m_enemySize.y && radarPos.y < -0.16f + m_enemySize.y)
		{
			m_basicEffect->Apply(context);
			context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
			context->OMSetDepthStencilState(states->DepthDefault(), 0);
			context->RSSetState(states->CullNone());
			context->IASetInputLayout(m_inputLayout.Get());
			// バッチの開始
			m_primitiveBatch->Begin();
			// 敵の位置を描画
			m_primitiveBatch->DrawQuad(
				VertexPositionColor(Vector3(radarPos.x, radarPos.y, 0.0f), Colors::Red),
				VertexPositionColor(Vector3(radarPos.x + 0.01f, radarPos.y, 0.0f), Colors::Red),
				VertexPositionColor(Vector3(radarPos.x + 0.01f, radarPos.y + 0.02f, 0.0f), Colors::Red),
				VertexPositionColor(Vector3(radarPos.x, radarPos.y + 0.02f, 0.0f), Colors::Red)
			);
			// バッチの終了
			m_primitiveBatch->End();
		}


	}



}