#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"Matrix.h"
#include"EnemyBullet.h"
#include<list>

class Player;

class Enemy {
public:
	~Enemy();

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	// 初期化
	void Initialaize(Model* model, uint32_t textureHandle);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection viewProjection);
	
	void Attack();

	//弾発射
	void Fire();

	void ApproathReset();

	static const int kFireInterval = 45;

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() { return bullets_;}

	// 衝突したら呼び出す関数
	void OnCollision();

	float radius = 1.5f;

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

	float size = 1.0f;

	Vector3 scale = {size, size, size};

	std::list<EnemyBullet*> bullets_;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//発射タイマー
	int32_t fireTimer = 0;
	
	Player* player_ = nullptr;


};
