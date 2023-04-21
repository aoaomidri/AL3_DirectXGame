#include"Player.h"
#include<assert.h>
#include"ImGuiManager.h"

Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialaize(Model* model, uint32_t textureHandle) { 
	//ぬるぽチェック
	assert(model);
	//メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();


	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

}

void Player::Rotate() {
	const float kRowSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRowSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRowSpeed;
	}
}

void Player::Update() {

	//デスフラグの立った球を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	Rotate();

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	} else {
		move.x = 0.0f;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else {
		move.y = 0.0f;
	}

	if (move.x>1.0f) {
		move.x = 1.0f;
	} else if (move.x < -1.0f) {
		move.x = -1.0f;
	}

	if (move.y>1.0f) {
		move.y = 1.0f;
	} else if (move.y < -1.0f) {
		move.y = -1.0f;
	}

	// 移動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
		
	//座標を加算
	worldTransform_.AddTransform(move);

	if (worldTransform_.translation_.x <= -kMoveLimitX) {
		move.x = 0.0f;
	} else if (worldTransform_.translation_.x >= kMoveLimitX) {
		move.x = 0.0f;
	}

	if (worldTransform_.translation_.y <= -kMoveLimitY) {
		move.y = 0.0f;
	} else if (worldTransform_.translation_.y >= kMoveLimitY) {
		move.y = 0.0f;
	}

	

	worldTransform_.UpdateMatrix(scale);

	// キャラの攻撃処理
	Attack();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::Text(
	    "Player_Position %0.1f,%0.1f,%0.1f\n", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::Text(
	    "Player_Rotate %0.1f,%0.1f,%0.1f", worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,worldTransform_.rotation_.z);
	ImGui::End();
}


void Player::Draw(ViewProjection viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		//ベクトルの向きを自機の向きと合わせる
		velocity = matrix.TransformNormal(velocity, worldTransform_.matWorld_);
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		//弾を登録する
		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() {

	Vector3 worldPos(0, 0, 0);

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;

}

void Player::OnCollision() {


}