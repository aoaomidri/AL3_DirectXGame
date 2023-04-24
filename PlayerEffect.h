#pragma once
#include "Model.h"
#include "WorldTransform.h"

class PlayerEffect {
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();

	// 衝突したら呼び出す関数
	void OnCollision();

	float radius = 0.001f;

	void Charge(const Vector3& position);
	// 弾のエフェクト
	void AttackEffect(int number);

	void MoveEffect();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	float size = 0.2f;

	Vector3 scale = {size, size, size};

	Vector3 velocity_;
	// 寿命
	static const int32_t kLifeTime = 40;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = true;

	int number_;
};
