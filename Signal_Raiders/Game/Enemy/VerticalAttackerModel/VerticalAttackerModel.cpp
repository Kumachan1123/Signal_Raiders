/*
	@file	VerticalAttackerModel.cpp
	@brief	範囲攻撃敵モデルクラス
*/
#include "pch.h"
#include "VerticalAttackerModel.h"

/*
*	@brief	コンストラクタ
*	@param	なし
*/
VerticalAttackerModel::VerticalAttackerModel()
	: m_commonResources{}// 共通リソース
	, m_bodyModel{}// モデル
	, m_attackFaceModel{}// 攻撃時の顔
	, m_idlingFaceModel{}// アイドリング時の顔
	, m_nowState{ IState::EnemyState::IDLING }// 現在のステータス
{}
/*
*	@brief	デストラクタ
*	@param	なし
*/
VerticalAttackerModel::~VerticalAttackerModel() {}
/*
*	@brief	初期化する
*	@param	resources	共通リソース
*	@return	なし
*/
void VerticalAttackerModel::Initialize(CommonResources* resources)
{
	m_commonResources = resources;// 共通リソース
	auto device = resources->GetDeviceResources()->GetD3DDevice();// デバイス取得
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");	// 影用のシェーダーを読み込む
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));// シェーダーを作成
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);	// モデルを読み込む準備
	fx->SetDirectory(L"Resources/Models/VerticalAttacker");// モデルのディレクトリを指定
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/VerticalAttacker/VerticalAttacker.cmo", *fx);//  胴体モデルを読み込む
	fx->SetDirectory(L"Resources/Models/Enemy");// モデルのディレクトリを指定（通常の敵の表情差分を流用）
	m_attackFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Face.cmo", *fx);// 攻撃時の顔
	m_idlingFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *fx);// 徘徊時の顔
	m_angryFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *fx);// 怒り時の顔
	m_damageFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *fx);// 攻撃を受けた時の顔
}
/*
*	@brief	描画する
*	@param	context	デバイスコンテキスト
*	@param	states	共通ステート
*	@param	world	ワールド行列
*	@param	view	ビュー行列
*	@param	proj	プロジェクション行列
*	@return なし
*/
void VerticalAttackerModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_bodyModel->Draw(context, *states, world, view, proj);// 胴体描画
	Vector3 lightDir = Vector3::UnitY;	// ライトの方向設定
	lightDir.Normalize();// 正規化
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));	// 影行列の元を作る
	shadowMatrix = world * shadowMatrix;// 影用行列を作成
	m_bodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// 影描画
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// ブレンドステート
			context->OMSetDepthStencilState(states->DepthNone(), 0);// 深度ステンシルステート
			context->RSSetState(states->CullNone());	// カリング
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);// ピクセルシェーダー設定
		});
	switch (m_nowState)	// 表情差分
	{
	case IState::EnemyState::IDLING:// 徘徊
		m_idlingFaceModel->Draw(context, *states, world, view, proj);// 徘徊時の顔
		break;
	case IState::EnemyState::ATTACK:// 攻撃
		m_attackFaceModel->Draw(context, *states, world, view, proj);// 攻撃時の顔
		break;
	case IState::EnemyState::ANGRY:// 怒り
		m_angryFaceModel->Draw(context, *states, world, view, proj);// 怒り時の顔
		break;
	case IState::EnemyState::HIT:// 被弾
		m_damageFaceModel->Draw(context, *states, world, view, proj);// 攻撃を受けた時の顔
		break;
	}
}