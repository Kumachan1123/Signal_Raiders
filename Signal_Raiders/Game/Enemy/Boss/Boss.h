/*
	@file	Boss.h
	@brief	ボスクラス
*/
#pragma once
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Interface/IBossLogic.h"
#include "Game/Enemy/BossBase/BossBase.h"
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/Boss/BossModel/BossModel.h"
//前方宣言
class CommonResources;
class BossBase;
class BossModel;
class BossAI;
class Boss : public IBossLogic
{
public:	// public関数
	// コンストラクタ
	Boss(BossBase* pBoss, CommonResources* commonResources);
	// デストラクタ
	~Boss();
	// 初期化
	void Initialize()override;
	// 状態変更
	void ChangeState()override;
	// 描画
	void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// 弾の発射位置を決定
	void BulletPositioning()override;
	// 弾を生成
	void CreateBullet();
	// 中央の弾を生成
	void CreateCenterBullet(BulletType type);
	// 左の弾を生成
	void CreateLeftBullet(BulletType type);
	// 右の弾を生成
	void CreateRightBullet(BulletType type);
private:// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	//モデル
	std::unique_ptr<BossModel>		m_pBossModel;
	// HPバー
	std::unique_ptr<EnemyHPBar>		m_pHPBar;
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 回転
	DirectX::SimpleMath::Vector3 m_rotate;
	// 敵の境界球
	DirectX::BoundingSphere m_bossBS;
	// マトリクス
	DirectX::SimpleMath::Matrix m_matrix;
	// 弾の発射位置: 中央
	DirectX::SimpleMath::Vector3 m_bulletPosCenter;
	// 弾の発射位置: 左
	DirectX::SimpleMath::Vector3 m_bulletPosLeft;
	// 弾の発射位置: 右
	DirectX::SimpleMath::Vector3 m_bulletPosRight;
	// 発射位置を回転させるためのクォータニオン
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	//  EnemyBulletクラスに送る弾のタイプ
	BulletType m_bulletType;
	// 弾の飛ぶ方向
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// 時間
	float m_time;
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// カメラのターゲット
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// カメラの上方向
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// ボスベースクラスのポインタ
	BossBase* m_pBossBase;
};