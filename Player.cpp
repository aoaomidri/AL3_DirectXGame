#include"Player.h"
#include<assert.h>
#include"ImGuiManager.h"

void Player::Initialaize(Model* model, uint32_t textureHandle) { 
	//ぬるぽチェック
	assert(model);
	//メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();


	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

}


void Player::Update() {

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	} else {
		move.x = 0.0f;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else {
		move.y = 0.0f;
	}

	if (move.x>1.0f) {
		move.x = 1.0f;
	} else if (move.x < -1.0f) {
		move.x = -1.0f;
	}

	if (move.y>1.0f) {
		move.y = 1.0f;
	} else if (move.y < -1.0f) {
		move.y = -1.0f;
	}

	// 移動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

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

	worldTransform_.matWorld_ = matrix.MakaAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//行列をバッファに転送
	worldTransform_.TransferMatrix();


	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Position");
	ImGui::Text(
	    "Player_Position %0.1f,%0.1f,%0.1f", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();
}


void Player::Draw(ViewProjection viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
}

