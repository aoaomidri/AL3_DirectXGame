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
	textureHandlePlayer = TextureManager::Load("Player/PlayerTex.png");
	textureHandleEnemy = TextureManager::Load("Enemy/EnemyTex.png");
	textureHamdleEnemyparts = TextureManager::Load("EnemyParts/EnemyParts.png");
	textureHandleWeapon = TextureManager::Load("Weapon/Sword.png");

	modelSkyDome_.reset(Model::CreateFromOBJ("skyDome", true));
	modelGround_.reset(Model::CreateFromOBJ("Ground", true));
	modelPlayerBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelPlayerHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelPlayerL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelPlayerR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelEnemy_.reset(Model::CreateFromOBJ("Enemy", true));
	modelEnemyL_parts_.reset(Model::CreateFromOBJ("EnemyParts", true));
	modelEnemyR_parts_.reset(Model::CreateFromOBJ("EnemyParts", true));
	modelPlayerWeapon_.reset(Model::CreateFromOBJ("Weapon", true));

	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラモデル配列
	std::vector<Model*> playerModels = {
	    modelPlayerBody_.get(), modelPlayerHead_.get(), modelPlayerL_arm_.get(),
	    modelPlayerR_arm_.get(),  modelPlayerWeapon_.get()};
	// 自キャラの初期化
	player_->Initialize(playerModels);

	//敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	//敵キャラモデル配列
	std::vector<Model*> enemyModels = {
	    modelEnemy_.get(), 
		modelEnemyL_parts_.get(), 
		modelEnemyR_parts_.get()
	};
	//敵キャラの初期化
	enemy_->Initialize(enemyModels);

	//天球の生成
	skyDome_ = std::make_unique<SkyDome>();
	//天球のモデル配列
	std::vector<Model*> skyDomeModels = {modelSkyDome_.get()};
	//天球の初期化
	skyDome_->Initialize(skyDomeModels);

	//地面の生成
	ground_ = std::make_unique<Ground>();
	//地面のモデル配列
	std::vector<Model*> groundModels_ = {modelGround_.get()};
	//地面の初期化
	ground_->Initialize(groundModels_);

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	enemyCamera_ = std::make_unique<EnemyCamera>();
	enemyCamera_->Initialize();
	//エネミーのワールドトランスフォームをカメラにセット
	enemyCamera_->SetTarget(&enemy_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());
	////軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	////軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() { 
	player_->Update();

	enemy_->Update();

	skyDome_->Update();

	ground_->Update();

	followCamera_->Update();

	enemyCamera_->Update();

	#ifdef _DEBUG
	/*if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
	}*/

	if (input_->TriggerKey(DIK_RETURN)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

	/*ImGui::Begin("CameraInforMation");
	ImGui::DragFloat3("CameraRotate", &followCamera_->GetViewProjection().rotation_.x, 0.1f);
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();*/

#endif // _DEBUG

	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		debugCamera_->SetFarZ(1500.0f);
		viewProjection_.matView = enemyCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = enemyCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
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
	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

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

void GameScene::CheckAllCollisions() {
	if (IsCollisionOBBViewFrustum(enemy_->GetOBB(),followCamera_->GetViewingFrustum())) {
		player_->OnCollision();

	} else {
		player_->SetchackCollision();

	}

	if (IsCollisionOBBViewFrustum(player_->GetOBB(), enemyCamera_->GetViewingFrustum())) {
		enemy_->OnCollision();
	} else {
		enemy_->SetchackCollision();
	}

}

bool GameScene::IsCollisionViewFrustum(const OBB& obb, const ViewingFrustum& viewingFrustum) {
	std::unique_ptr<MyVector> vec = std::make_unique<MyVector>();
	std::unique_ptr<MyMatrix> mat = std::make_unique<MyMatrix>();
	/*ステップ1視錐台の生成*/
	// 頂点の個数
	const int OBBVertex = 8;
	const int FrustumVertex = 8;

	// 法線の個数
	const int normalLine = 6;

	// 当たる距離
	const float CollisionDistance = 0.00f;

	// それぞれの幅
	Vector2 nearPlane{};
	Vector2 farPlane{};
	// 面の頂点
	Vector3 nearPlanePoints_[4] = {0};
	Vector3 farPlanePoints_[4] = {0};
	// 視錐台の行列
	Matrix4x4 FrustumMatWorld = mat->MakeAffineMatrix(
	    {1.0f, 1.0f, 1.0f}, {viewingFrustum.rotate_}, {viewingFrustum.translation_});

	// 向きベクトルnear
	Vector3 directionNear = vec->Normalize(viewingFrustum.direction);
	directionNear.z = directionNear.z * viewingFrustum.nearZ;
	// 向きベクトルfar
	Vector3 directionFar = vec->Normalize(viewingFrustum.direction);
	directionFar.z = directionFar.z * viewingFrustum.farZ;

	// 近平面の縦横
	nearPlane.y = vec->Length(directionNear) * std::tan(viewingFrustum.verticalFOV / 2);
	nearPlane.x = nearPlane.y * viewingFrustum.aspectRatio;
	// 遠平面の縦横
	farPlane.y = vec->Length(directionFar) * std::tan(viewingFrustum.verticalFOV / 2);
	farPlane.x = farPlane.y * viewingFrustum.aspectRatio;

	nearPlanePoints_[0] = {
	    directionNear.x + -nearPlane.x, directionNear.y + nearPlane.y, directionNear.z}; // 左上
	nearPlanePoints_[1] = {
	    directionNear.x + nearPlane.x, directionNear.y + nearPlane.y, directionNear.z}; // 右上
	nearPlanePoints_[2] = {
	    directionNear.x + -nearPlane.x, directionNear.y + -nearPlane.y, directionNear.z}; // 左下
	nearPlanePoints_[3] = {
	    directionNear.x + nearPlane.x, directionNear.y + -nearPlane.y, directionNear.z}; // 右下

	farPlanePoints_[0] = {
	    directionFar.x + -farPlane.x, directionFar.y + farPlane.y, directionFar.z}; // 左上
	farPlanePoints_[1] = {
	    directionFar.x + farPlane.x, directionFar.y + farPlane.y, directionFar.z}; // 右上
	farPlanePoints_[2] = {
	    directionFar.x + -farPlane.x, directionFar.y + -farPlane.y, directionFar.z}; // 左下
	farPlanePoints_[3] = {
	    directionFar.x + farPlane.x, directionFar.y + -farPlane.y, directionFar.z}; // 右下

	// 頂点
	Vector3 FrustumPoints[FrustumVertex]{0};
	// near
	FrustumPoints[0] = nearPlanePoints_[0];
	FrustumPoints[1] = nearPlanePoints_[1];
	FrustumPoints[2] = nearPlanePoints_[2];
	FrustumPoints[3] = nearPlanePoints_[3];
	// far
	FrustumPoints[4] = farPlanePoints_[0];
	FrustumPoints[5] = farPlanePoints_[1];
	FrustumPoints[6] = farPlanePoints_[2];
	FrustumPoints[7] = farPlanePoints_[3];

	/*ステップ2 OBBの生成*/

	Vector3 obbPoints[OBBVertex]{};

	// obbの行列
	Matrix4x4 worldMatrix = {
	    obb.orientations[0].x, obb.orientations[0].y, obb.orientations[0].z, 0,
	    obb.orientations[1].x, obb.orientations[1].y, obb.orientations[1].z, 0,
	    obb.orientations[2].x, obb.orientations[2].y, obb.orientations[2].z, 0,
	    obb.center.x,          obb.center.y,          obb.center.z,          1};

	obbPoints[0] = obb.size * -1;
	obbPoints[1] = {obb.size.x * -1, obb.size.y * -1, obb.size.z};
	obbPoints[2] = {obb.size.x, obb.size.y * -1, obb.size.z * -1};
	obbPoints[3] = {obb.size.x, obb.size.y * -1, obb.size.z};
	obbPoints[4] = {obb.size.x * -1, obb.size.y, obb.size.z * -1};
	obbPoints[5] = {obb.size.x * -1, obb.size.y, obb.size.z};
	obbPoints[6] = {obb.size.x, obb.size.y, obb.size.z * -1};
	obbPoints[7] = obb.size;

	for (int i = 0; i < OBBVertex; i++) {
		obbPoints[i] = vec->TransformNormal(obbPoints[i], worldMatrix);
		obbPoints[i] = obb.center + obbPoints[i];
	}

	/*ステップ3 OBBを視錐台のローカル座標に変換*/
	// 視錐台の逆行列
	Matrix4x4 FrustumInverceMat = mat->Inverce(FrustumMatWorld);
	for (int i = 0; i < OBBVertex; i++) {
		obbPoints[i] = vec->Transform(obbPoints[i], FrustumInverceMat);
	}

	/*ステップ4 当たり判定*/
	// near面から
	Vector3 v01 = FrustumPoints[1] - FrustumPoints[0];
	Vector3 v12 = FrustumPoints[2] - FrustumPoints[1];

	// far
	Vector3 v65 = FrustumPoints[5] - FrustumPoints[6];
	Vector3 v54 = FrustumPoints[4] - FrustumPoints[5];

	// left
	Vector3 v02 = FrustumPoints[2] - FrustumPoints[0];
	Vector3 v26 = FrustumPoints[6] - FrustumPoints[2];

	// right
	Vector3 v53 = FrustumPoints[3] - FrustumPoints[5];
	Vector3 v31 = FrustumPoints[1] - FrustumPoints[3];

	// up
	Vector3 v41 = FrustumPoints[1] - FrustumPoints[4];
	Vector3 v10 = FrustumPoints[0] - FrustumPoints[1];

	// down
	Vector3 v23 = FrustumPoints[3] - FrustumPoints[2];
	Vector3 v36 = FrustumPoints[6] - FrustumPoints[3];

	Vector3 normal[normalLine] = {};

	float distance[48] = {};
	// near
	normal[0] = vec->Normalize(vec->Cross(v01, v12));
	// far
	normal[1] = vec->Normalize(vec->Cross(v65, v54));
	// left
	normal[2] = vec->Normalize(vec->Cross(v02, v26));
	// right
	normal[3] = vec->Normalize(vec->Cross(v53, v31));
	// up
	normal[4] = vec->Normalize(vec->Cross(v41, v10));
	// down
	normal[5] = vec->Normalize(vec->Cross(v23, v36));
	for (int i = 0; i < 8; i++) {
		distance[0 + i * normalLine] = vec->Dot(obbPoints[i] - FrustumPoints[0], normal[0]);
		distance[1 + i * normalLine] = vec->Dot(obbPoints[i] - FrustumPoints[4], normal[1]);
		distance[2 + i * normalLine] = vec->Dot(obbPoints[i] - FrustumPoints[0], normal[2]);
		distance[3 + i * normalLine] = vec->Dot(obbPoints[i] - FrustumPoints[1], normal[3]);
		distance[4 + i * normalLine] = vec->Dot(obbPoints[i] - FrustumPoints[0], normal[4]);
		distance[5 + i * normalLine] = vec->Dot(obbPoints[i] - FrustumPoints[2], normal[5]);
	}

	for (int i = 0; i < 8; i++) {
		if (distance[0 + i * normalLine] <= CollisionDistance &&
		    distance[1 + i * normalLine] <= CollisionDistance &&
		    distance[2 + i * normalLine] <= CollisionDistance &&
		    distance[3 + i * normalLine] <= CollisionDistance &&
		    distance[4 + i * normalLine] <= CollisionDistance &&
		    distance[5 + i * normalLine] <= CollisionDistance) {
			return true;
		}
	}

	return false;
}

bool GameScene::IsCollisionOBB(const OBB& obb, const ViewingFrustum& viewingFrustum) {
	std::unique_ptr<MyVector> vec = std::make_unique<MyVector>();
	std::unique_ptr<MyMatrix> mat = std::make_unique<MyMatrix>();
	/*ステップ1視錐台の生成*/
	// 頂点の個数
	const int OBBVertex = 8;
	const int FrustumVertex = 8;

	// 法線の個数
	const int normalLine = 6;

	// 当たる距離
	const float CollisionDistance = 0.00f;

	// それぞれの幅
	Vector2 nearPlane{};
	Vector2 farPlane{};
	// 面の頂点
	Vector3 nearPlanePoints_[4] = {0};
	Vector3 farPlanePoints_[4] = {0};
	// 視錐台の行列
	Matrix4x4 FrustumMatWorld = mat->MakeAffineMatrix(
	    {1.0f, 1.0f, 1.0f}, {viewingFrustum.rotate_}, {viewingFrustum.translation_});

	// 向きベクトルnear
	Vector3 directionNear = vec->Normalize(viewingFrustum.direction);
	directionNear.z = directionNear.z * viewingFrustum.nearZ;
	// 向きベクトルfar
	Vector3 directionFar = vec->Normalize(viewingFrustum.direction);
	directionFar.z = directionFar.z * viewingFrustum.farZ;

	// 近平面の縦横
	nearPlane.y = vec->Length(directionNear) * std::tan(viewingFrustum.verticalFOV / 2);
	nearPlane.x = nearPlane.y * viewingFrustum.aspectRatio;
	// 遠平面の縦横
	farPlane.y = vec->Length(directionFar) * std::tan(viewingFrustum.verticalFOV / 2);
	farPlane.x = farPlane.y * viewingFrustum.aspectRatio;

	nearPlanePoints_[0] = {
	    directionNear.x + -nearPlane.x, directionNear.y + nearPlane.y, directionNear.z}; // 左上
	nearPlanePoints_[1] = {
	    directionNear.x + nearPlane.x, directionNear.y + nearPlane.y, directionNear.z}; // 右上
	nearPlanePoints_[2] = {
	    directionNear.x + -nearPlane.x, directionNear.y + -nearPlane.y, directionNear.z}; // 左下
	nearPlanePoints_[3] = {
	    directionNear.x + nearPlane.x, directionNear.y + -nearPlane.y, directionNear.z}; // 右下

	farPlanePoints_[0] = {
	    directionFar.x + -farPlane.x, directionFar.y + farPlane.y, directionFar.z}; // 左上
	farPlanePoints_[1] = {
	    directionFar.x + farPlane.x, directionFar.y + farPlane.y, directionFar.z}; // 右上
	farPlanePoints_[2] = {
	    directionFar.x + -farPlane.x, directionFar.y + -farPlane.y, directionFar.z}; // 左下
	farPlanePoints_[3] = {
	    directionFar.x + farPlane.x, directionFar.y + -farPlane.y, directionFar.z}; // 右下

	for (int i = 0; i < 4; i++) {
		nearPlanePoints_[i] = vec->TransformNormal(nearPlanePoints_[i], FrustumMatWorld);
		farPlanePoints_[i] = vec->TransformNormal(farPlanePoints_[i], FrustumMatWorld);
	}

	// 頂点
	Vector3 FrustumPoints[FrustumVertex]{0};
	// near
	FrustumPoints[0] = viewingFrustum.translation_ + nearPlanePoints_[0];
	FrustumPoints[1] = viewingFrustum.translation_ + nearPlanePoints_[1];
	FrustumPoints[2] = viewingFrustum.translation_ + nearPlanePoints_[2];
	FrustumPoints[3] = viewingFrustum.translation_ + nearPlanePoints_[3];
	// far
	FrustumPoints[4] = viewingFrustum.translation_ + farPlanePoints_[0];
	FrustumPoints[5] = viewingFrustum.translation_ + farPlanePoints_[1];
	FrustumPoints[6] = viewingFrustum.translation_ + farPlanePoints_[2];
	FrustumPoints[7] = viewingFrustum.translation_ + farPlanePoints_[3];

	/*ステップ2 OBBの生成*/

	Vector3 obbPoints[OBBVertex]{};

	// obbの行列
	Matrix4x4 worldMatrix = {
	    obb.orientations[0].x, obb.orientations[0].y, obb.orientations[0].z, 0,
	    obb.orientations[1].x, obb.orientations[1].y, obb.orientations[1].z, 0,
	    obb.orientations[2].x, obb.orientations[2].y, obb.orientations[2].z, 0,
	    obb.center.x,          obb.center.y,          obb.center.z,          1};

	// 手前
	obbPoints[0] = {obb.size.x * -1, obb.size.y, obb.size.z * -1}; // 左上
	obbPoints[1] = {obb.size.x, obb.size.y, obb.size.z * -1};      // 右上
	obbPoints[2] = obb.size * -1;                                  // 左下
	obbPoints[3] = {obb.size.x, obb.size.y * -1, obb.size.z * -1}; // 右下
	// 奥
	obbPoints[4] = {obb.size.x * -1, obb.size.y, obb.size.z};      // 左上
	obbPoints[5] = obb.size;                                       // 右上
	obbPoints[6] = {obb.size.x * -1, obb.size.y * -1, obb.size.z}; // 左下
	obbPoints[7] = {obb.size.x, obb.size.y * -1, obb.size.z};      // 右下

	/*ステップ3 視錐台をOBBのローカル座標に変換*/
	// OBBの逆行列
	Matrix4x4 OBBInverceMat = mat->Inverce(worldMatrix);
	for (int i = 0; i < OBBVertex; i++) {
		FrustumPoints[i] = vec->Transform(FrustumPoints[i], OBBInverceMat);
	}

	/*ステップ4 当たり判定*/
	// near面から
	Vector3 v01 = obbPoints[1] - obbPoints[0];
	Vector3 v12 = obbPoints[2] - obbPoints[1];

	// far
	Vector3 v65 = obbPoints[5] - obbPoints[6];
	Vector3 v54 = obbPoints[4] - obbPoints[5];

	// left
	Vector3 v02 = obbPoints[2] - obbPoints[0];
	Vector3 v26 = obbPoints[6] - obbPoints[2];

	// right
	Vector3 v53 = obbPoints[3] - obbPoints[5];
	Vector3 v31 = obbPoints[1] - obbPoints[3];

	// up
	Vector3 v41 = obbPoints[1] - obbPoints[4];
	Vector3 v10 = obbPoints[0] - obbPoints[1];

	// down
	Vector3 v23 = obbPoints[3] - obbPoints[2];
	Vector3 v36 = obbPoints[6] - obbPoints[3];

	Vector3 normal[normalLine] = {};

	float distance[48] = {};
	// near
	normal[0] = vec->Normalize(vec->Cross(v01, v12));
	// far
	normal[1] = vec->Normalize(vec->Cross(v65, v54));
	// left
	normal[2] = vec->Normalize(vec->Cross(v02, v26));
	// right
	normal[3] = vec->Normalize(vec->Cross(v53, v31));
	// up
	normal[4] = vec->Normalize(vec->Cross(v41, v10));
	// down
	normal[5] = vec->Normalize(vec->Cross(v23, v36));
	for (int i = 0; i < 8; i++) {
		distance[0 + i * normalLine] = vec->Dot(FrustumPoints[i] - obbPoints[0], normal[0]);
		distance[1 + i * normalLine] = vec->Dot(FrustumPoints[i] - obbPoints[4], normal[1]);
		distance[2 + i * normalLine] = vec->Dot(FrustumPoints[i] - obbPoints[0], normal[2]);
		distance[3 + i * normalLine] = vec->Dot(FrustumPoints[i] - obbPoints[1], normal[3]);
		distance[4 + i * normalLine] = vec->Dot(FrustumPoints[i] - obbPoints[0], normal[4]);
		distance[5 + i * normalLine] = vec->Dot(FrustumPoints[i] - obbPoints[2], normal[5]);
	}

	for (int i = 0; i < 8; i++) {
		if (distance[0 + i * normalLine] <= CollisionDistance &&
		    distance[1 + i * normalLine] <= CollisionDistance &&
		    distance[2 + i * normalLine] <= CollisionDistance &&
		    distance[3 + i * normalLine] <= CollisionDistance &&
		    distance[4 + i * normalLine] <= CollisionDistance &&
		    distance[5 + i * normalLine] <= CollisionDistance) {
			return true;
		}
	}

	return false;
}

bool GameScene::IsCollisionOBBViewFrustum(const OBB& obb, const ViewingFrustum& viewingFrustum) {
	if (IsCollisionOBB(obb, viewingFrustum) || IsCollisionViewFrustum(obb, viewingFrustum)) {
		return true;
	} else {
		return false;
	}
}
