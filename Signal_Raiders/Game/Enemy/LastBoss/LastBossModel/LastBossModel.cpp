/*
*	@file LastBossModel.cpp
*	@brief ラスボスモデルクラス
*/

#include <pch.h>
#include "LastBossModel.h"



/*
*	@breif	コンストラクタ
*	@return	なし
*/
LastBossModel::LastBossModel()
	: m_commonResources{}// 共通リソース
	, m_bodyModel{}// 胴体
	, m_pixelShader{}// 影用のピクセルシェーダー
	, m_damageFaceModel{}// 普段の顔
	, m_attackFaceModel{}// 攻撃時の顔
	, m_nowState(IState::EnemyState::IDLING)// 現在のステート
{}

/*
*	@breif	デストラクタ
*   @return	なし
*/
LastBossModel::~LastBossModel() {}

/*
*	@breif	初期化
*	@param[in]	CommonResources* resources
*   @return	なし
*/
void LastBossModel::Initialize(CommonResources* resources)
{
	m_commonResources = resources;// 共通リソース
	auto device = resources->GetDeviceResources()->GetD3DDevice();// デバイス
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");// ピクセルシェーダーの読み込み
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));// ピクセルシェーダーの作成
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);// エフェクトファクトリー
	fx->SetDirectory(L"Resources/Models/Boss");// モデルのディレクトリ
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/LastBoss_Body.cmo", *fx);// 胴体
	m_damageFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/LastBoss_DamageFace.cmo", *fx);// ノックバック中の顔
	m_attackFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/LastBoss_Face.cmo", *fx);// 攻撃時の顔
	m_angryFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/LastBoss_AngryFace.cmo", *fx);// 怒り状態の顔
}



void LastBossModel::Render(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states, DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	Vector3 lightDir = Vector3::UnitY;// ライトの方向
	lightDir.Normalize();// 正規化
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));	// 影行列の元を作る
	shadowMatrix = world * shadowMatrix;// 影行列
	m_bodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// 影描画
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// ブレンドステート
			context->OMSetDepthStencilState(states->DepthNone(), 0);// 深度ステンシルステート
			context->RSSetState(states->CullClockwise());// ラスタライザーステート
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);// ピクセルシェーダー
		});
	m_bodyModel->Draw(context, *states, world, view, proj);// 胴体
	switch (m_nowState)// 現在のステートによって描画を変える
	{
	case IState::EnemyState::DAMAGE:// ダメージ
		m_damageFaceModel->Draw(context, *states, world, view, proj);// ノックバック中の顔
		break;
	case IState::EnemyState::ATTACK:// 攻撃
		m_attackFaceModel->Draw(context, *states, world, view, proj);// 攻撃時の顔
		break;
	case IState::EnemyState::ANGRY:// 怒り
		m_angryFaceModel->Draw(context, *states, world, view, proj);// 怒り状態の顔
		break;

	}
}
