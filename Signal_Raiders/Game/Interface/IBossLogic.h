#pragma once
#ifndef IBOSSLOGIC_DEFINED
#define IBOSSLOGIC_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
class Player;
class CommonResources;
class BulletManager;

class IBossLogic
{
public:
	virtual ~IBossLogic() = default;                                          // デストラクタ
	//virtual void SetPlayer(Player* pPlayer) = 0;                               // プレイヤー設定
	//virtual void SetEnemyHP(int hp) = 0;                                    // HP設定
	//virtual void SetAudioManager(class AudioManager* audioManager) = 0;        // オーディオマネージャー設定
	//virtual void SetBulletManager(BulletManager* bulletManager) = 0;          // 弾マネージャー設定
	//virtual void SetBulletType(int bossBulletType) = 0;                        // 弾タイプ設定
	virtual void Initialize() = 0;                                             // 初期化
	virtual void Update(float elapsedTime) = 0;                                // 更新
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;        // 描画
	virtual void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0; // 衝突判定描画
};
#endif // IBOSSLOGIC_DEFINED
