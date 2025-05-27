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
	: m_pCommonResources{}// 共通リソース
	, m_pBodyModel{}// モデル
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
	m_pCommonResources = resources;// 共通リソース
	auto device = resources->GetDeviceResources()->GetD3DDevice();// デバイス取得
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");	// 影用のシェーダーを読み込む
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));// シェーダーを作成
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("VerticalAttacker");// モデルマネージャーから垂直攻撃敵のモデルを取得
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("EnemyDamage");// モデルマネージャーからダメージ顔モデルを取得
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");// モデルマネージャーから攻撃顔モデルを取得
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");// モデルマネージャーから攻撃顔モデルを取得
	m_pFaceModelMap[IState::EnemyState::IDLING] = m_pCommonResources->GetModelManager()->GetModel("EnemyIdling");// モデルマネージャーから攻撃顔モデルを取得
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
	m_pBodyModel->Draw(context, *states, world, view, proj);// 胴体描画
	Vector3 lightDir = Vector3::UnitY;	// ライトの方向設定
	lightDir.Normalize();// 正規化
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));	// 影行列の元を作る
	shadowMatrix = world * shadowMatrix;// 影用行列を作成
	m_pBodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// 影描画
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// ブレンドステート
			context->OMSetDepthStencilState(states->DepthNone(), 0);// 深度ステンシルステート
			context->RSSetState(states->CullNone());	// カリング
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);// ピクセルシェーダー設定
		});
	auto it = m_pFaceModelMap.find(m_nowState);// ステートによって顔のモデルを変更
	if (it != m_pFaceModelMap.end() && it->second)// マップに存在する場合
		it->second->Draw(context, *states, world, view, proj);// 顔のモデルを描画
}