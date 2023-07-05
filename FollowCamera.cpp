#include "FollowCamera.h"
FollowCamera::FollowCamera() {

}

void FollowCamera::Initialize() { 
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void FollowCamera::Update() {
	////カメラ操作左右
	//if (input_->PushKey(DIK_RIGHT)) {
	//	viewProjection_.rotation_.y += 0.03f;
	//} else if (input_->PushKey(DIK_LEFT)) {
	//	viewProjection_.rotation_.y -= 0.03f;
	//}
	////カメラ操作上下
	//if (input_->PushKey(DIK_UP)) {
	//	viewProjection_.rotation_.x += 0.03f;
	//} else if (input_->PushKey(DIK_DOWN)) {
	//	viewProjection_.rotation_.x -= 0.03f;
	//}

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
