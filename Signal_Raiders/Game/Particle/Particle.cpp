/*
	@file	Particle.cpp
	@brief	弾の軌跡クラス
*/
#include "pch.h"
#include "Particle.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include  "Game/KumachiLib/BinaryFile.h"

#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include <algorithm>
#include <random>
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Particle::Particle(ParticleUtility::Type type, float size)
	:m_commonResources{}
	, m_timer(0.0f)
	, m_pDR{}
	, m_CBuffer{}
	, m_inputLayout{}
	, m_batch{}
	, m_states{}
	, m_texture{}
	, m_vertexShader{}
	, m_pixelShader{}
	, m_geometryShader{}
	, m_world{}
	, m_view{}
	, m_proj{}
	, m_billboard{}
	, m_type{ type }
	, m_size{ size * 10 }
{
}

Particle::~Particle()
{
}

void Particle::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	// シェーダーの作成
	CreateShader();
	// 画像の読み込み
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:
	case ParticleUtility::Type::PLAYERTRAIL:
		LoadTexture(L"Resources/Textures/Trail.png");
		break;
	case ParticleUtility::Type::BARRIERDESTROYED:
		LoadTexture(L"Resources/Textures/break.png");
		break;
	default:
		break;
	}
	// 板ポリゴン描画用
	DrawPolygon::InitializePositionColorTexture(m_pDR);
}

void Particle::CreateShader()
{
	ID3D11Device* device = m_pDR->GetD3DDevice();
	// コンパイルされたシェーダーの読み込み
	kumachi::BinaryFile VS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Particle/VS_Particle.cso");
	kumachi::BinaryFile PS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Particle/PS_Particle.cso");
	kumachi::BinaryFile GS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Particle/GS_Particle.cso");

	// インプットレイアウト作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VS.GetData(), VS.GetSize(),
		m_inputLayout.GetAddressOf());
	// 頂点シェーダー作成
	if (FAILED(device->CreateVertexShader(VS.GetData(), VS.GetSize(), nullptr, m_vertexShader.GetAddressOf())))
	{
		throw std::exception("頂点シェーダーの作成に失敗しました");
	}
	// ピクセルシェーダー作成
	if (FAILED(device->CreatePixelShader(PS.GetData(), PS.GetSize(), nullptr, m_pixelShader.GetAddressOf())))
	{
		throw std::exception("ピクセルシェーダーの作成に失敗しました");
	}
	// ジオメトリシェーダー作成
	if (FAILED(device->CreateGeometryShader(GS.GetData(), GS.GetSize(), nullptr, m_geometryShader.GetAddressOf())))
	{
		throw std::exception("ジオメトリシェーダーの作成に失敗しました");
	}
	// 定数バッファ作成
	D3D11_BUFFER_DESC desc = {};
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(ConstBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	device->CreateBuffer(&desc, nullptr, &m_CBuffer);
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = m_geometryShader.Get();
}



// 画像の読み込み
void Particle::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.GetAddressOf());
	m_texture.push_back(texture);
}

void Particle::Update(float elapsedTime)
{
	m_timer += elapsedTime;
	//	軌跡の更新
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:
	case ParticleUtility::Type::PLAYERTRAIL:
		Trail();
		break;
	case ParticleUtility::Type::BARRIERDESTROYED:

		if (m_timer <= 0.50f)BarrierBreak();

		break;
	default:
		break;
	}

	//	timerを渡してm_effectの更新処理を行う
	for (std::list<ParticleUtility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		//	子クラスからfalseで消す
		if (!(ite)->Update(elapsedTime))
		{
			ite = m_particleUtility.erase(ite);
			if (ite == m_particleUtility.end()) break;
		}
	}
}

void Particle::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	if (m_timer >= 1.9f)return;
	auto context = m_pDR->GetD3DDeviceContext();
	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();
	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			//	カメラ正面の距離でソート
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});
	m_vertices.clear();
	for (ParticleUtility& li : m_particleUtility)
	{
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) {
			//	内積がマイナスの場合はカメラの後ろなので表示する必要なし
			continue;
		}
		VertexPositionColorTexture vPCT{};

		//	座標
		vPCT.position = XMFLOAT3(li.GetPosition());
		//	テクスチャの色
		vPCT.color = XMFLOAT4(li.GetNowColor());

		//	現在のテクスチャのスケールを「XMFLOAT2」のXに入れる。
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_vertices.push_back(vPCT);
	}

	//	表示する点がない場合は描画を終わる
	if (m_vertices.empty())
	{
		return;
	}
	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	m_constantBuffer.matView = view.Transpose();
	m_constantBuffer.matProj = proj.Transpose();
	m_constantBuffer.matWorld = m_billboard.Transpose();
	m_constantBuffer.colors = SimpleMath::Vector4(1, 1, 1, 0);

	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	DrawPolygon::UpdateSubResources(context, m_CBuffer.Get(), &m_constantBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	// 描画準備
	DrawPolygon::DrawStart(context, m_inputLayout.Get(), m_texture);
	// シェーダをセットする
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	// 指定した座標を中心に、シェーダ側で板ポリゴンを生成・描画させる
	DrawPolygon::DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	// シェーダの登録を解除しておく
	DrawPolygon::ReleaseShader(context);
}

// ビルボードの作成
void Particle::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
{
	m_billboard =
		SimpleMath::Matrix::CreateBillboard(SimpleMath::Vector3::Zero, eye - target, up);

	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;
	m_billboard = rot * m_billboard;
}

