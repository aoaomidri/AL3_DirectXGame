#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include"Matrix.h"

class Player;

class EnemyBullet {
public:

	void Initialize(Model* model, Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewpprojection);

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }

	// 衝突したら呼び出す関数
	void OnCollision();

	float radius = 1.0f;
	//1フレームでの保管割合
	const float t = 0.1f;

	const float kBulletSpeed = 1.0f;

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t texturerHandle_ = 0u;

	float size = 0.5f;

	Vector3 scale = {0.5f, 0.5f, 3.0f};

	Vector3 velocity_;
	// 寿命
	static const int32_t kLifeTime = 60 * 3;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

	Player* player_ = nullptr;

	// 行列の作成
	Matrix matrix;
};
