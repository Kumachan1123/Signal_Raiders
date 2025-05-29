/*
*	@file	Stage.cpp
*	@brief	ステージクラス
*/
#include <pch.h>
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
	: m_pCommonResources{}
	, m_pModel{}
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
	m_pCommonResources = resources;// リソースを保存
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Stage");// モデルマネージャーからステージモデルを取得
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
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキストを取得
	auto states = m_pCommonResources->GetCommonStates();// 共通ステートを取得
	world *= Matrix::CreateTranslation(pos);// ワールド行列を更新
	m_pModel->Draw(context, *states, world, view, proj);// モデルを描画する
}