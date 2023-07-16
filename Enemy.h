#pragma once
#include"BaseCharacter.h"
#include "MyMatrix.h"
#include "MyVector.h"

class Enemy : public BaseCharacter{
public:
	// 初期化
	void Initialize(const std::vector<Model*>& models) override;

	// 更新
	void Update() override;

	// 描画
	void Draw(const ViewProjection& viewProjection) override;

private:
	//エネミーのパーツの座標
	WorldTransform worldTransformL_parts_;
	WorldTransform worldTransformR_parts_;
	
	// 行列の作成
	MyMatrix matrix;
	// ベクトルの計算
	MyVector vector;
	// モデルの大きさ
	float size = 2.0f;

	Vector3 scale = {size, size, size};
	// キャラクターの移動速さ
	float kCharacterSpeed = 0.5f;

	Matrix4x4 minusMatrix{0};

	Vector3 minusVelocity{0, 0, 0};

	Vector3 velocityZ{0, 0, 0};
};
