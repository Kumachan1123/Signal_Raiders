/*
*	@file	Stage.cpp
*	@brief	ステージクラス
*/
#include <pch.h>
#include "Stage.h"

// ステージの広さ
const float Stage::STAGE_SIZE = 98.5f;
// ステージの高さ
const float Stage::STAGE_HEIGHT = 3.0f;
/*
*	@brief	コンストラクタ
*	@details ステージのコンストラクタ
*	@param なし
*	@return なし
*/
Stage::Stage()
	: m_pCommonResources{}// 共通リソースポインタ
	, m_pModel{}// モデルポインタ
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
	// リソースがnullptrでないことを確認
	assert(resources);
	// リソースを保存
	m_pCommonResources = resources;
	// モデルマネージャーからステージモデルを取得
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Stage");
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
void Stage::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj,
	const DirectX::SimpleMath::Matrix& world)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// 共通ステートを取得
	auto states = m_pCommonResources->GetCommonStates();
	// モデルを描画する
	m_pModel->Draw(context, *states, world, view, proj);
}