#pragma once
#include"BaseCharacter.h"
#include"Input.h"
#include"MyMatrix.h"
#include"MyVector.h"

class Player : public BaseCharacter {
public:

	// 初期化
	void Initialize(const std::vector<Model*>& models) override;

	// 更新
	void Update() override;

	// 描画
	void Draw(const ViewProjection& viewProjection) override;

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();

	//浮遊ギミックの初期化更新
	void UpdateFloatingGimmick();

	// 腕ぶらぶらギミックの初期化
	void initializeMoveArm();

	//腕ぶらぶらギミックの更新
	void UpdateMoveArm();

	WorldTransform& GetWorldTransform() { return worldTransform_; };

	void SetViewProjection(const ViewProjection* viewprojection) {
		viewProjection_ = viewprojection;
	}

private:
	// ワールド変換データ
	/*カメラに渡す用*/
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;
	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};
	// 行列の作成
	MyMatrix matrix;
	// ベクトルの計算
	MyVector vector;
	//モデルの大きさ
	float size = 1.0f;

	Vector3 scale = {size, size, size};

	Matrix4x4 minusMatrix{0};

	Vector3 minusVelocity{0, 0, 0};

	Vector3 velocityZ{0, 0, 0};
	//コントローラーの入力
	XINPUT_STATE joyState;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動のサイクル
	int period = 0;
	// 浮遊の振幅(ｍ)
	float amplitude = 0;

	// 腕の媒介変数
	float armParameter_ = 0.0f;
	// 腕移動のサイクル
	int armPeriod = 0;
	//腕のふり幅
	float armAmplitude = 0;

	// 地面からの距離
	float disGround = 0;

	// キャラクターの移動速さ
	float kCharacterSpeed = 0.5f;
	
};
