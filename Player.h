#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"Matrix.h"
#include"PlayerBullet.h"
#include<list>

///<summary>
/// 自キャラ
///</summary>

class Player {
public:

	~Player();

	//初期化
	void Initialaize(Model* model, uint32_t textureHandle);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection viewProjection);

	//旋回処理
	void Rotate();

	///< summary>
	/// 攻撃
	///</summary>
	void Attack();

	Vector3 GetWorldPosition();
	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	// 衝突したら呼び出す関数
	void OnCollision();

	float radius = 1.5f;

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};
	//行列の作成
	Matrix matrix;
	//弾
	std::list<PlayerBullet*> bullets_;

	float size = 1.0f;

	Vector3 scale = {size, size, size};

	
};



