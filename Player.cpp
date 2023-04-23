﻿#include"Player.h"
#include<assert.h>
#include"ImGuiManager.h"


Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	for (PlayerEffect* effect : effect_){
		delete effect;
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
	const float kRowSpeed = 0.1f;

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

	// デスフラグの立ったエフェクトを削除
	effect_.remove_if([](PlayerEffect* effect) {
		if (effect->IsDead()) {
			delete effect;
			return true;
		}
		return false;
	});

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	Rotate();

	numberA += 10;
	if (numberA >= 360) {
		numberA = 0;
	}

	numberB += 10;
	if (numberB >= 360) {
		numberB = 0;
	}

	numberC += 10;
	if (numberC >= 360) {
		numberC = 0;
	}

	numberD += 10;
	if (numberD >= 360) {
		numberD = 0;
	}
	


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
	if (bullets_.remove_if([](PlayerBullet* bullet) {
		    if (bullet->IsDead()) {
			    return true;
		    }
		    return false;}) == false){
		AttackEffect(numberA);
		AttackEffect(numberB);
		AttackEffect(numberC);
		AttackEffect(numberD);

	}
	
		

	// 発射タイマーカウントダウン
	if (input_->PushKey(DIK_RSHIFT)) {
		// 弾を発射
		Charge();
		Charge();
	}
	
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();				
	}
	for (PlayerEffect* effect:effect_) {
		effect->Update();
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
	for (PlayerEffect* effect : effect_) {
		effect->Draw(viewProjection);
	}
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 0.8f;
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

void Player::Charge() {

	// 弾の速度
	int numberX = rand();
	int numberY = rand();
	int numberZ = rand();

	Vector3 playerPos = GetWorldPosition();
	Vector3 effectPos = { 
		numberX % 20 + (playerPos.x - 10), 
		numberY % 20 + (playerPos.y - 10), 
		numberZ % 20 + (playerPos.z - 10)
	};

	
	PlayerEffect* newEffect = new PlayerEffect();
	newEffect->Initialize(model_, effectPos);
	//
	effect_.push_back(newEffect);
	
	newEffect->Charge(playerPos);
}

void Player::AttackEffect(int number) {
	Vector3 effectPos = {0, 0, -100};	

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = GetBullets();
	for (PlayerBullet* bullet : playerBullets) {
			effectPos = bullet->GetWorldPosition();
	}	

	PlayerEffect* newEffect = new PlayerEffect();
	newEffect->Initialize(model_, effectPos);
	//
	effect_.push_back(newEffect);


	 newEffect->AttackEffect(number);
	
}