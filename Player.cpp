#include "Player.h"
#include<assert.h>
void Player::Initialaize(Model* model, uint32_t textureHandle) {
	// ぬるぽチェック
	assert(model);
	// メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 3.5f, 0.0f};

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {


	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.5f;


	//// 押した方向で移動ベクトルを変更(左右)
	//if (input_->PushKey(DIK_A)) {
	//	move.x -= kCharacterSpeed;
	//} else if (input_->PushKey(DIK_D)) {
	//	move.x += kCharacterSpeed;
	//} else {
	//	move.x = 0.0f;
	//}

	//// 押した方向で移動ベクトルを変更(上下)
	//if (input_->PushKey(DIK_W)) {
	//	move.z += kCharacterSpeed;
	//} else if (input_->PushKey(DIK_S)) {
	//	move.z -= kCharacterSpeed;
	//} else {
	//	move.z = 0.0f;
	//}

	//if (move.x > 0.5f) {
	//	move.x = 0.5f;
	//} else if (move.x < -0.5f) {
	//	move.x = -0.5f;
	//}

	//if (move.z > 0.5f) {
	//	move.z = 0.5f;
	//} else if (move.z < -0.5f) {
	//	move.z = -0.5f;
	//}

	if (Input::GetInstance()->GetJoystickState(0,joyState)) {
		//移動量
		move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed,
		    0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed,

		};
		move = vector.Multiply(kCharacterSpeed, vector.Normalize(move));
	}
	Matrix4x4 newRotateMatrix = matrix.MakeRotateMatrixY(viewProjection_->rotation_);

	move = vector.TransformNormal(move,newRotateMatrix);

	//プレイヤーの移動方向に見た目を合わせる
	if (move.x != 0.0f || move.z != 0.0f) {
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	// 座標を加算
	worldTransform_.AddTransform(move);

	worldTransform_.UpdateMatrix(scale);
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

const WorldTransform& Player::GetWorldTransform() { 

	return worldTransform_;

}
