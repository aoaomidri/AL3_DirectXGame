#include "EnemyBullet.h"
#include<assert.h>
#include"Player.h"
#include"ImGuiManager.h"


void EnemyBullet::Initialize(Model* model, Vector3& position, const Vector3& velocity) { 
	assert(model);

	model_ = model;

	texturerHandle_ = TextureManager::Load("Red.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;

}

void EnemyBullet::Update() { 
	assert(player_);
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	
	playerPos = player_->GetPlayerWorldPosition();

	ImGui::Begin("Player");
	

	toPlayer = playerPos - worldTransform_.translation_;

	toPlayer = matrix_.Normalize(toPlayer);
	velocity_ = matrix_.Normalize(velocity_);

	ImGui::Text("position:(%+.2f,%+.2f,%+.2f)", velocity_.x, velocity_.y, velocity_.z);
	ImGui::End();
	velocity_ = vec.Mutiply(matrix_.Slerp(velocity_, toPlayer, t), kBulletSpeed);

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

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}