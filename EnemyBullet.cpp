#include "EnemyBullet.h"
#include<assert.h>

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