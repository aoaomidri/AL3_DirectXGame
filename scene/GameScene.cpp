#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model;
	delete modelEnemy;
	delete debugCamera_;
	delete player_;
	delete enemy_;
}

void GameScene::Initialize() {
	//初期化類
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle = TextureManager::Load("white1x1.png");
	textureHandleEnemy = TextureManager::Load("Danger.png");
	model = Model::Create();
	modelEnemy = Model::Create();
	viewProjection.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialaize(model, textureHandle);
	//敵キャラの生成
	enemy_ = new Enemy();
	//敵キャラの生成
	enemy_->Initialaize(modelEnemy, textureHandleEnemy);
	enemy_->SetPlayer(player_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);
}

void GameScene::Update() {

	//自キャラの更新
	player_->Update();
	if (enemy_) {
		enemy_->Update();
	}

	#ifdef _DEBUG
	if (isDebugCameraActive_==false) {
		if (input_->TriggerKey(DIK_RETURN)) {
			isDebugCameraActive_ = true;
		}
	} else if (isDebugCameraActive_ == true) {
		if (input_->TriggerKey(DIK_RETURN)) {
			isDebugCameraActive_ = false;
		}
	}
	

	#endif // _DEBUG

	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection.matView = debugCamera_->GetViewProjection().matView;
		viewProjection.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection.TransferMatrix();

	} else {
		viewProjection.UpdateMatrix();
	}
	CheckAllCollisions();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection);
	if (enemy_) {
		enemy_->Draw(viewProjection);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() { 
	Vector3 posA(0, 0, 0), posB(0, 0, 0);

	float length = 0.0f;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	// 自エフェクトの取得
	const std::list<PlayerEffect*>& playerEffects = player_->GetEffects();

	#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();
		//AとBの距離
		length = (powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2));
		if (length <= player_->radius + bullet->radius){
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet:playerBullets) {
		posB = bullet->GetWorldPosition();
		length = (powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2));
		if (length <= enemy_->radius + bullet->radius) {
			// 自キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* bulletPL:playerBullets) {
		for (EnemyBullet* bulletEN:enemyBullets) {		
			posA = bulletPL->GetWorldPosition();
			posB = bulletEN->GetWorldPosition();
			length =
			    (powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2));

			if (length <= bulletPL->radius + bulletEN->radius) {
				// 自キャラの衝突時コールバックを呼び出す
				bulletPL->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				bulletEN->OnCollision();
			}		
		}	
	}

	#pragma endregion

	#pragma region 自キャラとエフェクトの当たり判定
	posA = player_->GetWorldPosition();

	for (PlayerEffect* effect:playerEffects) {
		posB = effect->GetWorldPosition();
		// AとBの距離
		length = (powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2));
		if (length <= (player_->radius-1.0f) + effect->radius) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			effect->OnCollision();
		}
	}
#pragma endregion
}
