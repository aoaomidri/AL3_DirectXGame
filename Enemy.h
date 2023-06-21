#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"Matrix.h"
#include"EnemyEffect.h"
#include<list>

class Player;

class GameScene;

class Enemy {
public:
	~Enemy();

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	// 初期化
	void Initialaize(Model* model, uint32_t textureHandle, Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection viewProjection);
	
	//弾発射
	void Fire();

	void ApproathReset();

	// 弾のエフェクト
	void HitEffect();

	static const int kFireInterval = 60;
	//死んだかどうか
	bool IsDead() const { return isDead_; }
	
	// 衝突したら呼び出す関数
	void OnCollision();

	Vector3 GetWorldPosition();

	float radius = 1.5f;

	//セッター
	void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};
	// 行列の作成
	Matrix matrix;
	//メンバ関数ポインタのテーブル
	static void (Enemy::*eFuncTable[])();
	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	float size = 1.0f;

	Vector3 scale = {size, size, size};

	//フェーズ
	Phase phase_ = Phase::Approach;
	//発射タイマー
	int32_t fireTimer = 0;

	int32_t leaveTime = 600;
	
	Player* player_ = nullptr;
		// デスフラグ
	bool isDead_ = false;

	bool effectOn = true;

	Vector3 effectPos = {100, 100, -100};

	void ApproachUpdate();

	void LeaveUpdate();
};
