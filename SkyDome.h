#pragma once
#include"Model.h"
#include"WorldTransform.h"
class SkyDome {

public:
	SkyDome();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	float size = 100.0f;

	Vector3 scale = {size, size, size};
};
