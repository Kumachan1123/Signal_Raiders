/*
*	@file	Particle.cpp
*	@brief	パーティクルクラス
*/
#include <pch.h>
#include "Particle.h"

/*
*	@brief	インプットレイアウト
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@detail クラスの生成処理を行う
*	@param type パーティクルのタイプ
*	@param size パーティクルのサイズ
*	@return なし
*/
Particle::Particle(ParticleUtility::Type type, float size)
	:m_pCommonResources{}// 共通リソース
	, m_timer(0.0f)// 経過時間
	, m_elapsedTime(0.0f)// フレーム時間
	, m_pDR{}// デバイスリソース
	, m_pCBuffer{}// コンスタントバッファ
	, m_pInputLayout{}// 入力レイアウト
	, m_pTexture{}// テクスチャ
	, m_pVertexShader{}// 頂点シェーダー
	, m_pPixelShader{}// ピクセルシェーダー
	, m_pGeometryShader{}// ジオメトリシェーダー
	, m_world{}// ワールド行列
	, m_view{}// ビュー行列
	, m_proj{}// プロジェクション行列
	, m_billboard{}// ビルボード行列
	, m_type{ type }// パーティクルのタイプ
	, m_size{ size * 10 }// パーティクルのサイズ(渡した値の十倍）
	, m_barrierBreakSize{ 0 }// バリアの破片のサイズ
	, m_anim{ 0 }// フレーム数
	, m_animTime{ 0.0f }// アニメーション時間
	, m_animSpeed{ 30.0f }// アニメーションの速度
	, m_frameRows{ 1 }// フレームの行数
	, m_frameCols{ 1 }// フレームの列数
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
}
/*
*	@brief	デストラクタ
*	@detail クラスの破棄処理を行う
*	@param なし
*	@return なし
*/
Particle::~Particle() {/*do nothing.*/ }
/*
*	@brief	初期化
*	@detail クラスの初期化処理を行う
*	@param CommonResources* resources 共通リソース
*	@return なし
*/
void Particle::Initialize(CommonResources* resources)
{
	// 共通リソースを取得
	m_pCommonResources = resources;
	// デバイスリソースを取得
	m_pDR = m_pCommonResources->GetDeviceResources();
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// シェーダーの作成
	CreateShaders();
	// 画像の読み込み
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:// 敵の弾の軌跡
	case ParticleUtility::Type::PLAYERTRAIL:// プレイヤーの弾の軌跡
		//	アニメーションの速度
		m_animSpeed = 1;
		//	フレームの列数
		m_frameCols = 1;
		//	フレームの行数
		m_frameRows = 1;
		// テクスチャの取得
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Trail"));
		break;
		// バリア破壊
	case ParticleUtility::Type::BARRIERBREAK:
		m_animSpeed = 17.0f;// アニメーションの速度
		// 	フレームの列数
		m_frameCols = 5;
		// 	フレームの行数
		m_frameRows = 4;
		// テクスチャの取得
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Break"));
		break;
	default:// それ以外のパーティクル
		break;
	}
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);
}
/*
*	@brief	シェーダーの作成
*	@detail シェーダーの作成処理を行う
*	@param なし
*/
void Particle::CreateShaders()
{
	// 頂点シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Particle/VS_Particle.cso", m_pVertexShader);
	// ピクセルシェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Particle/PS_Particle.cso", m_pPixelShader);
	// ジオメトリシェーダーの作成
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Particle/GS_Particle.cso", m_pGeometryShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// 定数バッファ作成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーをセットする
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーをセットする
	m_shaders.ps = m_pPixelShader.Get();
	// シェーダーの構造体にジオメトリシェーダーをセットする
	m_shaders.gs = m_pGeometryShader.Get();
}
/*
*	@brief 更新
*	@detail クラスの更新処理を行う
*	@param float elapsedTime 経過時間
*	@return なし
*/
void Particle::Update(float elapsedTime)
{
	// フレーム時間をセット
	m_elapsedTime = elapsedTime;
	// タイマーを更新
	m_timer += elapsedTime;
	// アニメーションの更新
	m_animTime += elapsedTime * m_animSpeed;
	// タイマーが一定時間を超えたらリセット
	if (m_animTime >= 2.0f)
	{
		// フレーム数を更新
		m_anim++;
		// タイマーをリセット
		m_animTime = 0.0f;
	}
	// パーティクルのタイプによって処理を分ける
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:// 敵の弾の軌跡
	case ParticleUtility::Type::PLAYERTRAIL:// プレイヤーの弾の軌跡
		// 軌跡の更新
		Trail();
		break;
	case ParticleUtility::Type::BARRIERBREAK:// バリア破壊
		// バリア破壊の更新
		if (m_timer <= 0.50f)BarrierBreak();
		break;
	default:// それ以外のパーティクル
		break;
	}
	//	timerを渡して更新処理を行う
	for (std::list<ParticleUtility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		// パーティクルの更新
		if (!(ite->Update(elapsedTime)))
		{
			// 更新失敗（寿命切れなど）なら削除して次へ
			ite = m_particleUtility.erase(ite);
		}
		else
		{
			// 更新成功なら次の要素へ
			++ite;
		}
	}
}
/*
*	@brief 描画
*	@detail クラスの描画処理を行う
*	@param const DirectX::SimpleMath::Matrix& view ビュー行列
*	@param const DirectX::SimpleMath::Matrix& proj プロジェクション行列
*	@return なし
*/
void Particle::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 一定期間が過ぎたら描画しない
	if (m_timer >= 1.9f)return;
	// カメラの方向を取得
	Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	// カメラの方向を正規化
	cameraDir.Normalize();
	// カメラの前方に近い順にソート
	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			// 内積でソート
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});
	// 頂点をクリア
	m_vertices.clear();
	// リストのパーティクルを全て描画する
	for (ParticleUtility& li : m_particleUtility)
	{
		// 内積がマイナスの場合はカメラの後ろなので表示する必要なし
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) continue;
		// 頂点の構造体を作成
		VertexPositionColorTexture vPCT{};
		// 座標
		vPCT.position = XMFLOAT3(li.GetPosition());
		// テクスチャの色
		vPCT.color = XMFLOAT4(li.GetNowColor());
		// 現在のテクスチャのスケールをXMFLOAT2のXに入れる
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);
		// 頂点を追加
		m_vertices.push_back(vPCT);
	}
	// 表示する点がない場合は描画を終わる
	if (m_vertices.empty())return;
	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	// ビュー行列を転置してセットする
	m_constantBuffer.matView = view.Transpose();
	// プロジェクション行列を転置してセットする
	m_constantBuffer.matProj = proj.Transpose();
	// ビルボード行列をワールド行列としてセットする
	m_constantBuffer.matWorld = m_billboard.Transpose();
	// 色をセットする
	m_constantBuffer.colors = Vector4(1, 1, 1, 0);
	// フレーム数をセットする
	m_constantBuffer.count = Vector4((float)(m_anim));
	// 行数をセットする
	m_constantBuffer.height = Vector4((float)(m_frameRows));
	// 列数をセットする
	m_constantBuffer.width = Vector4((float)(m_frameCols));
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constantBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// バリアが壊れたときの破片は深度バッファを使わない
	if (m_type == ParticleUtility::Type::BARRIERBREAK)
	{

		// 描画前設定
		m_pDrawPolygon->DrawSetting(
			DrawPolygon::SamplerStates::ANISOTROPIC_WRAP,//サンプラーステート
			DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
			DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
			DrawPolygon::DepthStencilStates::DEPTH_READ);// 深度ステンシルステート
	}
	else	// それ以外のパーティクルは深度バッファを使う
	{

		// 描画前設定
		m_pDrawPolygon->DrawSetting(
			DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
			DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
			DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
			DrawPolygon::DepthStencilStates::DEPTH_READ); // 深度ステンシルステート
	}
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 指定した座標を中心に、シェーダ側で板ポリゴンを生成・描画させる
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief ビルボード行列の作成
*	@detail ビルボード行列の作成処理を行う
*	@param target 注視点
*	@param eye カメラの位置
*	@param up カメラの上方向
*	@return なし
*/
void Particle::CreateBillboard(const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& up)
{
	using namespace DirectX::SimpleMath;
	// ビルボード行列の作成
	m_billboard = Matrix::CreateBillboard(Vector3::Zero, eye - target, up);
	// 単位行列
	Matrix rot = Matrix::Identity;
	// X軸反転
	rot._11 = -1;
	// Z軸反転
	rot._33 = -1;
	// カメラの位置
	m_cameraPosition = eye;
	// カメラの注視点
	m_cameraTarget = target;
	// ビルボード行列の反転
	m_billboard = rot * m_billboard;
}
/*
*	@brief 軌跡の更新
*	@detail 軌跡の更新処理を行う
*	@param なし
*/
void Particle::Trail()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// タイマーが一定時間を超えたら新しいパーティクルを生成
	if (m_timer >= m_elapsedTime)
	{
		// 乱数の設定
		std::random_device seed;
		// メルセンヌ・ツイスタ法
		std::default_random_engine engine(seed());
		// ランダムな角度（0 から 2π まで）
		std::uniform_real_distribution<> angleDist(0, XM_2PI);
		// ランダムな速度の範囲を設定
		std::uniform_real_distribution<> speedDist(0.5f, 2.0f);
		// XY平面上のランダムな角度
		float randAngleXY = static_cast<float>(angleDist(engine));
		// XZ平面上のランダムな角度
		float randAngleXZ = static_cast<float>(angleDist(engine));
		// ランダムな速度
		float speed = static_cast<float>(speedDist(engine));
		// ランダムな方向の速度ベクトル
		Vector3 randomVelocity = speed * Vector3(
			cosf(randAngleXY) * sinf(randAngleXZ),// X成分
			cosf(randAngleXZ),					 // Y成分
			sinf(randAngleXY) * sinf(randAngleXZ)// Z成分
		);
		// プレイヤーの弾の軌跡
		if (m_type == ParticleUtility::Type::PLAYERTRAIL)
		{
			// パーティクルの生成
			ParticleUtility pU(
				1.0f,  // 生存時間(s)
				m_bulletPosition, // 初期位置 (基準座標)
				randomVelocity, // 初期速度（ランダムな方向）
				Vector3::One, // 加速度
				Vector3::One, // 回転速度
				Vector3(0, 0, 10), // 初期回転
				Vector3(m_size, m_size, 0), // 初期スケール
				Vector3(m_size / 10, m_size / 10, 0), // 最終スケール（小さくなる）
				Vector4(0, 0.2, 1, .5), // 初期カラー（白）
				Vector4(0, 1, 1, 0.5), // 最終カラー（白→透明）
				m_type // パーティクルのタイプ
			);
			// 生成したパーティクルをリストに追加
			m_particleUtility.push_back(pU);
		}
		// 敵の弾の軌跡
		if (m_type == ParticleUtility::Type::ENEMYTRAIL)
		{
			// パーティクルの生成
			ParticleUtility pU(
				1.0f,  // 生存時間(s)
				m_bulletPosition, // 初期位置 (基準座標)
				randomVelocity, // 初期速度（ランダムな方向）
				Vector3::One, // 加速度
				Vector3::One, // 回転速度
				Vector3(0, 0, 10), // 初期回転
				Vector3(1, 1, 0), // 初期スケール
				Vector3(0, 0, 0), // 最終スケール（小さくなる）
				Vector4(1, 0, 1, 1), // 初期カラー（白）
				Vector4(0, 1, 1, 0.25), // 最終カラー（白→透明）
				m_type // パーティクルのタイプ
			);
			// 生成したパーティクルをリストに追加
			m_particleUtility.push_back(pU);
		}
		// タイマーをリセット
		m_timer = 0.0f;
	}
}
/*
*	@brief バリア破壊の更新
*	@detail バリア破壊の更新処理を行う
*	@param なし
*	@return なし
*/
void Particle::BarrierBreak()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// バリアが壊れたときの破片は少し間隔が長めでもよい
	if (m_timer >= m_elapsedTime)
	{
		// 乱数の設定
		std::random_device seed;
		// メルセンヌ・ツイスタ法
		std::default_random_engine engine(seed());
		// 全方向ランダムな角度
		std::uniform_real_distribution<> angleDist(0, XM_2PI);
		// 粒子の速度範囲
		std::uniform_real_distribution<> speedDist(2.5f, 3.0f);
		// 破片のサイズをランダムに設定
		std::uniform_real_distribution<> sizeDist(1.5f, 5.0f);
		// 高さ方向（-1～1）のランダム範囲
		std::uniform_real_distribution<> heightDist(-1.0f, 1.0f);
		// 破片を複数生成
		for (int i = 0; i < 20; ++i)
		{
			// ランダムな方向で飛ばすための球面座標
			// 水平方向の角度 (0～2π)
			float theta = static_cast<float>(angleDist(engine));
			// 垂直方向の角度 (0～π)
			float phi = acosf(static_cast<float>(heightDist(engine)));
			// ランダムな速度
			float speed = static_cast<float>(speedDist(engine)) * 5.0f;
			Vector3 randomVelocity = speed * Vector3(// 球面座標系をデカルト座標系に変換して速度ベクトルを計算
				sinf(phi) * cosf(theta), // X成分
				cosf(phi),              // Y成分
				sinf(phi) * sinf(theta) // Z成分
			);
			// ランダムなサイズ
			float randomSize = static_cast<float>(sizeDist(engine)) * m_barrierBreakSize;
			// パーティクルの生成
			ParticleUtility pU(
				1.5f,  // 破片の生存時間（短くして消えるように）
				m_bossPosition, // 初期位置 (基準座標)
				randomVelocity * 10, // 初期速度（ランダムな方向）
				Vector3::Zero, // 重力加速度（Y軸方向に少し引っ張られる）
				Vector3::Zero, // 回転速度
				Vector3::Zero, // 初期回転
				Vector3(randomSize, randomSize, randomSize), // ランダムなサイズ
				Vector3(0.1f, 0.1f, 0.1f), // 最終スケール（小さくなる）
				Vector4(1.0f, 1.0f, 0.5f, 1.0f),  // 初期カラー（オレンジっぽい）
				Vector4(1.0f, 1.0f, 1.0f, 0.0f), // 最終カラー（白→透明）
				ParticleUtility::Type::BARRIERBREAK // タイプを追加
			);
			// 生成したパーティクルをリストに追加
			m_particleUtility.push_back(pU);
		}
	}
}
