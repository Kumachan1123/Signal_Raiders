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
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/ParticleUtility/ParticleUtility.h"
#include  "Game/KumachiLib/BinaryFile/BinaryFile.h"
//前方宣言
class CommonResources;
class Particle
{
public:// 構造体
	//シェーダーに送るコンスタントバッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix	 matWorld;	// ワールド行列
		DirectX::SimpleMath::Matrix	 matView;	// ビュー行列
		DirectX::SimpleMath::Matrix	 matProj;	// プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;	// 色
		DirectX::SimpleMath::Vector4 count;     // カウント
		DirectX::SimpleMath::Vector4 height;    // 高さ
		DirectX::SimpleMath::Vector4 width;     // 幅
	};
public:	// アクセサ
	// 弾の座標を設定
	void SetBulletPosition(const DirectX::SimpleMath::Vector3& bulletPos) { m_bulletPosition = bulletPos; }
	// ボスの座標を設定
	void SetBossPosition(const DirectX::SimpleMath::Vector3& bossPos) { m_bossPosition = bossPos; }
	// カメラの位置を設定
	void SetCameraPosition(const DirectX::SimpleMath::Vector3& cameraPos) { m_cameraPosition = cameraPos; }
	// カメラの注視点を設定
	void SetCameraTarget(const DirectX::SimpleMath::Vector3& cameraTarget) { m_cameraTarget = cameraTarget; }
	// カメラの上方向を設定
	void SetCameraUp(const DirectX::SimpleMath::Vector3& cameraUp) { m_cameraUp = cameraUp; }
	// シールド破壊のサイズを設定
	void SetBarrierBreakSize(float size) { m_barrierBreakSize = size; }
public:	// public関数
	// コンストラクタ
	Particle(ParticleUtility::Type type, float size);
	// デストラクタ
	~Particle();
	// 初期化
	void Initialize(CommonResources* resources);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// ビルボード行列の作成
	void CreateBillboard(
		const DirectX::SimpleMath::Vector3& target,
		const DirectX::SimpleMath::Vector3& eye,
		const DirectX::SimpleMath::Vector3& up);
private:// private関数
	// シェーダー作成
	void CreateShaders();
	// 軌跡
	void Trail();
	// バリア破壊
	void BarrierBreak();
private:// 定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:// public変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
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
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
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