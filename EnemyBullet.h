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
	const float t = 1.0f;

	const float kBulletSpeed = 0.9f;

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t texturerHandle_ = 0u;

	float size = 0.1f;

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
	Matrix matrix_;

	Vector3 vec;

	Matrix4x4 minusMatrix{0};

	Vector3 minusVelocity{0, 0, 0};

	Vector3 velocityZ{0, 0, 0};

	Vector3 playerPos = {};

	Vector3 toPlayer = {};
};
