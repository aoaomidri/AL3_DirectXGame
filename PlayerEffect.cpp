#include "PlayerEffect.h"
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>


void PlayerEffect::Initialize(Model* model, const Vector3& position) {
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

}

void PlayerEffect::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.AddTransform(velocity_);

	worldTransform_.UpdateMatrix(scale);
}

void PlayerEffect::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerEffect::OnCollision() { isDead_ = true; }

Vector3 PlayerEffect::GetWorldPosition() {

	Vector3 worldPos(0, 0, 0);

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerEffect::Charge(const Vector3& position) {

	// 弾の速度
	const float kBulletSpeed = 0.7f;
	int numberX = rand();
	int numberY = rand();
	int numberZ = rand();

	Vector3 velocity = {0, 0, 0};

	Vector3 vector = {
	    position.x - worldTransform_.translation_.x, 
		position.y - worldTransform_.translation_.y,
	    position.z - worldTransform_.translation_.z
	};

	float bulletNorm =
		sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

	if (bulletNorm != 0.0f) {

		velocity = {
			(vector.x / bulletNorm) * kBulletSpeed, 
			(vector.y / bulletNorm) * kBulletSpeed,
			(vector.z / bulletNorm) * kBulletSpeed
		};
	}

	velocity_ = velocity;

}

void PlayerEffect::AttackEffect(int number) {
	Vector3 velocity = {0, 0, 0};
	const float kBulletSpeed = 0.2f;
	number_ = number;
	int angle = (number_) % 360;

	float top = angle * static_cast<float>(M_PI) / 180.0f;
	float radius = 0;
	radius += kBulletSpeed;

	// 自弾リストの取得
	velocity.x = radius * std::cosf(top);
	velocity.y = radius * std::sinf(top);

	velocity_ = velocity;
}