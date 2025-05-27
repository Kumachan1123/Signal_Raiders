/*
	@file	EnemyModel.cpp
	@brief	敵モデルクラス
*/
#include "pch.h"
#include "EnemyModel.h"
/*
*	@brief	コンストラクタ
*	@param	なし
*	@return	なし
*/
EnemyModel::EnemyModel()
	: m_pCommonResources{}// 共通リソース
	, m_bodyModel{}// 胴体モデル
	, m_antennaModel{}// アンテナモデル
	, m_handModel{}// 手モデル
	, m_shadowModel{}// 影用のモデル
	, m_nowState{ IState::EnemyState::IDLING }// 現在の状態
{}
/*
*	@brief	デストラクタ
*	@param	なし
*	@return	なし
*/
EnemyModel::~EnemyModel()
{
	m_pCommonResources = nullptr;	// 共通リソースを解放
	m_bodyModel = nullptr;			// 胴体モデルを解放
	m_antennaModel = nullptr;		// アンテナモデルを解放
	m_handModel = nullptr;			// 手モデルを解放
	m_shadowModel = nullptr;		// 影用のモデルを解放
	m_pPixelShader.Reset();			// ピクセルシェーダーをリセット
	m_pFaceModelMap.clear();			// 顔のモデルマップをクリア
}


/*
*	@brief	初期化
*	@param	CommonResources* resources 共通リソース
*	@return	なし
*/
void EnemyModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;// 共通リソース取得
	auto device = resources->GetDeviceResources()->GetD3DDevice();// デバイス取得
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");	// 影用のシェーダーを読み込む
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));// シェーダー作成
	// モデルを読み込む（ 頭、アンテナ、手、表情差分）
	m_bodyModel = m_pCommonResources->GetModelManager()->GetModel("EnemyHead");// モデルマネージャーから頭モデルを取得
	m_antennaModel = m_pCommonResources->GetModelManager()->GetModel("EnemyAntenna");// モデルマネージャーからアンテナモデルを取得
	m_handModel = m_pCommonResources->GetModelManager()->GetModel("EnemyHand");// モデルマネージャーから手モデルを取得
	m_shadowModel = m_pCommonResources->GetModelManager()->GetModel("EnemyShadow");// モデルマネージャーから影用モデルを取得
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("EnemyDamage");// モデルマネージャーからダメージ顔モデルを取得
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");// モデルマネージャーから攻撃顔モデルを取得
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");// モデルマネージャーから攻撃顔モデルを取得
	m_pFaceModelMap[IState::EnemyState::IDLING] = m_pCommonResources->GetModelManager()->GetModel("EnemyIdling");// モデルマネージャーから攻撃顔モデルを取得
}
/*
*	@brief	描画
*	@param	ID3D11DeviceContext1* context コンテキスト
*	@param	DirectX::DX11::CommonStates* states ステート
*	@param	DirectX::SimpleMath::Matrix world ワールド行列
*	@param	DirectX::SimpleMath::Matrix view ビュー行列
*	@param	DirectX::SimpleMath::Matrix proj プロジェクション行列
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
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);// ピクセルシェーダーをセット
		});
	auto it = m_pFaceModelMap.find(m_nowState);// ステートによって顔のモデルを変更
	if (it != m_pFaceModelMap.end() && it->second)// マップに存在する場合
		it->second->Draw(context, *states, world, view, proj);// 顔のモデルを描画
}