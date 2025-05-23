/*
*	@file	PlayerBullet.h
*	@brief	プレイヤーの弾クラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
// DirectXライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include <DeviceResources.h>
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/Microsoft/ReadData.h"
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib//DrawCollision/DrawCollision.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Particle/Particle.h"
#include "Game/ParticleUtility/ParticleUtility.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/Interface/IBullet.h"
//前方宣言
class CommonResources;

class PlayerBullet : public IBullet
{
public:
	// アクセサ
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	float GetPlayerPositionY()const { return m_position.y; }
	float GetTime()const { return m_time; }
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }
	void SetAdditionalDamage(int damage) { m_additionalDamage = damage; }
	int GetAdditionalDamage()const { return m_additionalDamage; }
	// public関数
	PlayerBullet();
	~PlayerBullet()override;
	void Initialize(CommonResources* resources)override;// 初期化
	void Update(float elapsedTime)override;// 更新
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// 描画
	void RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// 影の描画
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// 当たり判定の描画
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir);// 弾を生成
	bool IsExpired() const { return m_time >= BulletParameters::PLAYER_BULLET_LIFETIME; }// 弾が生成されてからの経過時間が寿命を超えたかどうかを判定する
	int Damage()const { return BulletParameters::DAMAGE + m_additionalDamage; }// 敵にダメージを与える


private:
	// private変数
	// 軌跡用の履歴を保存する配列
	std::vector<DirectX::SimpleMath::Vector3> m_trailPositions;
	// 共通リソース
	CommonResources* m_commonResources;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 「弾」境界ボックス
	DirectX::BoundingSphere m_boundingSphere;
	// モデルの影
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// 弾の自転
	float m_angle;
	// 弾の軌跡ポインター
	std::unique_ptr<Particle> m_bulletTrail;
	// 追加ダメージ
	int m_additionalDamage;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_trailTexture;
	// カメラ
	DirectX::SimpleMath::Vector3 m_cameraEye;// カメラの位置
	DirectX::SimpleMath::Vector3 m_cameraTarget;	// カメラの向き
	DirectX::SimpleMath::Vector3 m_cameraUp;	// カメラの上方向
	// 弾のパラメータ
	DirectX::SimpleMath::Vector3 m_position;		// 弾の座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 弾の速さ
	DirectX::SimpleMath::Vector3 m_direction;		// 弾が飛ぶ方向
	float m_time;									// 生存時間
	// 弾モデル
	std::unique_ptr<DirectX::Model> m_model;
};