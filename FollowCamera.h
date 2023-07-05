#pragma once
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"MyMatrix.h"
#include"MyVector.h"
#include"Input.h"
class FollowCamera {
public:
	FollowCamera();

	void Initialize();

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target;}

	ViewProjection &GetViewProjection() { return viewProjection_;}

private:
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//追従対象
	const WorldTransform* target_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;
	//コントローラー入力
	XINPUT_STATE joyState;

	// 行列の作成
	MyMatrix matrix_;

	//Vectorの作成
	MyVector vector_;

};
