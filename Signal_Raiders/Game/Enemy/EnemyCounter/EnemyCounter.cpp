/*
*	@file	EnemyCounter.cpp
*	@brief	敵カウンタ―クラス
*/
#include <pch.h>
#include "EnemyCounter.h"
/*
*	@brief	インプットレイアウト
*	@return なし
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  EnemyCounter::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@details 敵カウンタ―クラスのコンストラクタ
*	@param CommonResources* pCommonResources	共通リソースへのポインタ
*	@return なし
*/
EnemyCounter::EnemyCounter()
	: m_pCommonResources{}// 共通リソースへのポインタ
	, m_enemyIndex{ 0 }// 敵の総数(保存用）
	, m_nowEnemy{ 0 }// 現在の敵の数(保存用）
	, m_pTexture{}//	数字画像
	, m_enemyIndex10{ 0 }// 敵の総数(10の位)
	, m_enemyIndex1{ 0 }// 敵の総数(1の位)
	, m_nowEnemy10{ 0 }// 現在の敵の数(10の位)
	, m_nowEnemy1{ 0 }// 現在の敵の数(1の位)
	, m_frameRows{ 1 }//	画像の行数
	, m_frameCols{ 10 }//	画像の列数
	, m_pInputLayout{}// インプットレイアウト
	, m_verticesEnemyIndex10{}// 総数の10の位
	, m_verticesEnemyIndex1{}// 総数の1の位
	, m_verticesNowEnemy10{}// 現在の敵の数の10の位
	, m_verticesNowEnemy1{}// 現在の敵の数の1の位
	, m_verticesRemaining{}// 「残り：」
	, m_verticesSlash{}// 「/」
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
{}
/*
*	@brief	デストラクタ
*	@details 敵カウンタ―クラスのデストラクタ
*	@param なし
*	@return なし
*/
EnemyCounter::~EnemyCounter() {/*do nothing*/ }
/*
*	@brief 初期化
*	@details 敵カウンタ―クラスの初期化
*	@param CommonResources* pCommonResources	共通リソースへのポインタ
*	@return なし
*/
void EnemyCounter::Initialize(CommonResources* pCommonResources)
{
	// 共通リソースへのポインタ取得
	m_pCommonResources = pCommonResources;
	//	数字テクスチャの取得
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Number"));
	// 「残り：」テクスチャの取得
	m_pRemaining.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Remaining"));
	// 「/」テクスチャの取得
	m_pSlash.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Slash"));
	// 板ポリゴン描画クラス初期化
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// 頂点シェーダー作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_pVertexShader);
	// ピクセルシェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pPixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーをセット
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーをセット
	m_shaders.ps = m_pPixelShader.Get();
	// シェーダーの構造体にジオメトリシェーダーをセット（使わないのでnullptr）
	m_shaders.gs = nullptr;
	// 各頂点の初期化
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		// 総数の10の位
		m_verticesEnemyIndex10[i] = {};
		// 総数の1の位
		m_verticesEnemyIndex1[i] = {};
		// 現在の敵の数の10の位
		m_verticesNowEnemy10[i] = {};
		// 現在の敵の数の1の位
		m_verticesNowEnemy1[i] = {};
		// 「残り：」
		m_verticesRemaining[i] = {};
		// 「/」
		m_verticesSlash[i] = {};
	}
}
/*
*	@brief 更新
*	@details 敵カウンタ―クラスの更新処理
*	@param float elapsedTime	経過時間
*	@return なし
*/
void EnemyCounter::Update(float elapsedTime)
{
	// 未使用変数の警告を無視
	UNREFERENCED_PARAMETER(elapsedTime);
	// 受け取った値から10の位と1の位を計算
	// 敵の総数の10の位
	m_enemyIndex10 = m_enemyIndex / 10;
	// 敵の総数の1の位
	m_enemyIndex1 = m_enemyIndex % 10;
	// 現在の敵の数の10の位
	m_nowEnemy10 = m_nowEnemy / 10;
	// 現在の敵の数の1の位
	m_nowEnemy1 = m_nowEnemy % 10;
}
/*
*	@brief 描画
*	@details 敵カウンタ―クラスの描画処理
*	@param float elapsedTime	経過時間
*	@return なし
*/
void EnemyCounter::Render()
{
	// 「残り：」
	DrawQuad(m_pRemaining, m_verticesRemaining, REMAINING_POS_X, REMAINING_POS_Y, REMAINING_SIZE_X, REMAINING_SIZE_Y, 0, 1, 1);
	// 「/」
	DrawQuad(m_pSlash, m_verticesSlash, SLASH_POS_X, SLASH_POS_Y, SLASH_SIZE_X, SLASH_SIZE_Y, 0, 1, 1);
	// 総数の1の位を描画
	DrawEnemyIndex1();
	// 総数の10の位を描画
	DrawEnemyIndex10();
	// 現在の敵の数の1の位を描画
	DrawNowEnemy1();
	// 現在の敵の数の10の位を描画
	DrawNowEnemy10();
}
/*
*	@brief	現在の敵の数の10の位を描画
*	@details 現在の敵の数の10の位を描画する
*	@param なし
*	@return なし
*/
void EnemyCounter::DrawNowEnemy10()
{
	// 現在の敵の数の10の位を描画
	DrawQuad(m_pTexture, m_verticesNowEnemy10,// 画像と頂点情報
		NOW_ENEMY_10_POS_X, NOW_ENEMY_10_POS_Y, NOW_ENEMY_SIZE_X, NOW_ENEMY_SIZE_Y, // 位置とサイズ
		m_nowEnemy10, m_frameCols, m_frameRows // 今の敵の数の10の位とフレーム情報
	);
}
/*
*	@brief	現在の敵の数の1の位を描画
*	@details 現在の敵の数の1の位を描画する
*	@param なし
*	@return なし
*/
void EnemyCounter::DrawNowEnemy1()
{
	// 現在の敵の数の1の位を描画
	DrawQuad(m_pTexture, m_verticesNowEnemy1,// 画像と頂点情報
		NOW_ENEMY_1_POS_X, NOW_ENEMY_1_POS_Y, NOW_ENEMY_SIZE_X, NOW_ENEMY_SIZE_Y, // 位置とサイズ
		m_nowEnemy1, m_frameCols, m_frameRows  // 今の敵の数の10の位とフレーム情報
	);
}
/*
*	@brief	敵の総数の10の位を描画
*	@details 敵の総数の10の位を描画する
*	@param なし
*	@return なし
*/
void EnemyCounter::DrawEnemyIndex10()
{
	// 敵の総数の10の位を描画
	DrawQuad(m_pTexture, m_verticesEnemyIndex10,// 画像と頂点情報
		ENEMY_INDEX_10_POS_X, ENEMY_INDEX_10_POS_Y, ENEMY_INDEX_SIZE_X, ENEMY_INDEX_SIZE_Y, // 位置とサイズ
		m_enemyIndex10, m_frameCols, m_frameRows // 敵の総数の10の位とフレーム情報
	);

}
/*
*	@brief	敵の総数の1の位を描画
*	@details 敵の総数の1の位を描画する
*	@param なし
*	@return なし
*/
void EnemyCounter::DrawEnemyIndex1()
{
	// 敵の総数の1の位を描画
	DrawQuad(m_pTexture, m_verticesEnemyIndex1,// 画像と頂点情報
		ENEMY_INDEX_1_POS_X, ENEMY_INDEX_1_POS_Y, ENEMY_INDEX_SIZE_X, ENEMY_INDEX_SIZE_Y,// 位置とサイズ
		m_enemyIndex1, m_frameCols, m_frameRows // 敵の総数の1の位とフレーム情報
	);
}
/*
*	@brief	Quad描画
*	@details 板ポリゴンを描画する
*	@param std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture	テクスチャ
*	@param DirectX::VertexPositionTexture* vertices	頂点情報
*	@param float startX	開始位置X
*	@param float startY	開始位置Y
*	@param float width	幅
*	@param float height	高さ
*	@param int frameIndex	フレームインデックス
*	@param int frameCols	フレームの列数
*	@param int frameRows	フレームの行数
*/
void EnemyCounter::DrawQuad(
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
	DirectX::VertexPositionTexture* vertices,
	float startX, float startY, float width, float height,
	int frameIndex, int frameCols, int frameRows)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 頂点座標の設定
	vertices[0] = { VertexPositionTexture(Vector3(startX, startY, 0), Vector2(0, 0)) };// 左上
	vertices[1] = { VertexPositionTexture(Vector3(startX + width, startY, 0), Vector2(1, 0)) };// 右上
	vertices[2] = { VertexPositionTexture(Vector3(startX + width, startY - height, 0), Vector2(1, 1)) };// 右下
	vertices[3] = { VertexPositionTexture(Vector3(startX, startY - height, 0), Vector2(0, 1)) };// 左下
	// コンスタントバッファに渡すデータを設定
	// ワールド行列を単位行列に設定
	m_constBuffer.matWorld = Matrix::Identity;
	// ビュー行列を単位行列に設定
	m_constBuffer.matView = Matrix::Identity;
	// プロジェクション行列を単位行列に設定
	m_constBuffer.matProj = Matrix::Identity;
	// アニメーションのコマを設定
	m_constBuffer.count = Vector4((float)(frameIndex));
	// 高さを設定
	m_constBuffer.height = Vector4((float)(frameRows));
	// 幅を設定
	m_constBuffer.width = Vector4((float)(frameCols));
	// 受け渡し用バッファの内容更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// ConstBufferからID3D11Bufferへの変換
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// デプスステンシルステート
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), texture);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertices);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}