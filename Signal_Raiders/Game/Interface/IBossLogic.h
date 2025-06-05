/*
*	@file IBossLogic.h
*	@brief ボスロジックのインターフェースクラス
*/
#pragma once
#ifndef IBOSSLOGIC_DEFINED
#define IBOSSLOGIC_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>

// 前方宣言
class Player;
class CommonResources;
class BulletManager;

// ボスロジックのインターフェースクラス
class IBossLogic
{
public:
	// publicメンバ関数
	// デストラクタ
	virtual ~IBossLogic() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 状態変更
	virtual void ChangeState() = 0;
	// 描画
	virtual void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	// 弾の発射位置を決定
	virtual void BulletPositioning() = 0;
	// 弾を生成
	virtual	void CreateBullet() = 0;
};
#endif // IBOSSLOGIC_DEFINED