#include "Enemy.h"
#include<assert.h>
#include"ImGuiManager.h"
#include"Player.h"

Enemy::~Enemy() { 

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

}

void Enemy::Initialaize(Model* model, uint32_t textureHandle) {
	// ぬるぽチェック
	assert(model);
	// メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	
	worldTransform_.translation_ = {10, 10, 30};

	worldTransform_.Initialize();
	
	ApproathReset();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Enemy::ApproathReset() {
	//発射タイマー初期化
	fireTimer = 10;
}

// 更新
void Enemy::Update() {

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		//移動
		move.z = 0.0f;
		worldTransform_.AddTransform(move);
		if (worldTransform_.translation_.z<0.0f) {
			phase_ = Phase::Leave;
		}
		//発射タイマーカウントダウン
		//fireTimer -= 1;
		//指定時間に達した
		if (fireTimer == 0){
			// 弾を発射
			Fire();
			//タイマー初期化
			fireTimer = kFireInterval;
		}
		
		break;
	case Enemy::Phase::Leave:
		//
		move.z = 0.4f;
		worldTransform_.AddTransform(move);

		break;
	
	}




	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	worldTransform_.UpdateMatrix(scale);


	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

}

// 描画
void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Attack() { 
	if (input_->TriggerKey(DIK_RSHIFT)) {

		// 弾の速度
		const float kBulletSpeed = -1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		//
		bullets_.push_back(newBullet);
	}

}

void Enemy::Fire() { 
	assert(player_);

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = {0, 0, 0};
		Vector3 playerPos = player_->GetWorldPosition();
	    Vector3 enemyPos = GetWorldPosition();
	    Vector3 vector = {
	        playerPos.x - enemyPos.x,
	        playerPos.y - enemyPos.y,
	        playerPos.z - enemyPos.z
	    };
	    float bulletNorm =
	        sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

		if (bulletNorm != 0.0f) {

			velocity = {
		    (vector.x / bulletNorm) * kBulletSpeed, 
			(vector.y / bulletNorm) * kBulletSpeed,
		    (vector.z / bulletNorm) * kBulletSpeed};
	    }

		

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	    newBullet->SetPlayer(player_);
		//
		bullets_.push_back(newBullet);
}

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos(0, 0, 0);

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision() {



}