#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include<imgui.h>
#include<fstream>
#include<iostream>

GameScene::GameScene() { 
	
}

GameScene::~GameScene() { 
	delete model;
	delete modelEnemy;
	delete debugCamera_;
	delete player_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;

	for (Enemy* enemy: enemys_) {
		delete enemy;
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

	for (EnemyEffect* effect : enemyEffects_) {
		delete effect;
	}
}

void GameScene::Initialize() {
	//初期化類
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle = TextureManager::Load("white1x1.png");
	textureHandleEnemy = TextureManager::Load("Danger.png");
	textureHandleSkydome = TextureManager::Load("skyDome/skyDome.jpg");
	TextureManager::Load("Magic.png");
	model = Model::Create();
	modelEnemy = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skyDome", true);
	viewProjection_.Initialize();
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, -50.0f};
	LoadenemyPopData();


	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialaize(model, textureHandle);
	
	//天球の生成
	skydome_ = new SkyDome();
	skydome_->Initialize(modelSkydome_, textureHandleSkydome);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//レールカメラの生成
	railCamera_ = new RailCamera();
	railCamera_->Initialize(worldTransform_.matWorld_, worldTransform_.rotation_);

	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldMatrix());
	////軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	////軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	UpdateEnemyPopCommands();

	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	enemyEffects_.remove_if([](EnemyEffect* effect) {
		if (effect->IsDead()) {
			delete effect;
			return true;
		}
		return false;
	});


	//自キャラの更新
	player_->Update(
	    railCamera_->GetViewProjection().matView, railCamera_->GetViewProjection().matProjection);
	for (Enemy* enemy:enemys_) {
		enemy->Update();
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	for (EnemyEffect* Effect : enemyEffects_) {
		Effect->Update();
	}

	skydome_->Update();

	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
	

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
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();

	} else {

		
		//viewProjection_.UpdateMatrix();
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
	player_->Draw(viewProjection_);

	for (Enemy* enemy:enemys_) {
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	for (EnemyEffect* effect : enemyEffects_) {
		effect->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	player_->DrawUI();
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
	////敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	// 自エフェクトの取得
	const std::list<PlayerEffect*>& playerEffects = player_->GetEffects();

	#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetPlayerWorldPosition();
	for (EnemyBullet* bullet : enemyBullets_) {
		posB = bullet->GetWorldPosition();
		//AとBの距離
		length = (powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2));
		if (length <= powf(player_->radius + bullet->radius, 2)) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と全ての敵キャラの当たり判定
	for (Enemy* enemy:enemys_) {
		for (PlayerBullet* bullet : playerBullets) {
			posA = enemy->GetWorldPosition();
			posB = bullet->GetWorldPosition();
			length =
			    (powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2));
			if (length <= powf(enemy->radius + bullet->radius, 2)) {
				// 敵キャラの衝突時コールバックを呼び出す
 				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
	
	#pragma endregion

	#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* bulletPL:playerBullets) {
		for (EnemyBullet* bulletEN : enemyBullets_) {		
			posA = bulletPL->GetWorldPosition();
			posB = bulletEN->GetWorldPosition();
			length =
			    (powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2));

			if (length <= powf(bulletPL->radius + bulletEN->radius,2)) {
				// 自キャラの衝突時コールバックを呼び出す
				bulletPL->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				bulletEN->OnCollision();
			}		
		}	
	}

	#pragma endregion

	#pragma region 自キャラとエフェクトの当たり判定
	posA = player_->GetPlayerWorldPosition();

	for (PlayerEffect* effect:playerEffects) {
		posB = effect->GetWorldPosition();
		// AとBの距離
		length = (powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2));
		if (length <= powf((player_->radius - 1.0f) + effect->radius, 2)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			effect->OnCollision();
		}
	}
#pragma endregion
}

void GameScene::LoadenemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();

}

void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (waitFlag_) {
		waitTimer_--;
		if (waitTimer_ <= 0){
			//待機完了
			waitFlag_ = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (std::getline(enemyPopCommands,line)) {
		//１行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			//x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			
			// z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			
			Vector3 enemyPosition = {x, y, z};
			//敵を発生させる
			PopEnemy(enemyPosition);
		}

		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');
			//待ち時間
			int32_t waitTime = atoi(word.c_str());
			//待機開始
			waitFlag_ = true;
			waitTimer_ = waitTime;

			//コマンドループを抜ける
			break;
		}
	}

}

void GameScene::PopEnemy(Vector3& position) {
	////敵キャラの生成
	Enemy* newEnemy_ = new Enemy();
	////敵キャラの生成
	newEnemy_->Initialaize(modelEnemy, textureHandleEnemy, position);
	
	newEnemy_->SetPlayer(player_);
	newEnemy_->SetGameScene(this);

	enemys_.push_back(newEnemy_);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//リストに登録する
	
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddEnemyEffect(EnemyEffect* enemyEffect) {
	//リストに追加する
	enemyEffects_.push_back(enemyEffect);
}
