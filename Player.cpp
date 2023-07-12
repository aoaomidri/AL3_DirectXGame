#include "Player.h"
#include<assert.h>
#include<imgui.h>
void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	worldTransformBody_.Initialize();
	worldTransformBody_.translation_ = {0.0f, 0.0f, 0.0f};

	worldTransformHead_.Initialize();

	worldTransformL_arm_.Initialize();

	worldTransformR_arm_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	InitializeFloatingGimmick();
	initializeMoveArm();
}

void Player::Update() {
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		kCharacterSpeed = 1.0f;
	} else {
		kCharacterSpeed = 0.5f;
	}


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
		worldTransformBody_.rotation_.y = std::atan2(move.x, move.z);
		worldTransformHead_.rotation_.y = std::atan2(move.x, move.z);

		worldTransformL_arm_.rotation_.y = std::atan2(move.x, move.z);
		worldTransformR_arm_.rotation_.y = std::atan2(move.x, move.z);
	}
	
	UpdateFloatingGimmick();
	UpdateMoveArm();

	// 座標を加算
	worldTransform_.AddTransform(move);
	worldTransformBody_.AddTransform(move);

	Matrix4x4 PlayerRotateMatrix = matrix.MakeRotateMatrix(worldTransformBody_.rotation_);
	Vector3 Head_offset = {0.0f, 4.5f, 0.0f};
	Vector3 L_arm_offset = {1.4f, 3.5f, 0.0f};
	Vector3 R_arm_offset = {-1.4f, 3.5f, 0.0f};

	Head_offset = vector.TransformNormal(Head_offset, PlayerRotateMatrix);
	L_arm_offset = vector.TransformNormal(L_arm_offset, PlayerRotateMatrix);
	R_arm_offset = vector.TransformNormal(R_arm_offset, PlayerRotateMatrix);
	// 座標をコピーしてオフセット分ずらす
	worldTransformHead_.translation_ = worldTransformBody_.translation_ + Head_offset;
	worldTransformL_arm_.translation_ = worldTransformBody_.translation_ + L_arm_offset;
	worldTransformR_arm_.translation_ = worldTransformBody_.translation_ + R_arm_offset;

	//座標を転送
	worldTransformBody_.UpdateMatrix(scale);
	worldTransformHead_.UpdateMatrix(scale);
	worldTransformL_arm_.UpdateMatrix(scale);
	worldTransformR_arm_.UpdateMatrix(scale);
}

void Player::Draw(const ViewProjection& viewProjection) {
	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformHead_, viewProjection);
	models_[2]->Draw(worldTransformL_arm_, viewProjection);
	models_[3]->Draw(worldTransformR_arm_, viewProjection);
}

void Player::InitializeFloatingGimmick() { 
	floatingParameter_ = 0.0f;
	period = 90;
	amplitude = 0.7f;
	disGround = 0.3f;
}

void Player::UpdateFloatingGimmick() {
	
	//1フレームでのパラメータ加算値
	const float step = 2.0f * static_cast<float>(M_PI) / period;

	//パラメータを1ステップ分加算
	floatingParameter_ += step;

	//2πを超えたら0に戻す
	floatingParameter_ =
	    static_cast<float>(std::fmod(floatingParameter_, 2.0 * static_cast<float>(M_PI)));	

	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude + disGround;
	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Translation", &worldTransformHead_.translation_.x, 4.0f, 10.0f);
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, 3.0f, 10.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, 3.0f, 10.0f);
	ImGui::SliderInt("Period", &period, 10, 180);
	ImGui::SliderFloat("Amplitude", &amplitude, 0.1f, 1.0f);
	ImGui::SliderFloat("DisGround", &disGround, 0.1f, 1.0f);
	ImGui::End();
}

void Player::initializeMoveArm() {
	armParameter_ = 0.0f;
	armPeriod = 75;
	armAmplitude = 0.5f;

}

void Player::UpdateMoveArm() {
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * static_cast<float>(M_PI) / armPeriod;

	// パラメータを1ステップ分加算
	armParameter_ += step;

	// 2πを超えたら0に戻す
	armParameter_ =
	    static_cast<float>(std::fmod(armParameter_, 2.0 * static_cast<float>(M_PI)));	

	worldTransformL_arm_.rotation_.x = std::sin(armParameter_) * armAmplitude;
	worldTransformR_arm_.rotation_.x = std::sin(armParameter_) * armAmplitude;
	ImGui::Begin("PlayerRotate");
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.rotation_.x, -90.0f, 90.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.rotation_.x, -90.0f, 90.0f);
	ImGui::End();

}