// パーティクルの生成
void Particle::Trail()
{
	// タイマーが一定時間（0.05秒）を超えたら新しいパーティクルを生成
	if (m_timer >= 0.00025f)
	{
		// 乱数の設定
		std::random_device seed;
		std::default_random_engine engine(seed());

		// ランダムな角度（0 から 2π まで）
		std::uniform_real_distribution<> angleDist(0, XM_2PI);

		// ランダムな速度の範囲を設定
		std::uniform_real_distribution<> speedDist(0.5f, 2.0f);

		// ランダムな角度
		float randAngleXY = static_cast<float>(angleDist(engine));
		float randAngleXZ = static_cast<float>(angleDist(engine));

		// ランダムな速度
		float speed = static_cast<float>(speedDist(engine));

		// ランダムな方向の速度ベクトル
		SimpleMath::Vector3 randomVelocity = speed * SimpleMath::Vector3(
			cosf(randAngleXY) * sinf(randAngleXZ),
			cosf(randAngleXZ),
			sinf(randAngleXY) * sinf(randAngleXZ)
		);



		if (m_type == ParticleUtility::Type::PLAYERTRAIL)// プレイヤーの弾の軌跡
		{
			// パーティクルの生成
			ParticleUtility pU(
				1.0f,  // 生存時間(s)
				m_bulletPosition, // 初期位置 (基準座標)
				randomVelocity, // 初期速度（ランダムな方向）
				SimpleMath::Vector3::One, // 加速度
				SimpleMath::Vector3::One, // 回転速度
				SimpleMath::Vector3(0, 0, 10), // 初期回転
				SimpleMath::Vector3(m_size, m_size, 0), // 初期スケール
				SimpleMath::Vector3(m_size / 10, m_size / 10, 0), // 最終スケール（小さくなる）
				SimpleMath::Vector4(0, 0.2, 1, .5), // 初期カラー（白）
				SimpleMath::Vector4(0, 1, 1, 0.5), // 最終カラー（白→透明）
				m_type // パーティクルのタイプ

			);
			// 生成したパーティクルをリストに追加
			m_particleUtility.push_back(pU);
		}

		if (m_type == ParticleUtility::Type::ENEMYTRAIL)// 敵の弾の軌跡
		{
			// パーティクルの生成
			ParticleUtility pU(
				1.0f,  // 生存時間(s)
				m_bulletPosition, // 初期位置 (基準座標)
				randomVelocity, // 初期速度（ランダムな方向）
				SimpleMath::Vector3::One, // 加速度
				SimpleMath::Vector3::One, // 回転速度
				SimpleMath::Vector3(0, 0, 10), // 初期回転
				SimpleMath::Vector3(1, 1, 0), // 初期スケール
				SimpleMath::Vector3(0, 0, 0), // 最終スケール（小さくなる）
				SimpleMath::Vector4(1, 0, 1, 1), // 初期カラー（白）
				SimpleMath::Vector4(0, 1, 1, 0.25), // 最終カラー（白→透明）
				m_type // パーティクルのタイプ

			);
			// 生成したパーティクルをリストに追加
			m_particleUtility.push_back(pU);
		}

		// タイマーをリセット
		m_timer = 0.0f;
	}
}

void Particle::BarrierBreak()
{
	if (m_timer >= 0.05f)  // バリアが壊れたときの破片は少し間隔が長めでもよい
	{
		std::random_device seed;
		std::default_random_engine engine(seed());
		std::uniform_real_distribution<> angleDist(0, XM_2PI); // 全方向ランダムな角度
		std::uniform_real_distribution<> speedDist(0.5f, 3.0f); // 粒子の速度範囲
		std::uniform_real_distribution<> sizeDist(1.5f, 5.0f);  // 破片のサイズをランダムに設定
		std::uniform_real_distribution<> heightDist(-1.0f, 1.0f); // 高さ方向（-1～1）のランダム範囲

		for (int i = 0; i < 20; ++i)  // 破片を複数生成
		{
			// ランダムな方向で飛ばすための球面座標
			float theta = static_cast<float>(angleDist(engine)); // 水平方向の角度 (0～2π)
			float phi = acosf(static_cast<float>(heightDist(engine))); // 垂直方向の角度 (0～π)

			// ランダムな速度
			float speed = static_cast<float>(speedDist(engine)) * 5.0f;

			// 球面座標系をデカルト座標系に変換して速度ベクトルを計算
			SimpleMath::Vector3 randomVelocity = speed * SimpleMath::Vector3(
				sinf(phi) * cosf(theta), // X成分
				cosf(phi),              // Y成分
				sinf(phi) * sinf(theta) // Z成分
			);

			// ランダムなサイズ
			float randomSize = static_cast<float>(sizeDist(engine));

			// パーティクルの生成
			ParticleUtility pU(
				2.0f,  // 破片の生存時間（短くして消えるように）
				m_bossPosition, // 初期位置 (基準座標)
				randomVelocity * 4, // 初期速度（ランダムな方向）
				SimpleMath::Vector3::Zero, // 重力加速度（Y軸方向に少し引っ張られる）
				SimpleMath::Vector3::Zero, // 回転速度
				SimpleMath::Vector3::Zero, // 初期回転
				SimpleMath::Vector3(randomSize, randomSize, randomSize), // ランダムなサイズ
				SimpleMath::Vector3(0.1f, 0.1f, 0.1f), // 最終スケール（小さくなる）
				SimpleMath::Vector4(1.0f, 1.0f, 0.3f, 1.0f),  // 初期カラー（オレンジっぽい）
				SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f), // 最終カラー（白→透明）
				ParticleUtility::Type::BARRIERDESTROYED // タイプを追加
			);
			m_particleUtility.push_back(pU);
		}
	}
}
