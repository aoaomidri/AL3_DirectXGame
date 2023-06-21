#include"Player.h"
#include<assert.h>
#include"ImGuiManager.h"



Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	for (PlayerEffect* effect : effect_){
		delete effect;
	}

	delete sprite2DReticle_;
}

void Player::Initialaize(Model* model, uint32_t textureHandle) { 
	//ぬるぽチェック
	assert(model);
	//メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	uint32_t textureReticle = TextureManager::Load("Magic.png");
	

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 50.0f};

	sprite2DReticle_ = Sprite::Create(textureReticle, {640.0f, 320.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});

	

	worldTransform3DReticle_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

}

void Player::Rotate() {
	const float kRowSpeed = 0.1f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y -= kRowSpeed;
	} else if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y += kRowSpeed;
	}
}

void Player::Update(const Matrix4x4& matView, const Matrix4x4& matProjection) {

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
	const float kCharacterSpeed = 0.1f;

	Rotate();

	worldPlayerPos = GetPlayerWorldPosition();
	

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	} else {
		move.x = 0.0f;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	} else {
		move.y = 0.0f;
	}

	

	if (move.x>0.5f) {
		move.x = 0.5f;
	} else if (move.x < -0.5f) {
		move.x = -0.5f;
	}

	if (move.y>0.5f) {
		move.y = 0.5f;
	} else if (move.y < -0.5f) {
		move.y = -0.5f;
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
	
	MouseReticle(matView, matProjection);
	//ShotReticle(matView, matProjection);

	// キャラの攻撃処理
	Attack();
	//マウスレティクルの設定
	

	/*if (bullets_.remove_if([](PlayerBullet* bullet) {
		    if (bullet->IsDead()) {
			    return true;
		    }
		    return false;}) == false){

		if (effectType > 1) {
			AttackEffect(numberA);
			AttackEffect(numberB);
		}
		if (effectType>2) {
			AttackEffect(numberC);
		}
		if (effectType>3) {
			AttackEffect(numberD);
		}
	}*/
	
		

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
	    "2DReticle:(%0.1f,%0.1f)", sprite2DReticle_->GetPosition().x,
	    sprite2DReticle_->GetPosition().y);
	ImGui::Text(
	    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);

	ImGui::Text("PlayerLife : %d", Life);
	ImGui::End();

	
}


void Player::Draw(ViewProjection viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);
	
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	for (PlayerEffect* effect : effect_) {
		effect->Draw(viewProjection);
	}
}

void Player::DrawUI() { 
	sprite2DReticle_->Draw();
}

void Player::Attack() { 
	if (input_->IsPressMouse(0)||input_->PushKey(DIK_SPACE)) {		
		bulletTime += 1;
		if (bulletTime % bulletInterval == 1) {

			// 弾の速度
			const float kBulletSpeed = 1.5f;
			Vector3 world3DReticlePos = GetWorldPosition(worldTransform3DReticle_.matWorld_);

			Vector3 velocity = world3DReticlePos - worldPlayerPos;
			velocity = matrix.NormalizePlus(velocity, kBulletSpeed);
			// ベクトルの向きを自機の向きと合わせる
			//velocity = matrix.TransformNormal(velocity, worldTransform_.matWorld_);
			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, worldPlayerPos, velocity);
			// 弾を登録する
			bullets_.push_back(newBullet);
		}
	} else {
		bulletTime = 0;
	}

}

void Player::MouseReticle(const Matrix4x4& matView, const Matrix4x4& matProjection) {
	// 2Dから3Dへのレティクルの変換
	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));

	// ビューポート行列
	Matrix4x4 matViewport =
	    matrix.MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    matrix.Multiply(matrix.Multiply(matView, matProjection), matViewport);

	Matrix4x4 matInverseVPV = matrix.Inverce(matViewProjectionViewport);

	// スクリーン座標
	Vector3 posNear = Vector3((float)mousePosition.x, (float)mousePosition.y, 0);
	Vector3 posFar = Vector3((float)mousePosition.x, (float)mousePosition.y, 1);

	// スクリーンからワールド座標系へ変換
	posNear = matrix.Transform(posNear, matInverseVPV);
	posFar = matrix.Transform(posFar, matInverseVPV);

	// マウスレイの方向

	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = matrix.Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translation_ =
	    posNear + vector.Mutiply(mouseDirection, kDistanceTestObject);
	worldTransform3DReticle_.UpdateMatrix(reticleScale_);

	
}

void Player::ShotReticle(const Matrix4x4& matView, const Matrix4x4& matProjection) {
	 //3Dから2Dへのレティクルの変換↓

	//自機から3Dレティクルへの距離
	 const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルへのオフセット
	 Vector3 offset = {0.0f, 0.0f, 1.0f};
	//自機のワールド行列の回転を反映
	 offset = matrix.TransformNormal(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	 offset = matrix.NormalizePlus(offset, kDistancePlayerTo3DReticle);
	//3Dレティクルの座標を設定
	 worldTransform3DReticle_.translation_ = worldPlayerPos + offset;
	 worldTransform3DReticle_.UpdateMatrix(reticleScale_);

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		Vector3 positionReticle = GetWorldPosition(worldTransform3DReticle_.matWorld_);
		//ビューポート行列
		Matrix4x4 matViewport =
		    matrix.MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		//ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport =
		    matrix.Multiply(matrix.Multiply(matView, matProjection), matViewport);
	
		//ワールド->スクリーン座標変換(ここで3Dから2Dになる)
		positionReticle = matrix.Transform(positionReticle, matViewProjectionViewport);

		//スプライトのレティクルに座標設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	}
	//ここまで↑
}

Vector3 Player::GetPlayerWorldPosition() {

	Vector3 worldPos(0, 0, 0);

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetWorldPosition(Matrix4x4 mat) {

	Vector3 worldPos(0, 0, 0);

	worldPos.x = mat.m[3][0];
	worldPos.y = mat.m[3][1];
	worldPos.z = mat.m[3][2];

	return worldPos;

}

void Player::OnCollision() { 
	Life -= 1;
}

void Player::Charge() {

	// 弾の速度
	int numberX = rand();
	int numberY = rand();
	int numberZ = rand();

	Vector3 playerPos = GetWorldPosition(worldTransform_.matWorld_);
	effectPos = { 
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

	Vector3 effectPos = {100, 100, -100};
	
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

void Player::MoveEffect() {
	 effectPos = {0, 0, 30};

	 PlayerEffect* newEffect;

	newEffect = new PlayerEffect;

	newEffect->Initialize(model_, effectPos);
	//
	effect_.push_back(newEffect);

	newEffect->MoveEffect();
	
}

void Player::SetParent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent; 
}