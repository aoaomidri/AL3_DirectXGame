#include "Enemy.h"
#include<imgui.h>
void Enemy::Initialize(const std::vector<Model*>& models) { 
	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = {20.0f, 0.0f, 20.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	worldTransformL_parts_.Initialize();
	worldTransformL_parts_.rotation_ = {1.57f, 0.0f, 0.0f};
	worldTransformR_parts_.Initialize();
	worldTransformR_parts_.rotation_ = {1.57f, 0.0f, 0.0f};
}

void Enemy::Update() {	
	
	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.3f};

	move = vector.TransformNormal(move, worldTransform_.matWorld_);
	
	if (move.x != 0.0f || move.z != 0.0f) {
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);

		worldTransformL_parts_.rotation_.y = std::atan2(move.x, move.z);
		worldTransformR_parts_.rotation_.y = std::atan2(move.x, move.z);
	}

	worldTransform_.rotation_.y += static_cast<float>(M_PI) / 180.0f;
	worldTransformL_parts_.rotation_.x += 1.0f / (static_cast<float>(M_PI) * 2.0f);
	worldTransformR_parts_.rotation_.x += 1.0f / (static_cast<float>(M_PI) * 2.0f);

	worldTransform_.AddTransform(move);

	/*ImGui::Begin("EnemyParts");
	ImGui::DragFloat3("EnemyPosition", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("Enemy_Rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Move", &move.x, 0.01f);*/
	//ImGui::DragFloat("EnemyPartsL_Rotate", &worldTransformL_parts_.rotation_.x, 0.01f);
	//ImGui::DragFloat("EnemyPartsR_Rotate", &worldTransformR_parts_.rotation_.x ,0.01f);
	//ImGui::End();
	

	Matrix4x4 EnemyRotateMatrix = matrix.MakeRotateMatrix(worldTransform_.rotation_);
	Vector3 L_parts_offset = {3.0f, 3.0f, 0.0f};
	Vector3 R_parts_offset = {-3.0f, 3.0f, 0.0f};

	L_parts_offset = vector.TransformNormal(L_parts_offset, EnemyRotateMatrix);
	R_parts_offset = vector.TransformNormal(R_parts_offset, EnemyRotateMatrix);

	worldTransformL_parts_.translation_ = worldTransform_.translation_ + L_parts_offset;
	worldTransformR_parts_.translation_ = worldTransform_.translation_ + R_parts_offset;

	

	worldTransform_.UpdateMatrix(scale);
	worldTransformL_parts_.UpdateMatrix(scale);
	worldTransformR_parts_.UpdateMatrix(scale);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	models_[0]->Draw(worldTransform_, viewProjection);
	models_[1]->Draw(worldTransformL_parts_, viewProjection);
	models_[2]->Draw(worldTransformR_parts_, viewProjection);
}
