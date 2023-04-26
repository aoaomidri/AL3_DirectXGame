#include "EnemyBullet.h"
#include<assert.h>
#include"Player.h"

void EnemyBullet::Initialize(Model* model, Vector3& position, const Vector3& velocity) { 
	assert(model);

	model_ = model;

	texturerHandle_ = TextureManager::Load("Red.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;

}

void EnemyBullet::Update() { 
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	/*if (velocity_.x <= 0){
	    isDead_ = true;
	} else if (velocity_.y <= 0) {
	    isDead_ = true;
	} else if (velocity_.z <= 0) {
	    isDead_ = true;
	}*/
	Matrix matrix_;

	Matrix4x4 minusMatrix{0};

	Vector3 minusVelocity{0,0,0};

	Vector3 velocityZ{0, 0, 0};

	

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	minusVelocity = {0, -worldTransform_.rotation_.y, 0};

	minusMatrix = matrix_.MakeRotateMatrixY(minusVelocity);

	velocityZ = matrix_.TransformNormal(velocity_, minusMatrix);

	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	worldTransform_.AddTransform(velocity_);

	worldTransform_.UpdateMatrix(scale);
}

void EnemyBullet::Draw(const ViewProjection& viewpprojection) { 
	
	model_->Draw(worldTransform_, viewpprojection, texturerHandle_);

}

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() {

	Vector3 worldPos(0, 0, 0);

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}