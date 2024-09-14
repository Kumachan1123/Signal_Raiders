/*
	@file	BulletTrail.h
	@brief	弾の軌跡クラス
	作成者：くまち
*/
#pragma once
//前方宣言
class CommonResources;
#include "Game/CommonResources.h"
#include <vector>
#include <list>

#include <Game/ParticleUtility/ParticleUtility.h>
class BulletTrail
{
public:
	//	データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};
	//変数
public:

	DirectX::SimpleMath::Vector3 m_bulletPos;// 弾の座標

	// 共通リソース
	CommonResources* m_commonResources;
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//	変数
	DX::DeviceResources* m_pDR;
	float m_timer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	DirectX::SimpleMath::Matrix m_world;// ワールド行列
	DirectX::SimpleMath::Matrix m_view;// ビュー行列
	DirectX::SimpleMath::Matrix m_proj;	// プロジェクション行列

	DirectX::SimpleMath::Vector3 m_cameraPosition;// カメラの位置
	DirectX::SimpleMath::Vector3 m_cameraTarget;// カメラの注視点
	DirectX::SimpleMath::Vector3 m_cameraUp;// カメラの上方向
	DirectX::SimpleMath::Matrix m_billboard;// ビルボード行列

	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	std::list<ParticleUtility> m_particleUtility;
	//	関数
public:
	BulletTrail();
	~BulletTrail();
	void Initialize(CommonResources* resources);
	void LoadTexture(const wchar_t* path);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 eye,
		DirectX::SimpleMath::Vector3 up);
	//	Setter
	void SetBulletPosition(DirectX::SimpleMath::Vector3 bulletPos) { m_bulletPos = bulletPos; }
	void SetCameraPosition(DirectX::SimpleMath::Vector3 cameraPos) { m_cameraPosition = cameraPos; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 cameraTarget) { m_cameraTarget = cameraTarget; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 cameraUp) { m_cameraUp = cameraUp; }
private:
	void CreateShader();
	void Trail();

};