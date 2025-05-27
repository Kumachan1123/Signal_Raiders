/*
	@file	Boss.h
	@brief	ボスクラス
*/
#pragma once
// 標準ライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
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
class BossAI;
class BossModel;
class Boss : public IBossLogic
{
public:
	// public関数
	Boss(BossBase* pBoss, CommonResources* commonResources);// コンストラクタ
	~Boss();// デストラクタ
	void Initialize()override; // 初期化(Boss, LastBossにてモデルを生成)
	void ChangeState()override;// 状態変更(Boss,LastBossにて状態を変更)
	void Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// 描画(Boss, LastBossにて描画)
	void BulletPositioning()override;	// 弾の発射位置を決定
	void CreateBullet();// 弾を生成
	void CreateCenterBullet(BulletType type);// 中央の弾を生成
	void CreateLeftBullet(BulletType type);// 左の弾を生成
	void CreateRightBullet(BulletType type);// 右の弾を生成
private:
	// privateメンバ変数
	CommonResources* m_pCommonResources;	// 共通リソース
	std::unique_ptr<BossModel>		m_pBossModel;//モデル
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// HPバー
	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	DirectX::BoundingSphere m_bossBS;	//敵の境界球
	DirectX::SimpleMath::Matrix m_matrix;// マトリクス
	// 弾の発射位置
	DirectX::SimpleMath::Vector3 m_bulletPosCenter;	// 中央
	DirectX::SimpleMath::Vector3 m_bulletPosLeft;	// 左
	DirectX::SimpleMath::Vector3 m_bulletPosRight;	// 右
	// 発射位置を回転させるためのクォータニオン
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// 弾のタイプ
	BulletType m_bulletType;// EnemyBulletクラスに送る
	// 弾の飛ぶ方向
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// 時間
	float m_time;
	// プレイヤーのカメラの情報
	DirectX::SimpleMath::Vector3 m_cameraEye;// カメラの位置
	DirectX::SimpleMath::Vector3 m_cameraTarget;// カメラの注視点
	DirectX::SimpleMath::Vector3 m_cameraUp;	// カメラの上方向
	// ボスベースクラスのポインタ
	BossBase* m_pBossBase;
};