#include "PlayerEffect.h"
#include <assert.h>
#include"Matrix.h"
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
	} else {
		isDead_ = false;
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

	Matrix matrix_;

	Matrix4x4 minusMatrix{0};

	Vector3 minusVelocity{0,0,0};

	Vector3 velocityZ{0, 0, 0};

	// ’e‚Ì‘¬“x
	const float kEffectSpeed = 0.7f;

	Vector3 velocity = {0, 0, 0};

	Vector3 vector = {
	    position.x - worldTransform_.translation_.x, 
		position.y - worldTransform_.translation_.y,
	    position.z - worldTransform_.translation_.z
	};

	float effectNorm =
		sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

	if (effectNorm != 0.0f) {
		velocity = {
			(vector.x / effectNorm) * kEffectSpeed, 
			(vector.y / effectNorm) * kEffectSpeed,
			(vector.z / effectNorm) * kEffectSpeed
		};
	}
	size = 0.1f;
	scale = {size, size, size * 5};

	velocity_ = velocity;

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	minusVelocity = {0, -worldTransform_.rotation_.y, 0};

	minusMatrix = matrix_.MakeRotateMatrixY(minusVelocity);

	velocityZ = matrix_.TransformNormal(velocity_, minusMatrix);

	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	worldTransform_.AddTransform(velocity_);

	worldTransform_.UpdateMatrix(scale);

}

void PlayerEffect::AttackEffect(int number) {
	Vector3 velocity = {0, 0, 0};
	const float kBulletSpeed = 0.2f;
	int angle = (number) % 360;

	float top = angle * static_cast<float>(M_PI) / 180.0f;
	float radius = 0;
	radius += kBulletSpeed;

	velocity.x = radius * std::cosf(top);
	velocity.y = radius * std::sinf(top);

	velocity_ = velocity;
}

void PlayerEffect::MoveEffect() {
	Vector3 velocity = {0, 0, 0};
	float numberX = (rand() % 11 - 5) / 10.0f;
	float numberY = (rand() % 11 - 5) / 10.0f;
	float numberZ = (rand() % 11 - 5) / 10.0f;

	velocity = {numberX, numberY, numberZ};
	
	velocity_ = velocity;
}