#pragma once
#include"BaseCharacter.h"
#include"Input.h"
#include"MyMatrix.h"
#include"MyVector.h"
#include<optional>
#include"Adjustment_Item.h"

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

	//通常行動初期化
	void BehaviorRootInitialize();
	//攻撃行動初期化
	void BehaviorAttackInitialize();
	// 通常行動更新
	void BehaviorRootUpdate();
	//攻撃行動更新
	void BehaviorAttackUpdate();

	//WorldTransform& GetWorldTransform() { return worldTransform_; };


	void SetViewProjection(const ViewProjection* viewprojection) {
		viewProjection_ = viewprojection;
	}
	//調整項目の適用
	void ApplyGlobalVariables();

private:

	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;

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
	//向きをそろえる
	Matrix4x4 minusMatrix{0};

	Vector3 minusVelocity{0, 0, 0};

	Vector3 velocityZ{0, 0, 0};
	//コントローラーの入力
	XINPUT_STATE joyState;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動のサイクル
	int floatingCycle_ = 0;
	// 浮遊の振幅(ｍ)
	float floatingAmplitude = 0;

	// 腕の媒介変数
	float armParameter_ = 0.0f;
	// 腕移動のサイクル
	int armPeriod = 0;
	//腕のふり幅
	float armAmplitude = 0;

	// 地面からの距離
	float disGround = 0;

	//キャラクターのスピード調整変数
	float kCharacterSpeedBase = 0.5f;
	// キャラクターの移動速さ
	float kCharacterSpeed = 0.0f;

	Vector3 Head_offset;
	Vector3 L_arm_offset;
	Vector3 R_arm_offset;
	Vector3 Weapon_offset;

	Vector3 Head_offset_Base = {0.0f, 4.5f, 0.0f};
	Vector3 L_arm_offset_Base = {1.4f, 3.5f, 0.0f};
	Vector3 R_arm_offset_Base = {-1.4f, 3.5f, 0.0f};
	Vector3 Weapon_offset_Base = {0.0f, 3.5f, 0.0f};


	//武器の回転
	float weapon_Rotate = 0.0f;
	float arm_Rotate = -3.15f;
	//武器開店に関連する変数
	const float moveWeapon = 0.1f;
	const float moveWeaponShakeDown = 0.2f;
	const float MaxRotate = 1.55f;
	const float MinRotate = -0.6f;

	int WaitTimeBase = 20;
	int WaitTime = 0;

	bool isShakeDown = false;

	//振る舞い
	enum class Behavior {
		kRoot,//通常状態
		kAttack,//攻撃中
	};
	
	Behavior behavior_ = Behavior::kRoot;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};
