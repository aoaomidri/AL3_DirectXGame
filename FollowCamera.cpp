#include "FollowCamera.h"
#include<imgui.h>

void FollowCamera::Initialize() { 
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	destinationAngleX_= 0.2f;

	distance = -30.0f;

	vector_ = std::make_unique<MyVector>();
	matrix_ = std::make_unique<MyMatrix>();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void FollowCamera::SetTarget(const WorldTransform* target) { 
	target_ = target;
	Reset();

}

void FollowCamera::Update() {

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float rotateSpeed = 0.05f;

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			if (distance <= -10.0f && distance >= -100.0f){
				distance += (float)joyState.Gamepad.sThumbRY / SHRT_MAX;
			} else if (distance < -100.0f) {
				distance = -100.0f;
			} else if (distance > -10.0f) {
				distance = -10.0f;
			}
			
			
		} else {
			destinationAngleY_ += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;

			destinationAngleX_ += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * rotateSpeed;
		}


		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			destinationAngleY_ = target_->rotation_.y;
			destinationAngleX_ = 0.2f;
		}	

	}
	
	viewProjection_.rotation_.y =
	    vector_->LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.1f);
	viewProjection_.rotation_.x =
	    vector_->LerpShortAngle(viewProjection_.rotation_.x, destinationAngleX_, 0.1f);

	ImGui::Begin("Camera");
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x, 0.01f);
	ImGui::End();

	if (target_) {
		//追従座標の補完
		interTarget_ = vector_->Lerp(interTarget_, target_->translation_, t);
		//追従対象からカメラまでのオフセット
		Vector3 offset = offsetCalculation();
		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = interTarget_ + offset;

	}
	viewingFrustum_ = {
	    .translation_ = viewProjection_.translation_,
	    .rotate_ = viewProjection_.rotation_,
	    .direction = {0.0f, 0.0f, 1.0f},
	    .verticalFOV = viewProjection_.fovAngleY,
	    .aspectRatio = viewProjection_.aspectRatio,
	    .nearZ = viewProjection_.nearZ,
	    .farZ = viewProjection_.farZ,
	};
	//ビュー行列の更新
	viewProjection_.UpdateMatrix();
}

Vector3 FollowCamera::offsetCalculation() const {
	Vector3 offset = {0.0f, 5.0f, distance};
	Matrix4x4 newRotateMatrix = matrix_->MakeRotateMatrix(viewProjection_.rotation_);

	offset = vector_->TransformNormal(offset, newRotateMatrix);

	return offset;
}

void FollowCamera::Reset() {
	if (target_) {
		//追従座標・角度の初期化
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}
	Vector3 offset = offsetCalculation();
	viewProjection_.translation_ = interTarget_ + offset;
}