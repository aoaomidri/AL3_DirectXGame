#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"Matrix.h"
#include"PlayerBullet.h"
#include <Sprite.h>

///<summary>
/// 自キャラ
///</summary>

class Player {
public:

	~Player();

	//初期化
	void Initialaize(Model* model, uint32_t textureHandle);

	//更新
	void Update(const Matrix4x4& matView, const Matrix4x4& matProjection);

	//描画
	void Draw(ViewProjection viewProjection);

	//UI描画
	void DrawUI();

	//旋回処理
	void Rotate();

	void MouseReticle(const Matrix4x4& matView, const Matrix4x4& matProjection);

	void ShotReticle(const Matrix4x4& matView, const Matrix4x4& matProjection);

	///< summary>
	/// 攻撃
	///</summary>
	void Attack();

	Vector3 GetPlayerWorldPosition();

	Vector3 GetWorldPosition(Matrix4x4 mat);
	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	// エフェクトリストを取得
	const std::list<PlayerEffect*>& GetEffects() { return effect_; }

	// 衝突したら呼び出す関数
	void OnCollision();

	float radius = 1.5f;

	static const int kChargeInterval = 1;

	void Charge();

	// 弾のエフェクト
	void AttackEffect(int number);

	// 弾のエフェクト
	void MoveEffect();

	void SetParent(const WorldTransform* parent);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	//モデル
	Model* model_ = nullptr;
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};
	//行列の作成
	Matrix matrix;
	//ベクトルの計算
	Vector3 vector;
	//プレイヤーのワールド座標
	Vector3 worldPlayerPos;
	
	//弾
	std::list<PlayerBullet*> bullets_;

	std::list<PlayerEffect*> effect_;

	float size = 1.0f;

	Vector3 scale = {size, size, size};

	Vector3 reticleScale_ = {size / 2.0f, size / 2.0f, size / 2.0f};

	// 発射の時間
	int bulletTime = 0;
	// 自機の弾の発射間隔
	int bulletInterval = 6;

	int Life = 0;	

	// 発射タイマー
	int32_t ChargeTimer = 0;

	int effectType = 1;

	int numberA = 0;

	int numberB = 120;

	int numberC = 240;

	int numberD = 270;

	Vector3 effectPos = {100, 100, -100};
	

};



