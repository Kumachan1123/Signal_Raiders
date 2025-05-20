/*
	@file	BossModel.cpp
	@brief	敵モデルクラス
*/
#include "pch.h"
#include "BossModel.h"
/*
*	@breif	コンストラクタ
*	@return	なし
*/
BossModel::BossModel()
	: m_commonResources{}// 共通リソース
	, m_bodyModel{}// 胴体
	, m_pixelShader{}// 影用のピクセルシェーダー
	, m_nowState(IState::EnemyState::IDLING)// 現在のステート
{}

/*
*	@breif	デストラクタ
*	@return	なし
*/
BossModel::~BossModel() {/*do nothing*/ }

/*
*	@breif	初期化
*	@param[in]	CommonResources* resources 共通リソース
*	@return	なし
*/
void BossModel::Initialize(CommonResources* resources)
{
	m_commonResources = resources;// 共通リソース
	auto device = resources->GetDeviceResources()->GetD3DDevice();// デバイス
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");// ピクセルシェーダーの読み込み
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));// ピクセルシェーダーの作成
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);// エフェクトファクトリー
	fx->SetDirectory(L"Resources/Models/Boss");// モデルのディレクトリ
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss.cmo", *fx);// 胴体
	m_faceModelMap[IState::EnemyState::HIT] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Damage.cmo", *fx);//ダメージを受けた時の顔
	m_faceModelMap[IState::EnemyState::ATTACK] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Attack.cmo", *fx);// 攻撃時の顔
	m_faceModelMap[IState::EnemyState::ANGRY] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Angry.cmo", *fx);// 怒り状態の顔
}
/*
*	@brief	描画
*	@param[in]	context  デバイスコンテキスト
*	@param[in]	states	コモンステート
* 　@param[in]	world	ワールド行列
*	@param[in]	view	ビュー行列
*	@param[in]	proj	プロジェクション行列
*	@return	なし
*/
void BossModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
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
	auto it = m_faceModelMap.find(m_nowState);// ステートによって顔のモデルを変更
	if (it != m_faceModelMap.end() && it->second)// マップに存在する場合
		it->second->Draw(context, *states, world, view, proj);// 顔のモデルを描画
}
