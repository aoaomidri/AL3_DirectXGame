#include "EnemyEffect.h"
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>


void EnemyEffect::Initialize(Model* model, const Vector3& position) {
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void EnemyEffect::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	} else {
		isDead_ = false;
	}

	worldTransform_.AddTransform(velocity_);

	worldTransform_.UpdateMatrix(scale);
}

void EnemyEffect::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyEffect::OnCollision() { isDead_ = true; }

Vector3 EnemyEffect::GetWorldPosition() {

	Vector3 worldPos(0, 0, 0);

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void EnemyEffect::Charge(const Vector3& position) {

	// ’e‚Ì‘¬“x
	const float kBulletSpeed = 0.7f;

	Vector3 velocity = {0, 0, 0};

	Vector3 vector = {
	    position.x - worldTransform_.translation_.x, position.y - worldTransform_.translation_.y,
	    position.z - worldTransform_.translation_.z};

	float bulletNorm = sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

	if (bulletNorm != 0.0f) {
		velocity = {
		    (vector.x / bulletNorm) * kBulletSpeed, (vector.y / bulletNorm) * kBulletSpeed,
		    (vector.z / bulletNorm) * kBulletSpeed};
	}

	velocity_ = velocity;
}

void EnemyEffect::AttackEffect(int number) {
	Vector3 velocity = {0, 0, 0};
	const float kBulletSpeed = 0.2f;
	number_ = number;
	int angle = (number_) % 360;

	float top = angle * static_cast<float>(M_PI) / 180.0f;
	float radius = 0;
	radius += kBulletSpeed;

	velocity.x = radius * std::cosf(top);
	velocity.y = radius * std::sinf(top);

	velocity_ = velocity;
}

void EnemyEffect::HitEffect() {
	Vector3 velocity = {0, 0, 0};
	float numberX = (rand() % 11 - 5) / 5.0f;
	float numberY = (rand() % 11 - 5) / 5.0f;
	float numberZ = (rand() % 11 - 5) / 5.0f;

	velocity = {numberX, numberY, numberZ};

	velocity_ = velocity;
}