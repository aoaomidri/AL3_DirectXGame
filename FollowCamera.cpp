#include "FollowCamera.h"

void FollowCamera::Initialize() { 
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	viewProjection_.rotation_.x = 0.2f;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void FollowCamera::Update() {

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float rotateSpeed = 0.05f;

		viewProjection_.rotation_.y -= (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;

		viewProjection_.rotation_.x += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * rotateSpeed;
	}



	if (target_) {
		//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -30.0f};
		Matrix4x4 newRotateMatrix = matrix_.MakeRotateMatrix(viewProjection_.rotation_);

		offset = vector_.TransformNormal(offset, newRotateMatrix);

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;

	}
	//ビュー行列の更新
	viewProjection_.UpdateMatrix();



}
