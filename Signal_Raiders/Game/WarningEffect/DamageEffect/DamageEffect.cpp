/*
*	@file DamageEffect.cpp
*	@brief 攻撃を受けた時の演出クラス
*/
#include <pch.h>
#include "DamageEffect.h"
// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC>  DamageEffect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*	@brief コンストラクタ
*	@details ダメージエフェクトのコンストラクタ
*	@param resources CommonResourcesクラスのポインタ
*	@return なし
*/
DamageEffect::DamageEffect(CommonResources* resources)
	: m_pCommonResources(resources)// リソースクラスのポインタ
	, m_time(0.0f)// 時間
	, m_constBuffer{}// コンスタントバッファ
	, m_pDR(resources->GetDeviceResources())// デバイスリソース
	, m_pPlayer(nullptr)// プレイヤー
	, m_pEnemy(nullptr)// 敵
	, m_enemyDirection{}// 攻撃してきた敵の向き
	, m_playEffect(false)// エフェクト再生フラグ
	, m_effectType(EffectType::DAMAGE)// エフェクトタイプ
	, m_pDrawPolygon(DrawPolygon::GetInstance())// 描画ポリゴンクラスのインスタンス
	, m_pCreateShader(CreateShader::GetInstance())// シェーダー作成クラスのインスタンス
{
}
/*
*	@brief デストラクタ
*	@details ダメージエフェクトのデストラクタ
*	@param なし
*	@return なし
*/
DamageEffect::~DamageEffect() {/* do nothing */ }
/*
*	@brief 初期化
*	@details ダメージエフェクトの初期化
*	@param なし
*	@return なし
*/
void DamageEffect::Initialize()
{
	using namespace DirectX::SimpleMath;
	// エフェクト再生フラグをtrueにする
	m_playEffect = true;
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// シェーダーの作成
	MakeShader();
	// 頂点情報の初期化
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
	// エフェクトのタイプに応じて分岐
	if (m_effectType == EffectType::DAMAGE)// ダメージエフェクトなら
	{
		// 赤色に設定
		m_constBuffer.colors = Vector4(2.0f, 0.0f, 0.0f, 1.0f);
		// 攻撃してきた敵の向きを取得
		m_enemyDirection = m_pPlayer->GetEnemyBulletDirection();
	}
	else if (m_effectType == EffectType::INCOMINGENEMY)// 警戒エフェクトなら
	{
		// 黄色に設定
		m_constBuffer.colors = Vector4(1.0f, 0.5f, 0.0f, 1.0f);
		// 敵の位置を取得
		m_enemyPosition = m_pEnemy->GetPosition();
		// プレイヤーと敵の位置の差を取得
		m_enemyDirection = m_pPlayer->GetPlayerPos() - m_enemyPosition;
	}
}
/*
*	@brief シェーダー作成
*	@details シェーダーの作成
*	@param なし
*	@return なし
*/
void  DamageEffect::MakeShader()
{
	// 頂点シェーダー作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/DamageEffect/VS_Damage.cso", m_pVertexShader);
	// ピクセルシェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/DamageEffect/PS_Damage.cso", m_pPixelShader);
	// 入力レイアウト取得
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// コンスタントバッファ作成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// 頂点シェーダーをセット
	m_shaders.vs = m_pVertexShader.Get();
	// ピクセルシェーダーをセット
	m_shaders.ps = m_pPixelShader.Get();
	// ジオメトリシェーダーは使わないのでnullptr
	m_shaders.gs = nullptr;
}
/*
*	@brief 更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void  DamageEffect::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// エフェクト再生フラグがfalseなら更新しない
	if (!m_playEffect)return;
	// 時間を加算
	m_time += elapsedTime;
	// 再生時間を過ぎたら
	if (m_time >= PLAY_TIME)
	{
		// 時間を初期化
		m_time = 0.0f;
		// アルファ値を初期化
		m_constBuffer.colors.w = 0.0f;
		// エフェクト再生フラグをfalseにする
		m_playEffect = false;
		// プレイヤーにエフェクト再生フラグを送る
		m_pPlayer->SetisPlayEffect(m_playEffect);
		// 更新終了
		return;
	}
	// 時間をコンスタントバッファに格納
	m_constBuffer.time = Vector4(m_time);
	// アルファ値を計算
	m_constBuffer.colors.w = 0.5f + 0.5f * sin(m_time * 2.0f);
	// 角度を計算する
	float angle = CalculateAngle();
	// 角度からUV座標を求める
	m_constBuffer.uv = GetUVFromAngle(angle);
}
/*
*	@brief 角度計算
*	@details 敵の向きとプレイヤーの向きの差を計算
*	@param なし
*	@return 角度
*/
float DamageEffect::CalculateAngle() const
{
	using namespace DirectX::SimpleMath;
	// プレイヤーの向き
	Vector3 playerDir = m_pPlayer->GetPlayerDir();
	// プレイヤーと敵の向きの差
	float angle = atan2(m_enemyDirection.x, m_enemyDirection.z) - atan2(playerDir.x, playerDir.z);
	// ラジアンから度に変換
	angle = DirectX::XMConvertToDegrees(angle);
	// 0から360の間に収める
	return (angle < 0) ? angle + 360 : angle;
}
/*
*	@brief 角度からUV座標を求める
*	@details 角度からUV座標を求める
*	@param angle 角度
*	@return UV座標
*/
DirectX::SimpleMath::Vector4 DamageEffect::GetUVFromAngle(float angle) const
{
	using namespace DirectX::SimpleMath;
	// 度からラジアンに変換
	float rad = DirectX::XMConvertToRadians(angle);
	// 半径は左右または上下の中心からの距離
	float radius = UV_W - UV_C;
	// 座標を計算:U
	float u = UV_C - radius * -sin(rad);
	// 座標を計算:V
	float v = UV_C + radius * cos(rad);
	// UV座標を返す
	return  Vector4(u, v, 0, 0);
}
/*
*	@brief 描画
*	@details ダメージエフェクトの描画
*	@param なし
*	@return なし
*/
void  DamageEffect::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionTexture vertex[4] =
	{
		// 頂点情報													UV情報
		VertexPositionTexture(Vector3(-SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f),  Vector2(0.0f, 0.0f)),// 左上
		VertexPositionTexture(Vector3(SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f),   Vector2(1.0f, 0.0f)),// 右上
		VertexPositionTexture(Vector3(SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f),   Vector2(1.0f, 1.0f)),// 右下
		VertexPositionTexture(Vector3(-SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f),  Vector2(0.0f, 1.0f)),// 左下
	};
	// ビュー行列を転置してコンスタントバッファに格納
	m_constBuffer.matView = m_view.Transpose();
	// プロジェクション行列を転置してコンスタントバッファに格納
	m_constBuffer.matProj = m_proj.Transpose();
	// ワールド行列を転置してコンスタントバッファに格納
	m_constBuffer.matWorld = m_world.Transpose();
	// コンスタントバッファの更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// ConstBufferからID3D11Bufferへの変換
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// コンスタントバッファのセット
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	// 描画開始
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// シェーダーの登録
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	// シェーダー解放
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief エフェクトの破棄判定
*	@details エフェクトの破棄判定
*	@param なし
*	@return 破棄するならtrue、破棄しないならfalse
*/
bool DamageEffect::Destroy() const
{
	// 破棄時間を過ぎたら破棄する
	if (m_time >= DESTROY_TIME)
	{
		return true;// 破棄時間を過ぎたらtrue
	}
	else
	{
		// それ以外はfalse
		return false;
	}
}