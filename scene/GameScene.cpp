#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"ImGuiManager.h"
#include"PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//サウンドデータの読み込み
	soundDateHandle_ = audio_->LoadWave("fanfare.wav");
	//音声再生
	voiceHandle_ = audio_->PlayWave(soundDateHandle_,true);
	//
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
}

void GameScene::Update() { 
	Vector2 position = sprite_->GetPosition(); 
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);
	debugCamera_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
	//音声停止
		audio_->StopWave(voiceHandle_);
	}
	

	ImGui::Begin("Debug1");
	ImGui::Text("Kagerou Days %d.%d.%d", 2011, 8, 15);
	//入力ボックス
	ImGui::InputFloat3("inputFloat3", inputFloat3);
	//float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	//デモウィンドウの表示を有効化
	ImGui::ShowDemoWindow();
	ImGui::End();
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
	sprite_->Draw();

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
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, -10, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
	
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
