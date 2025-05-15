/*
	@file	EnemyModel.cpp
	@brief	敵モデルクラス
*/
#include "pch.h"
#include "EnemyModel.h"
/*
*	@brief	コンストラクタ
*	@param[in]	なし
*	@return	なし
*/
EnemyModel::EnemyModel()
	: m_commonResources{}// 共通リソース
	, m_bodyModel{}// 胴体モデル
	, m_antennaModel{}// アンテナモデル
	, m_attackFaceModel{}	// 攻撃態勢の顔モデル
	, m_handModel{}// 手モデル
	, m_idlingFaceModel{}// 普段の顔モデル
	, m_angryFaceModel{}// おこ顔モデル
	, m_damageFaceModel{}// 攻撃を受けた時の顔モデル
	, m_shadowModel{}// 影用のモデル
	, m_nowState{ IState::EnemyState::IDLING }
{}
/*
*	@brief	デストラクタ
*	@param[in]	なし
*	@return	なし
*/
EnemyModel::~EnemyModel() {/*do nothing*/ }


/*
*	@brief	初期化
*	@param[in]	CommonResources* resources 共通リソース
*	@return	なし
*/
void EnemyModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;// 共通リソース取得
	auto device = resources->GetDeviceResources()->GetD3DDevice();// デバイス取得
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");	// 影用のシェーダーを読み込む
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));// シェーダー作成
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);	// モデルを読み込む準備
	fx->SetDirectory(L"Resources/Models/Enemy");// モデルのディレクトリを指定
	// モデルを読み込む（ 頭、アンテナ、手、表情差分）
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Head.cmo", *fx);// 頭
	m_antennaModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Antenna.cmo", *fx);// アンテナ
	m_attackFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Face.cmo", *fx);// 攻撃態勢の顔
	m_handModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Hand.cmo", *fx);// 手
	m_idlingFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *fx);// 普段の顔	
	m_angryFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *fx);// おこ顔
	m_damageFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *fx);// 攻撃を受けた時の顔
	m_shadowModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);	// 影用のモデルを読み込む
}
/*
*	@brief	描画
*	@param[in]	ID3D11DeviceContext1* context コンテキスト
*	@param[in]	DirectX::DX11::CommonStates* states ステート
*	@param[in]	DirectX::SimpleMath::Matrix world ワールド行列
*	@param[in]	DirectX::SimpleMath::Matrix view ビュー行列
*	@param[in]	DirectX::SimpleMath::Matrix proj プロジェクション行列
*	@return	なし
*/
void EnemyModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_bodyModel->Draw(context, *states, world, view, proj);// 頭
	m_antennaModel->Draw(context, *states, world, view, proj);// アンテナ
	m_handModel->Draw(context, *states, world, view, proj);	// 手
	Vector3 lightDir = Vector3::UnitY;	// ライトの方向
	lightDir.Normalize();// 正規化
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));	// 影行列の元を作る
	shadowMatrix = world * shadowMatrix;	// 影の行列をワールド行列にかける
	m_shadowModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// 影描画
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// ブレンドステートを不透明にする
			context->OMSetDepthStencilState(states->DepthNone(), 0);	// 深度ステンシルステートを無効にする
			context->RSSetState(states->CullNone());	// カリングを無効にする
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);// ピクセルシェーダーをセット
		});
	switch (m_nowState)	// 表情差分
	{
	case IState::EnemyState::IDLING:// 徘徊
		m_idlingFaceModel->Draw(context, *states, world, view, proj);// 描画
		break;
	case IState::EnemyState::ATTACK:// 攻撃
		m_attackFaceModel->Draw(context, *states, world, view, proj);// 描画
		break;
	case IState::EnemyState::ANGRY:// 怒り
		m_angryFaceModel->Draw(context, *states, world, view, proj);// 描画
		break;
	case IState::EnemyState::HIT:// 被弾中
		m_damageFaceModel->Draw(context, *states, world, view, proj);// 描画
		break;

	}
}