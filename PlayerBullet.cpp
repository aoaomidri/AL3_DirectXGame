#include "PlayerBullet.h"
#include <assert.h>
PlayerBullet::~PlayerBullet() {
	for (PlayerEffect* effect : effect_) {
		delete effect;
	}
}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) { 
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;

}

void PlayerBullet::Update() { 
	// デスフラグの立ったエフェクトを削除
	effect_.remove_if([](PlayerEffect* effect) {
		if (effect->IsDead()) {
			delete effect;
			return true;
		}
		return false;
	});

	if (--deathTimer_<=0) {
		isDead_ = true;
	}

	

	numberA += rotateAngle;
	if (numberA >= 360) {
		numberA = 0;
	}

	numberB += rotateAngle;
	if (numberB >= 360) {
		numberB = 0;
	}

	numberC += rotateAngle;
	if (numberC >= 360) {
		numberC = 0;
	}

	numberD += rotateAngle;
	if (numberD >= 360) {
		numberD = 0;
	}

	numberE += rotateAngle;
	if (numberE >= 360) {
		numberE = 0;
	}

	numberF += rotateAngle;
	if (numberF >= 360) {
		numberF = 0;
	}

	/*AttackEffect(numberA);
	AttackEffect(numberB);
	AttackEffect(numberC);
	AttackEffect(numberD);
	AttackEffect(numberE);
	AttackEffect(numberF);*/

	worldTransform_.AddTransform(velocity_);

	worldTransform_.UpdateMatrix(scale);

	for (PlayerEffect* effect : effect_) {
		effect->Update();
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (PlayerEffect* effect : effect_) {
		effect->Draw(viewProjection);
	}

}

void PlayerBullet::OnCollision() { 
	isDead_ = true;

}

Vector3 PlayerBullet::GetWorldPosition() {

	Vector3 worldPos(0, 0, 0);

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::AttackEffect(int number) {

	Vector3 effectPos = {100, 100, -100};

	// 自弾リストの取得
	effectPos = worldTransform_.translation_;

	PlayerEffect* newEffect = new PlayerEffect();
	newEffect->Initialize(model_, effectPos);
	//
	effect_.push_back(newEffect);

	newEffect->AttackEffect(number);
}