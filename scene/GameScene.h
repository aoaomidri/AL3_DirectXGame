#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "DebugCamera.h"
#include"SkyDome.h"
#include"RailCamera.h"
#include"EnemyBullet.h"
#include"EnemyEffect.h"
#include<filesystem>
#include<sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
	
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	//敵の発生
	void PopEnemy(Vector3& position);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadenemyPopData();

	// 敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	//敵弾を追加する
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	//敵エフェクトを追加する
	void AddEnemyEffect(EnemyEffect* enemyEffect);

	//敵のリストを取得
	const std::list<Enemy*>& GetEnemys() { return enemys_; }
	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() { return enemyBullets_; }

	Vector3 GetWorldPosition() {
		Vector3 worldPos(0, 0, 0);

		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];

		return worldPos;
	}

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle = 0;
	uint32_t textureHandleEnemy = 0;
	uint32_t textureHandleSkydome = 0;

	//3Dモデル
	Model* model = nullptr;

	Model* modelEnemy = nullptr;

	Model* modelSkydome_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	WorldTransform worldTransform_;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	std::list<Enemy*> enemys_;
	// 敵の弾
	std::list<EnemyBullet*> enemyBullets_;
	//敵のエフェクト
	std::list<EnemyEffect*> enemyEffects_;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	//待機フラグ
	bool waitFlag_ = false;
	//待機タイマー
	int32_t waitTimer_ = 0;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//天球
	SkyDome* skydome_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
