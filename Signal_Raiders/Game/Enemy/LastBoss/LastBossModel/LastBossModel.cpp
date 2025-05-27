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
	: m_pCommonResources{}// 共通リソース
	, m_bodyModel{}// 胴体
	, m_pPixelShader{}// 影用のピクセルシェーダー
	, m_nowState(IState::EnemyState::IDLING)// 現在のステート
{}

/*
*	@breif	デストラクタ
*   @return	なし
*/
LastBossModel::~LastBossModel() {}

/*
*	@breif	初期化
*	@param	CommonResources* resources
*   @return	なし
*/
void LastBossModel::Initialize(CommonResources* resources)
{
	m_pCommonResources = resources;// 共通リソース
	auto device = resources->GetDeviceResources()->GetD3DDevice();// デバイス
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");// ピクセルシェーダーの読み込み
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));// ピクセルシェーダーの作成
	m_bodyModel = m_pCommonResources->GetModelManager()->GetModel("LastBossBody");// 胴体モデルをマネージャーから取得
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceDamage");// ダメージ顔モデルをマネージャーから取得
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceAttack");// 攻撃顔モデルをマネージャーから取得
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceAngry");// 怒り顔モデルをマネージャーから取得

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
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);// ピクセルシェーダー
		});
	m_bodyModel->Draw(context, *states, world, view, proj);// 胴体
	auto it = m_pFaceModelMap.find(m_nowState);// ステートによって顔のモデルを変更
	if (it != m_pFaceModelMap.end() && it->second)// マップに存在する場合
		it->second->Draw(context, *states, world, view, proj);// 顔のモデルを描画
}
