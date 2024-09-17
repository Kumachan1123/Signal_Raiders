/*
	@file	BulletTrail.cpp
	@brief	弾の軌跡クラス
	作成者：くまち
*/
#include "pch.h"
#include "Game/BulletTrail/BulletTrail.h"
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

const std::vector<D3D11_INPUT_ELEMENT_DESC> BulletTrail::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

BulletTrail::BulletTrail(ParticleUtility::Type type)
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
{
}

BulletTrail::~BulletTrail()
{
}

void BulletTrail::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	auto device = m_pDR->GetD3DDevice();
	//	シェーダーの作成
	CreateShader();
	// 画像の読み込み
	LoadTexture(L"Resources/Textures/Trail2.png");

	//	プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);
}

void BulletTrail::CreateShader()
{
	ID3D11Device* device = m_pDR->GetD3DDevice();
	// コンパイルされたシェーダーの読み込み
	kumachi::BinaryFile VS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Trail/TrailVS.cso");
	kumachi::BinaryFile PS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Trail/TrailPS.cso");
	kumachi::BinaryFile GS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Trail/TrailGS.cso");

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
}



// 画像の読み込み
void BulletTrail::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.GetAddressOf());
	m_texture.push_back(texture);
}

void BulletTrail::Update(float elapsedTime)
{
	m_timer += elapsedTime;
	//	軌跡の更新
	Trail();
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

void BulletTrail::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
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
		VertexPositionColorTexture vPCT;
		//	表示するパーティクルの中心座標のみを入れる。
		//	→つまり、C++で用意しているデータだけではテクスチャを表示できない。
		//	　→ジオメトリシェーダを使う事前提のデータ、ということ
		vPCT.position = XMFLOAT3(li.GetPosition());
		//	テクスチャの色
		vPCT.color = XMFLOAT4(li.GetNowColor());


		//	現在のテクスチャのスケールを「XMFLOAT2」のXに入れる。
		//	Yは使っていないし、そもそものTextureのUV座標とは違う使い方になっていることに注意
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_vertices.push_back(vPCT);
	}

	//	表示する点がない場合は描画を終わる
	if (m_vertices.empty())
	{
		return;
	}
	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose();
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	半透明描画指定		補間アルファ合成
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);

	//	カリングはなし
	context->RSSetState(m_states->CullCounterClockwise());

	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	ピクセルシェーダにテクスチャを登録する。
	for (int i = 0; i < m_texture.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//	指定した座標を中心に、シェーダ側で板ポリゴンを生成・描画させる
	m_batch->Begin();

	//	ここまでの処理の関係上、以下は使えない
	//m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);

	//	ジオメトリシェーダでPointを受け取ることになっているため、
	//	ここではD3D11_PRIMITIVE_TOPOLOGY_POINTLISTを使う
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());

	m_batch->End();

	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

/// <summary>
/// ビルボード作成関数
/// </summary>
/// <param name="target">カメラターゲット（注視点）</param>
/// <param name="eye">カメラアイ（カメラ座標）</param>
/// <param name="up">上向きベクトル（基本はYのみ１のベクトル）</param>
void BulletTrail::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
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
void BulletTrail::Trail()
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



		if (m_type == ParticleUtility::Type::PLAYERTRAIL)
		{
			// パーティクルの生成
			ParticleUtility pU(
				1.0f,  // 生存時間(s)
				m_bulletPos, // 初期位置 (基準座標)
				randomVelocity, // 初期速度（ランダムな方向）
				SimpleMath::Vector3::One, // 加速度
				SimpleMath::Vector3::One, // 回転速度
				SimpleMath::Vector3(0, 0, 10), // 初期回転
				SimpleMath::Vector3(1, 1, 0), // 初期スケール
				SimpleMath::Vector3(0, 0, 0), // 最終スケール（小さくなる）
				SimpleMath::Vector4(0, 0.2, 1, .5), // 初期カラー（白）
				SimpleMath::Vector4(0.5, 1, 1, 0), // 最終カラー（白→透明）
				m_type // パーティクルのタイプ

			);
			// 生成したパーティクルをリストに追加
			m_particleUtility.push_back(pU);
		}

		if (m_type == ParticleUtility::Type::ENEMYTRAIL)
		{
			// パーティクルの生成
			ParticleUtility pU(
				1.0f,  // 生存時間(s)
				m_bulletPos, // 初期位置 (基準座標)
				randomVelocity, // 初期速度（ランダムな方向）
				SimpleMath::Vector3::One, // 加速度
				SimpleMath::Vector3::One, // 回転速度
				SimpleMath::Vector3(0, 0, 10), // 初期回転
				SimpleMath::Vector3(1, 1, 0), // 初期スケール
				SimpleMath::Vector3(0, 0, 0), // 最終スケール（小さくなる）
				SimpleMath::Vector4(1, 0, 1, 1), // 初期カラー（白）
				SimpleMath::Vector4(1, 1, 1, 0), // 最終カラー（白→透明）
				m_type // パーティクルのタイプ

			);
			// 生成したパーティクルをリストに追加
			m_particleUtility.push_back(pU);
		}

		// タイマーをリセット
		m_timer = 0.0f;
	}
}

