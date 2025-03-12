/*
	@file	Effect.cpp
	@brief	エフェクトクラス
*/
#include "pch.h"
#include "Effect.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;


/*
*	@brief	入力レイアウト
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Effect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const 	float Effect::m_vertexMinX = -1.0f;
const 	float Effect::m_vertexMaxX = 1.0f;
const 	float Effect::m_vertexMinY = -1.0f;
const 	float Effect::m_vertexMaxY = 1.0f;

/*
*	@brief	コンストラクタ
*	@param[in] resources 共通リソース
*	@param[in] type エフェクトの種類
*	@param[in] playPos 再生する座標
*	@param[in] scale スケール
*	@param[in] world ワールド行列
*	@return	なし
*/
Effect::Effect(CommonResources* resources, EffectType type, DirectX::SimpleMath::Vector3 playPos, float scale, DirectX::SimpleMath::Matrix world)
	: m_position(playPos)
	, m_type(type)
	, m_scale(scale)
	, m_commonResources(resources)
	, m_world(world)
	, m_isPlaying(false)
	, m_anim(0)
	, m_animSpeed(30.0f)
	, m_animTime(0.0f)
	, m_frameRows{}// 画像の行数
	, m_frameCols{}// 画像の列数
	, m_vertices{}
	, m_offSetY(0.0f)
	, m_pDrawPolygon(DrawPolygon::GetInstance())
	, m_pCreateShader(CreateShader::GetInstance())
{
	switch (m_type)// エフェクトの種類によって画像を読み込む
	{
	case EffectType::ENEMY_DEAD:// 敵死亡エフェクト
		LoadTexture(L"Resources/Textures/effect.png");
		m_frameRows = 4; // 画像の行数
		m_frameCols = 5; // 画像の列数

		break;
	case EffectType::ENEMY_HIT:// 敵ヒットエフェクト
		LoadTexture(L"Resources/Textures/hit.png");
		m_frameRows = 2; // 画像の行数
		m_frameCols = 4; // 画像の列数

		break;
	}
	m_pDrawPolygon->InitializePositionTexture(m_commonResources->GetDeviceResources());// 頂点情報の初期化
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);	// シェーダー作成クラスの初期化
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_vertexShader);// 頂点シェーダー
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pixelShader);	// ピクセルシェーダー
	m_pInputLayout = m_pCreateShader->GetInputLayout();// インプットレイアウトの取得
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));// コンスタントバッファ生成
	m_shaders.vs = m_vertexShader.Get();// 頂点シェーダ
	m_shaders.ps = m_pixelShader.Get();// ピクセルシェーダ
	m_shaders.gs = nullptr;// ジオメトリシェーダ
	m_isPlaying = true;	// アニメーションを再生中にする
}
/*
*	@brief	デストラクタ
*	@return なし
*/
Effect::~Effect() {}
/*
*	@brief	画像を読み込む
*	@param[in] path 画像のパス
*	@return なし
*/
void Effect::LoadTexture(const wchar_t* path)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();// デバイス
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// テクスチャ
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());// 画像の読み込み
	m_textures.push_back(texture);// テクスチャの追加
}
/*
*	@brief	更新処理
*	@param[in] elapsedTime 経過時間
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
*	@param[in] view ビュー行列
*	@param[in] proj プロジェクション行列
*	@return なし
*/
void Effect::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// 頂点情報
	m_vertices[0] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMaxY, 0), SimpleMath::Vector2(0, 1)) };// 左上
	m_vertices[1] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMaxY, 0), SimpleMath::Vector2(1, 1)) };// 右上
	m_vertices[2] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX,m_vertexMinY, 0), SimpleMath::Vector2(1, 0)) };// 右下
	m_vertices[3] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX,m_vertexMinY, 0), SimpleMath::Vector2(0, 0)) }; // 左下
	VertexPositionTexture billboardVertex[4]{};// ビルボード用の頂点情報
	for (int i = 0; i < 4; i++)
	{
		billboardVertex[i] = m_vertices[i];		// 頂点情報をコピー
		billboardVertex[i].position.x *= m_scale;// スケール
		billboardVertex[i].position.y *= m_scale;// スケール
		billboardVertex[i].position.y -= m_offSetY;// 高さ
	}
	// ビルボード行列を作成
	Matrix billboardMatrix = view.Invert();// ビュー行列の逆行列
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;
	Matrix worldBillboard = m_world * billboardMatrix;	// ビルボードをアフィン変換
	worldBillboard *= Matrix::CreateTranslation(m_position);// 位置
	m_constBuffer.matWorld = worldBillboard.Transpose();// ワールド行列
	m_constBuffer.matView = view.Transpose();// ビュー行列
	m_constBuffer.matProj = proj.Transpose();// プロジェクション行列
	m_constBuffer.count = Vector4((float)(m_anim));// アニメーションカウント
	m_constBuffer.height = Vector4((float)(m_frameRows));// 高さ
	m_constBuffer.width = Vector4((float)(m_frameCols));// 幅
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);// コンスタントバッファの更新
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };	// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->DrawSetting(// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// テクスチャのサンプラーステート(リニア、ラップ)
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート(非乗算)
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート(カリングなし)
		DrawPolygon::DepthStencilStates::DEPTH_READ);// 深度ステンシルステート(読み取りのみ)
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);	// 描画準備
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// シェーダーの登録
	m_pDrawPolygon->DrawTexture(billboardVertex);// テクスチャの描画
	m_pDrawPolygon->ReleaseShader();// シェーダーの解放
}

/*
*	@brief	終了処理
*	@return なし
*/
void Effect::Finalize()
{
	m_vertexShader.Reset();// 頂点シェーダの解放
	m_pixelShader.Reset();// ピクセルシェーダの解放
	m_pInputLayout.Reset();	// インプットレイアウトの解放
	m_cBuffer.Reset();// コンスタントバッファの解放
	m_textures.clear();// テクスチャの解放
}
