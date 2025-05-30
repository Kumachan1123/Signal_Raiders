/*
*  @file LastBoss.h
*  @brief ラスボスクラス
*/
#pragma once
// 標準ライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Interface/IBossLogic.h"
#include "Game/Enemy/BossBase/BossBase.h"
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/LastBoss/LastBossModel/LastBossModel.h"

//前方宣言
class CommonResources;
class BossAI;
class LastBossModel;
class BossBase;
class LastBoss : public IBossLogic
{
public:// public関数

	LastBoss(BossBase* pBoss, CommonResources* commonResources);// コンストラクタ
	~LastBoss()override;// デストラクタ
	void Initialize()override;                                             // Boss, LastBossにてモデルを生成
	void ChangeState()override;                                // Boss,LastBossにて状態を変更
	void Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;        // Boss, LastBossにて描画
	void BulletPositioning()override;	// 弾の発射位置を決定
	void CreateBullet()override;// 弾を生成
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	//モデル
	std::unique_ptr<LastBossModel>		m_pBossModel;
	// HPバー
	std::unique_ptr<EnemyHPBar>		m_pHPBar;
	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	DirectX::BoundingSphere m_bossBS;	//敵の境界球
	DirectX::SimpleMath::Matrix m_matrix;// マトリクス
	// 弾の発射位置
	// 右上
	DirectX::SimpleMath::Vector3 m_bulletPosRightUp;
	// 右下
	DirectX::SimpleMath::Vector3 m_bulletPosRightDown;
	// 左上
	DirectX::SimpleMath::Vector3 m_bulletPosLeftUp;
	// 左下
	DirectX::SimpleMath::Vector3 m_bulletPosLeftDown;
	// 発射位置を回転させるためのクォータニオン
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// 弾のタイプ
	BulletType m_bulletType;// EnemyBulletクラスに送る
	// 弾の飛ぶ方向
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// 時間
	float m_time;
	// カメラの視点 
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// カメラのターゲット
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// カメラの上ベクトル
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// ボスのベースクラス
	BossBase* m_pBossBase;

};