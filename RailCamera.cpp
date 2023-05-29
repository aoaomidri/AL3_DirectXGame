#include "RailCamera.h"
#include<imgui.h>
RailCamera::RailCamera() { 
	move = {0.0f, 0.0f, 0.01f};
}

RailCamera::~RailCamera() { 
	delete matrix;

}

void RailCamera::Initialize(Vector3& worldPosition, Vector3& rotate) { 
	worldTransform_.translation_ = worldPosition;
	worldTransform_.rotation_ = rotate;

	viewProjection_.farZ = 300.0f;
	viewProjection_.Initialize();
	
}

void RailCamera::Update() { 
	//worldTransform_.AddTransform(move);
	worldTransform_.rotation_.y += 0.002f;
	worldTransform_.CalculationMatrix(
	    scale, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.matView = matrix->Inverce(worldTransform_.matWorld_);
	
	
	ImGui::Begin("Camera");
	ImGui::DragFloat3("CameraTransform", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("CameraRotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat("MatWorld", &worldTransform_.matWorld_.m[3][2], 0.01f);
	ImGui::DragFloat("MatView", &viewProjection_.matView.m[3][2], 0.01f);
	ImGui::End();
}
