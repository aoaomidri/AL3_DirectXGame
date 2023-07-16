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
#include"Enemy.h"
#include"SkyDome.h"
#include"Ground.h"
#include "DebugCamera.h"
#include"FollowCamera.h"
#include<filesystem>
#include<sstream>
#include<memory>
#include<imgui.h>

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle = 0;
	uint32_t textureHandleSkydome = 0;
	uint32_t textureHandleGround = 0;
	uint32_t textureHandlePlayer = 0;
	uint32_t textureHandleEnemy = 0;
	uint32_t textureHamdleEnemyparts = 0;
	uint32_t textureHandleWeapon = 0;
	// 3Dモデル
	std::unique_ptr<Model> modelSkyDome_;

	std::unique_ptr<Model> modelGround_;
	/*プレイヤーのモデル*/
	std::unique_ptr<Model> modelPlayerBody_;
	std::unique_ptr<Model> modelPlayerHead_;
	std::unique_ptr<Model> modelPlayerL_arm_;
	std::unique_ptr<Model> modelPlayerR_arm_;
	/*エネミーのモデル*/
	std::unique_ptr<Model> modelEnemy_;
	std::unique_ptr<Model> modelEnemyL_parts_;
	std::unique_ptr<Model> modelEnemyR_parts_;
	/*武器のモデル*/
	std::unique_ptr<Model> modelPlayerWeapon_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	WorldTransform worldTransform_;

	// 自キャラ
	std::unique_ptr<Player> player_;

	//敵キャラ
	std::unique_ptr<Enemy> enemy_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;

	//プレイヤーを追うカメラ
	std::unique_ptr<FollowCamera> followCamera_;

	//天球
	std::unique_ptr<SkyDome> skyDome_;

	//地面
	std::unique_ptr<Ground> ground_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
