/*
*	@file	Particle.h
*	@brief	パーティクルクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <algorithm>
#include <random>
#include <list>
// DirectXのライブラリ
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// 外部ライブラリ
#include <DeviceResources.h>
#include "Game/CommonResources.h"
// 自作ヘッダーファイル
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include <Game/ParticleUtility/ParticleUtility.h>
#include  "Game/KumachiLib/BinaryFile/BinaryFile.h"
//前方宣言
class CommonResources;
class Particle
{
public:
	// 構造体
	struct ConstBuffer//シェーダーに送るコンスタントバッファ
	{
		DirectX::SimpleMath::Matrix	 matWorld;	// ワールド行列
		DirectX::SimpleMath::Matrix	 matView;	// ビュー行列
		DirectX::SimpleMath::Matrix	 matProj;	// プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;	// 色
		DirectX::SimpleMath::Vector4 count;     // カウント
		DirectX::SimpleMath::Vector4 height;    // 高さ
		DirectX::SimpleMath::Vector4 width;     // 幅
	};
public:
	// アクセサ
	void SetBulletPosition(DirectX::SimpleMath::Vector3 bulletPos) { m_bulletPosition = bulletPos; }// 弾の座標を設定
	void SetBossPosition(DirectX::SimpleMath::Vector3 bossPos) { m_bossPosition = bossPos; }// ボスの座標を設定
	void SetCameraPosition(DirectX::SimpleMath::Vector3 cameraPos) { m_cameraPosition = cameraPos; }// カメラの位置を設定
	void SetCameraTarget(DirectX::SimpleMath::Vector3 cameraTarget) { m_cameraTarget = cameraTarget; }// カメラの注視点を設定
	void SetCameraUp(DirectX::SimpleMath::Vector3 cameraUp) { m_cameraUp = cameraUp; }// カメラの上方向を設定
	void SetBarrierBreakSize(float size) { m_barrierBreakSize = size; }// シールド破壊のサイズを設定
public:
	// public関数
	Particle(ParticleUtility::Type type, float size);// コンストラクタ
	~Particle();// デストラクタ
	void Initialize(CommonResources* resources);// 初期化
	void LoadTexture(const wchar_t* path);// テクスチャの読み込み
	void Update(float elapsedTime);// 更新
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// 描画
	void CreateBillboard(// ビルボード行列の作成
		DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 eye,
		DirectX::SimpleMath::Vector3 up);
private:
	void CreateShaders();// シェーダー作成
	void Trail();// 軌跡
	void BarrierBreak();// バリア破壊
public:
	// public変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// 経過時間
	float m_timer;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// テクスチャハンドルの配列
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
	// コンスタントバッファ（構造体）
	ConstBuffer m_constantBuffer;
	// フレームの行数と列数
	int m_frameRows;
	int m_frameCols;
	// フレーム数
	int m_anim;
	// アニメーションの速度
	float m_animSpeed;
	// アニメーション時間
	float m_animTime;
	// フレーム時間
	float m_elapsedTime;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	//	ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	// 弾の座標
	DirectX::SimpleMath::Vector3 m_bulletPosition;
	// ボスの座標
	DirectX::SimpleMath::Vector3 m_bossPosition;
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	// カメラの注視点
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// カメラの上方向
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// ビルボード行列
	DirectX::SimpleMath::Matrix m_billboard;
	// 頂点
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	// パーティクルのユーティリティ
	std::list<ParticleUtility> m_particleUtility;
	// パーティクルタイプ
	ParticleUtility::Type m_type;
	// パーティクルのサイズ
	float m_size;
	// シールド破壊のサイズ
	float m_barrierBreakSize;
};