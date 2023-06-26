#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"MyMatrix.h"
#include"MyVector.h"
#include <Sprite.h>

class Player {
public:

	// 初期化
	void Initialaize(Model* model, uint32_t textureHandle);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};
	// 行列の作成
	MyMatrix matrix;
	// ベクトルの計算
	MyVector vector;

	float size = 1.0f;

	Vector3 scale = {size, size, size};
};
