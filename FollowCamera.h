#pragma once
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"MyMatrix.h"
#include"MyVector.h"
#include"Input.h"
class FollowCamera {

private:
	struct ViewingFrustum {
		Vector3 translation_; // カメラの座標
		Vector3 rotate_;      // カメラの回転
		Vector3 direction;    // カメラの向き
		float verticalFOV;    // 縦画角
		float aspectRatio;    // アスペクト比
		float nearZ;          // 深度限界（手前側）
		float farZ;           // 深度限界（奥側）
	};

	ViewingFrustum viewingFrustum_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//追従対象
	const WorldTransform* target_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

	//コントローラー入力
	XINPUT_STATE joyState;

	//追従対象の残像座標
	Vector3 interTarget_ = {};

	float distance;

	//補完レート
	const float t = 0.1f;

	//目標角度
	float destinationAngleY_ = 0.0f;
	float destinationAngleX_ = 0.0f;
	// 行列の作成
	std::unique_ptr<MyMatrix> matrix_;

	//Vectorの作成
	std::unique_ptr<MyVector> vector_;

public:
	void Initialize();

	void Update();

	void Reset();

	Vector3 offsetCalculation() const;

	void SetTarget(const WorldTransform* target);

	ViewProjection& GetViewProjection() { return viewProjection_; }

	viewingFrustum& GetViewingFrustum() { return viewingFrustum_; }
};
