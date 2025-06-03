/*
	@file	Effect.cpp
	@brief	エフェクトクラス
*/
#include <pch.h>
#include "Effect.h"
/*
*	@brief	入力レイアウト
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Effect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const 	float Effect::m_vertexMinX = -1.0f;// 最小X座標
const 	float Effect::m_vertexMaxX = 1.0f;// 最大X座標
const 	float Effect::m_vertexMinY = -1.0f;// 最小Y座標
const 	float Effect::m_vertexMaxY = 1.0f;// 最大Y座標

/*
*	@brief	コンストラクタ
*	@details エフェクトの初期化を行う
*	@param resources 共通リソース
*	@param type エフェクトの種類
*	@param playPos 再生する座標
*	@param scale スケール
*	@param world ワールド行列
*	@return	なし
*/
Effect::Effect(CommonResources* resources, EffectType type, const DirectX::SimpleMath::Vector3& playPos, float scale, const DirectX::SimpleMath::Matrix& world)
	: m_position(playPos)// エフェクトを再生する座標
	, m_type(type)// エフェクトの種類
	, m_scale(scale)// エフェクトのスケール
	, m_pCommonResources(resources)// 共通リソース
	, m_world(world)// ワールド行列
	, m_isPlaying(false)// 再生中かのフラグ
	, m_anim(0)// アニメーションのコマ
	, m_animSpeed(30.0f)// アニメーションのスピード
	, m_animTime(0.0f)// アニメーションの時間
	, m_frameRows{}// 画像の行数
	, m_frameCols{}// 画像の列数
	, m_vertices{}// 頂点情報
	, m_offSetY(0.0f)// 高さ
	, m_pDrawPolygon(DrawPolygon::GetInstance())// 板ポリゴン描画クラス
	, m_pCreateShader(CreateShader::GetInstance())// シェーダー作成クラス
{
	switch (m_type)// エフェクトの種類によって画像を読み込む
	{
	case EffectType::ENEMY_DEAD:// 敵死亡エフェクト
		m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Effect")); // テクスチャの取得
		m_frameRows = 4; // 画像の行数
		m_frameCols = 5; // 画像の列数
		break;
	case EffectType::ENEMY_HIT:// 敵ヒットエフェクト
		m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Hit")); // テクスチャの取得
		m_frameRows = 2; // 画像の行数
		m_frameCols = 4; // 画像の列数
		break;
	}
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());// 頂点情報の初期化
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);	// シェーダー作成クラスの初期化
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_pVertexShader);// 頂点シェーダー
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pPixelShader);	// ピクセルシェーダー
	m_pInputLayout = m_pCreateShader->GetInputLayout();// インプットレイアウトの取得
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// コンスタントバッファ生成
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダ
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダ
	m_shaders.gs = nullptr;// ジオメトリシェーダ
	m_isPlaying = true;	// アニメーションを再生中にする
}
/*
*	@brief	デストラクタ
*	@details エフェクトの終了処理を行う
*	@param なし
*	@return なし
*/
Effect::~Effect() {}
/*
*	@brief	画像を読み込む
*	@details 指定されたパスから画像を読み込み、テクスチャとして保存する
*	@param path 画像のパス
*	@return なし
*/
void Effect::LoadTexture(const wchar_t* path)
{
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// デバイス
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// テクスチャ
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());// 画像の読み込み
	m_pTextures.push_back(texture);// テクスチャの追加
}
/*
*	@brief	更新処理
*	@details エフェクトのアニメーションを更新する
*	@param elapsedTime 経過時間
*	@return なし
*/
void Effect::Update(float elapsedTime)
{
	m_animTime += elapsedTime * m_animSpeed;// 経過時間を加算
	if (m_animTime >= 1.0f)// 1秒経過
	{
		m_anim++;// アニメのコマを薦める
		m_animTime = 0.0f;// タイマーをリセット
	}
	if (m_anim == m_frameRows * m_frameCols)m_isPlaying = false;// 再生終了
}

/*
*	@brief	描画処理
*	@details エフェクトを描画する
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void Effect::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 頂点情報																	    
	m_vertices[0] = { VertexPositionTexture(Vector3(m_vertexMinX, m_vertexMaxY, 0), Vector2(0, 1)) };// 左上
	m_vertices[1] = { VertexPositionTexture(Vector3(m_vertexMaxX, m_vertexMaxY, 0), Vector2(1, 1)) };// 右上
	m_vertices[2] = { VertexPositionTexture(Vector3(m_vertexMaxX,m_vertexMinY, 0),  Vector2(1, 0)) };// 右下
	m_vertices[3] = { VertexPositionTexture(Vector3(m_vertexMinX,m_vertexMinY, 0),  Vector2(0, 0)) }; // 左下
	// ビルボード用の頂点情報
	VertexPositionTexture billboardVertex[4]{};
	// ビルボードの高さを調整
	for (int i = 0; i < 4; i++)
	{
		billboardVertex[i] = m_vertices[i];		// 頂点情報をコピー
		billboardVertex[i].position.x *= m_scale;// スケール
		billboardVertex[i].position.y *= m_scale;// スケール
		billboardVertex[i].position.y -= m_offSetY;// 高さ
	}
	// ビルボード行列を作成
	// ビュー行列の逆行列
	Matrix billboardMatrix = view.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;
	// ビルボードをアフィン変換
	Matrix worldBillboard = m_world * billboardMatrix;
	// ビルボードの位置を設定
	worldBillboard *= Matrix::CreateTranslation(m_position);
	// コンスタントバッファの更新
	// ワールド行列を転置して設定
	m_constBuffer.matWorld = worldBillboard.Transpose();
	// ビュー行列を転置して設定
	m_constBuffer.matView = view.Transpose();
	// プロジェクション行列を転置して設定
	m_constBuffer.matProj = proj.Transpose();
	// アニメーションの設定
	// アニメーションのコマ数を設定
	m_constBuffer.count = Vector4((float)(m_anim));
	// テクスチャの列を設定
	m_constBuffer.height = Vector4((float)(m_frameRows));
	// テクスチャの行を設定
	m_constBuffer.width = Vector4((float)(m_frameCols));
	// コンスタントバッファの更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// シェーダーにコンスタントバッファを設定
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// テクスチャのサンプラーステート(リニア、ラップ)
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート(非乗算)
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート(カリングなし)
		DrawPolygon::DepthStencilStates::DEPTH_READ);// 深度ステンシルステート(読み取りのみ)
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// シェーダーの登録
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// テクスチャの描画
	m_pDrawPolygon->DrawTexture(billboardVertex);
	// シェーダーの解放
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief	終了処理
*	@details エフェクトのリソースを解放する
*	@param なし
*	@return なし
*/
void Effect::Finalize()
{
	// 頂点シェーダの解放
	m_pVertexShader.Reset();
	// ピクセルシェーダの解放
	m_pPixelShader.Reset();
	// インプットレイアウトの解放
	m_pInputLayout.Reset();
	// コンスタントバッファの解放
	m_pCBuffer.Reset();
	// テクスチャの解放
	m_pTextures.clear();
}
