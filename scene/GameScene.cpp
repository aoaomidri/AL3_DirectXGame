#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include<imgui.h>
#include<fstream>
#include<iostream>

GameScene::GameScene() {}

GameScene::~GameScene() {

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle = TextureManager::Load("white1x1.png");
	textureHandleSkydome = TextureManager::Load("skyDome/skyDome.jpg");
	textureHandleGround = TextureManager::Load("Ground/firld.png");

	model_.reset(Model::Create());
	modelSkyDome_.reset(Model::CreateFromOBJ("skyDome", true));
	modelGround_.reset(Model::CreateFromOBJ("Ground", true));

	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, -50.0f};

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialaize(model_.get(), textureHandle);

	//天球の生成
	skyDome_ = std::make_unique<SkyDome>();
	//天球の初期化
	skyDome_->Initialize(modelSkyDome_.get(), textureHandleSkydome);

	//地面の生成
	ground_ = std::make_unique<Ground>();
	//地面の初期化
	ground_->Initialize(modelGround_.get(), textureHandleGround);

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	////軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	////軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() { 
	player_->Update();

	skyDome_->Update();

	ground_->Update();

	#ifdef _DEBUG
	if (isDebugCameraActive_ == false) {
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
	}
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
	player_->Draw(viewProjection_);

	skyDome_->Draw(viewProjection_);

	ground_->Draw(viewProjection_);

	/// </summary>

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
