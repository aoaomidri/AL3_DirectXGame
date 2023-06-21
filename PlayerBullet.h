#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"PlayerEffect.h"
#include<list>


class PlayerBullet {
public:
	~PlayerBullet();

	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();

	//衝突したら呼び出す関数
	void OnCollision();

	// エフェクトリストを取得
	const std::list<PlayerEffect*>& GetEffects() { return effect_; }

	// 弾のエフェクト
	void AttackEffect(int number);

	float radius = 1.0f;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	std::list<PlayerEffect*> effect_;

	float size = 0.8f;

	Vector3 scale = {size, size, size};

	Vector3 velocity_;
	//寿命
	static const int32_t kLifeTime = 180 ;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	int rotateAngle = 10;

	int numberA = 0;

	int numberB = 60;

	int numberC = 120;

	int numberD = 180;

	int numberE = 240;

	int numberF = 300;
};
