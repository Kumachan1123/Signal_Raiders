/*
*	@file	EnemyBullet.h
*	@brief	敵の弾クラス
*/
#pragma once
#ifndef ENEMY_BULLET_DEFINED
#define ENEMY_BULLET_DEFINED
// 標準ライブラリ
#include <memory>
#include <cassert>
#include <SimpleMath.h>
// 外部ライブラリ
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/ReadData.h>
#include <Libraries/Microsoft/DebugDraw.h>
// 自作ヘッダーファイル
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/CommonResources.h"
#include "Game/Particle/Particle.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/EnemyBullet/NormalBullet/NormalBullet.h"
#include "Game/Enemy/EnemyBullet/SpeedBullet/SpeedBullet.h"
#include "Game/Enemy/EnemyBullet/SpecialBullet/SpecialBullet.h"
#include "Game/Interface/IBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBulletFactory/EnemyBulletFactory.h"
#include "Game/Enemy/EnemyBullet/EnemyBulletType/EnemyBulletType.h"

// 前方宣言
class CommonResources;
class IEnemyBullet;
class NormalBullet;
class SpeedBullet;
class SpecialBullet;

// 敵の弾クラス
class EnemyBullet : public IBullet
{
public:
	// アクセサ
	// 「弾」境界球を取得
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	// 「弾」境界球を設定
	void SetBoundingSphere(DirectX::BoundingSphere sphere) { m_boundingSphere = sphere; }
	//時間を取得
	float GetTime()const { return m_time; }
	// 弾の種類を取得
	BulletType GetBulletType()const { return m_bulletType; }
	// 弾の種類を設定
	void SetBulletType(BulletType type) { m_bulletType = type; }
	// 弾の座標を取得
	DirectX::SimpleMath::Vector3 GetPosition()const { return m_position; }
	// 弾の座標を設定
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	// 弾の速度を取得
	DirectX::SimpleMath::Vector3 GetVelocity()const { return m_velocity; }
	// 弾の速度を設定
	void SetVelocity(DirectX::SimpleMath::Vector3 vel) { m_velocity = vel; }
	// 弾の方向を取得
	DirectX::SimpleMath::Vector3 GetDirection()const { return m_direction; }
	// 弾の方向を設定
	void SetDirection(DirectX::SimpleMath::Vector3 dir) { m_direction = dir; }
	// 弾の大きさを取得	
	float GetSize()const { return m_size; }
	// 弾の大きさを設定
	void SetSize(float size) { m_size = size; }
	// 敵の座標を取得
	DirectX::SimpleMath::Vector3 GetEnemyPosition()const { return m_enemyPosition; }
	// 敵の座標を設定
	void SetEnemyPosition(DirectX::SimpleMath::Vector3 pos) { m_enemyPosition = pos; }
	// 現在のターゲットの位置を取得
	DirectX::SimpleMath::Vector3 GetCurrentTarget()const { return m_currentTarget; }
	// 現在のターゲットの位置を設定
	void SetCurrentTarget(DirectX::SimpleMath::Vector3 target) { m_currentTarget = target; }
	// 弾の座標を取得
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	// 弾の座標を設定
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	// カメラの位置を設定
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	// カメラのターゲットを設定
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	// カメラの上ベクトルを設定
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }
	// 発射した敵のポインターを取得
	IEnemy* GetShooter() const { return m_pShooter; }
	// 発射した敵のポインターを設定
	void SetShooter(IEnemy* enemy) { m_pShooter = enemy; }
	// 弾発射時のターゲットの位置を取得
	DirectX::SimpleMath::Vector3 GetTarget() const { return m_target; }
	// 弾発射時のターゲットの位置を設定
	void SetTarget(DirectX::SimpleMath::Vector3 target) { m_target = target; }
	// プレイヤーからの距離を取得
	float GetDistance() const { return m_distance; }
	// プレイヤーからの距離を設定
	void SetDistance(float distance) { m_distance = distance; }
	// プレイヤーの角度を取得
	float GetAngle() const { return m_spiralAngle; }
	// プレイヤーの角度を設定
	void SetAngle(float angle) { m_spiralAngle = angle; }
	// 展開完了フラグを取得
	bool GetIsExpand() const { return m_isExpand; }
	// 展開完了フラグを設定
	void SetIsExpand(bool isExpand) { m_isExpand = isExpand; }
	// 発射フラグを取得
	bool GetIsShot() const { return m_isShot; }
	// 発射フラグを設定
	void SetIsShot(bool isShot) { m_isShot = isShot; }
	// 螺旋弾の回転方向を決める
	void SetRotateDirection(int direction) { m_rotateDirection = direction; }
	// SE音量を取得
	float GetSEVolume() const { return m_seVolume; }
	// SE音量を設定
	void SetSEVolume(float volume) { m_seVolume = volume; }
