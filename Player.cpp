#include "Player.h"
#include<assert.h>
void Player::Initialaize(Model* model, uint32_t textureHandle) {
	// ぬるぽチェック
	assert(model);
	// メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {


	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.1f;


	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	} else {
		move.x = 0.0f;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	} else {
		move.y = 0.0f;
	}

	if (move.x > 0.5f) {
		move.x = 0.5f;
	} else if (move.x < -0.5f) {
		move.x = -0.5f;
	}

	if (move.y > 0.5f) {
		move.y = 0.5f;
	} else if (move.y < -0.5f) {
		move.y = -0.5f;
	}

	// 移動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	// 座標を加算
	worldTransform_.AddTransform(move);

	if (worldTransform_.translation_.x <= -kMoveLimitX) {
		move.x = 0.0f;
	} else if (worldTransform_.translation_.x >= kMoveLimitX) {
		move.x = 0.0f;
	}

	if (worldTransform_.translation_.y <= -kMoveLimitY) {
		move.y = 0.0f;
	} else if (worldTransform_.translation_.y >= kMoveLimitY) {
		move.y = 0.0f;
	}

	worldTransform_.UpdateMatrix(scale);
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}