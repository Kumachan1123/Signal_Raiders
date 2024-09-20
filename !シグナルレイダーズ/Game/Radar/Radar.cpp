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
	/*m_radarPos{ 0.735f, 0.54f },
	m_radarSize{ 0.03f, 0.06f },*/
	m_radarPos{ 0.74f, -0.55f },
	m_radarSize{ 0.02f, -0.04f }

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

	// バッチの開始
	m_primitiveBatch->Begin();
	m_basicEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	// レーダーの背景を描画
	m_primitiveBatch->DrawQuad(
		VertexPositionColor(Vector3(0.5f, -.16f, 0.0f), Vector4(0.3, 0.3, 0.3, 0.6)),
		VertexPositionColor(Vector3(1.0f, -.16f, 0.0f), Vector4(0.3, 0.3, 0.3, 0.6)),
		VertexPositionColor(Vector3(1.0f, -1.0f, 0.0f), Vector4(0.3, 0.3, 0.3, 0.6)),
		VertexPositionColor(Vector3(0.5f, -1.0f, 0.0f), Vector4(0.3, 0.3, 0.3, 0.6))
	);
	// バッチの終了
	m_primitiveBatch->End();

	// バッチの開始
	m_primitiveBatch->Begin();
	// プレイヤー位置をレーダー中心に描画
	// レーダーの背景を描画した時の値から中心を計算し、プレイヤーの位置を描画
	m_basicEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	m_primitiveBatch->DrawQuad(
		VertexPositionColor(Vector3(m_radarPos.x, m_radarPos.y, 0.0f), Colors::LimeGreen),
		VertexPositionColor(Vector3(m_radarPos.x + m_radarSize.x, m_radarPos.y, 0.0f), Colors::LimeGreen),
		VertexPositionColor(Vector3(m_radarPos.x + m_radarSize.x, m_radarPos.y + m_radarSize.y, 0.0f), Colors::LimeGreen),
		VertexPositionColor(Vector3(m_radarPos.x, m_radarPos.y + m_radarSize.y, 0.0f), Colors::LimeGreen)
	);
	// バッチの終了
	m_primitiveBatch->End();



	m_basicEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	// 敵の位置を描画
// 敵の位置を描画
	for (const auto& enemyPos : m_enemyPos)
	{
		Vector3 relativePos = enemyPos - m_playerPos;

		// レーダー範囲内の敵のみ描画
		if (relativePos.Length() <= m_range)
		{
			// レーダーの中心からの相対位置を計算
			Vector2 radarPos = Vector2(
				relativePos.x / m_range * (1.0f - 0.5f), // ミニマップの横幅
				relativePos.z / m_range * (1.0f - 0.16f) // ミニマップの高さ
			);

			// バッチの開始
			m_primitiveBatch->Begin();
			// 敵の位置を描画
			m_primitiveBatch->DrawQuad(
				VertexPositionColor(Vector3(m_radarPos.x + radarPos.x, m_radarPos.y + radarPos.y, 0.0f), Colors::Red),
				VertexPositionColor(Vector3(m_radarPos.x + radarPos.x + 0.01f, m_radarPos.y + radarPos.y, 0.0f), Colors::Red),
				VertexPositionColor(Vector3(m_radarPos.x + radarPos.x + 0.01f, m_radarPos.y + radarPos.y + 0.02f, 0.0f), Colors::Red),
				VertexPositionColor(Vector3(m_radarPos.x + radarPos.x, m_radarPos.y + radarPos.y + 0.02f, 0.0f), Colors::Red)
			);
			// バッチの終了
			m_primitiveBatch->End();
		}
	}


}