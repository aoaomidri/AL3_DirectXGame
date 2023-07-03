#include "Ground.h"
#include <assert.h>
Ground::Ground() { worldTransform_.translation_ = {0, 0, 0}; }

void Ground::Initialize(Model* model, uint32_t textureHandle) {
	// ぬるぽチェック
	assert(model);
	// メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void Ground::Update() { 
	worldTransform_.UpdateMatrix(scale);
}

void Ground::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}