public:
	// public関数
	// コンストラクタ
	EnemyBullet(float size);
	// デストラクタ
	~EnemyBullet()override;
	// 初期化
	void Initialize(CommonResources* pCommonResources)override;
	// 更新
	void Update(float elapsedTime)override;
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// 影描画
	void RenderShadow(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// 境界球描画
	void RenderBoundingSphere(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 弾の生成
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir, const DirectX::SimpleMath::Vector3& target);
	// 弾が生成されてからの経過時間が寿命を超えたかどうかを判定する
	bool IsExpired() const;
private:
	// private関数
	//弾のワールド行列を作成
	DirectX::SimpleMath::Matrix BulletWorldMatrix();
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// シェーダー作成クラスのポインター
	CreateShader* m_pCreateShader;
	// 弾のワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 敵の座標
	DirectX::SimpleMath::Vector3 m_enemyPosition;
	// 弾の座標
	DirectX::SimpleMath::Vector3 m_position;
	// 弾の速さ
	DirectX::SimpleMath::Vector3 m_velocity;
	// 弾の大きさ
	float m_size;
	// 弾が飛ぶ方向
	DirectX::SimpleMath::Vector3 m_direction;
	// 弾の位置オフセット
	DirectX::SimpleMath::Vector3 m_positionOffSet;
	// 弾の敵との距離
	float m_distance;
	// 弾のモデル
	DirectX::Model* m_pModel;
	// 生存時間
	float m_time;
	// 経過時間
	float m_elapsedTime;
	// 発射した敵のポインター
	IEnemy* m_pShooter;
	// 「弾」境界ボックス
	DirectX::BoundingSphere m_boundingSphere;
	// 弾発射時のターゲットの位置
	DirectX::SimpleMath::Vector3 m_target;
	// 現在のターゲットの位置
	DirectX::SimpleMath::Vector3 m_currentTarget;
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// カメラのターゲット
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// カメラの上ベクトル
	float m_angle;
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// 弾の自転
	// 特殊弾の回転角度
	float m_spiralAngle;
	// 回転方向（螺旋弾用:+なら右回り、-なら左回り）
	int m_rotateDirection;
	// 高さ
	float m_height;
	// 角度
	// 展開完了フラグ
	bool m_isExpand;
	// 発射フラグ
	bool m_isShot;
	// 回転基準地点
	DirectX::SimpleMath::Vector3 m_basePos;
	// 回転速度
	float m_rotationSpeed;
	// 弾の速度
	float m_bulletSpeed;
	// 弾の軌道
	std::unique_ptr<Particle> m_bulletTrail;
	// 弾の種類別ポインター
	// 通常弾
	std::unique_ptr<NormalBullet> m_pNormalBullet;
	// 速い弾
	std::unique_ptr<SpeedBullet> m_pSpeedBullet;
	// 特殊弾
	std::unique_ptr<SpecialBullet> m_pSpecialBullet;
	// 弾の種類ごとに処理を変えるためのポインター
	std::unique_ptr<IEnemyBullet> m_pEnemyBullet;
	// モデルの影
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// 弾の種類
	BulletType m_bulletType;
	// SE音量
	float m_seVolume;
};
#endif //ENEMY_BULLET_DEFINED