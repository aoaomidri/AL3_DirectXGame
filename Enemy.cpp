#include "Enemy.h"
void Enemy::Initialize(const std::vector<Model*>& models) { 
	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = {20.0f, 0.0f, 20.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

}

void Enemy::Update() { 
	worldTransform_.UpdateMatrix(scale);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	models_[0]->Draw(worldTransform_, viewProjection);

}
