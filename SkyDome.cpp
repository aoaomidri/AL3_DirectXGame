#include "SkyDome.h"
#include<assert.h>
SkyDome::SkyDome() { 
	worldTransform_.translation_ = {0, 0, 0};
}

void SkyDome::Initialize(Model* model, uint32_t textureHandle) {
	// ぬるぽチェック
	assert(model);
	// メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void SkyDome::Update() { worldTransform_.UpdateMatrix(scale); }

void SkyDome::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
