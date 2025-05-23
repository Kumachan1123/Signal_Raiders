/*
	@file	Sky.cpp
	@brief	スカイクラス
*/
#include "pch.h"
#include "Stage.h"

const float Stage::STAGE_SIZE = 98.5f;	// ステージの広さ
const float Stage::STAGE_HEIGHT = 3.0f;	// ステージの高さ
/*
*	@brief	コンストラクタ
*	@details ステージのコンストラクタ
*	@param なし
*	@return なし
*/
Stage::Stage()
	: m_commonResources{}
	, m_model{}
	, m_texturePath{}
{
}
/*
*	@brief	デストラクタ
*	@details ステージのデストラクタ
*	@param なし
*	@return なし
*/
Stage::~Stage() { /* do nothing.*/ }
/*
*	@brief	初期化
*	@details ステージの初期化
*	@param resources 共通リソース
*	@return なし
*/
void Stage::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(resources);// リソースがnullptrでないことを確認
	m_commonResources = resources;// リソースを保存
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();// デバイスを取得
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);// モデルを読み込む準備
	fx->SetDirectory(L"Resources/models/Stage");// モデルのディレクトリを設定
	wcscpy_s(m_texturePath, L"Resources/models/Stage/Stage.cmo");//テクスチャパスを設定する
	m_model = Model::CreateFromCMO(device, m_texturePath, *fx);// モデルを読み込む
	m_model->UpdateEffects([](DirectX::IEffect* effect)	// モデルのエフェクト情報を更新する
		{

			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);// ベーシックエフェクトを設定する
			if (!basicEffect)return;// エフェクトがnullptrの場合は処理を終える
			basicEffect->SetLightEnabled(0, false);// ライトを無効にする
			basicEffect->SetLightEnabled(1, false);// ライトを無効にする
			basicEffect->SetLightEnabled(2, false);// ライトを無効にする
			basicEffect->SetEmissiveColor(Colors::White);// モデルを自発光させる
		}
	);
}
/*
*	@brief	描画
*	@details ステージの描画
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@param world ワールド行列
*	@param pos ワールド座標
*	@return なし
*/
void Stage::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Vector3 pos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキストを取得
	auto states = m_commonResources->GetCommonStates();// 共通ステートを取得
	world *= Matrix::CreateTranslation(pos);// ワールド行列を更新
	m_model->Draw(context, *states, world, view, proj);// モデルを描画する
}