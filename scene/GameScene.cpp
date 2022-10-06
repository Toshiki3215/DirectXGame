#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	//自キャラの解放
	delete player_;

	delete enemy_;

	delete modelSkydome_;

	//delete playerModel_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_);

	//エネミーの生成
	enemy_ = new Enemy();

	//エネミーの初期化
	enemy_->Initilize(model_);

	//天球の生成
	skydome_ = new Skydome();

	//天球の初期化
	skydome_->Initialize(model_);

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//playerModel_ = Model::CreateFromOBJ("Player", true);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	viewProjection_ = debugCamera_->GetViewProjection();

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//カメラ視点座標を設定
	viewProjection_.eye = {0.0f, 0.0f, -10.0f};

	//カメラ注視点座標を設定
	viewProjection_.target = {0, 0, 0};

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	//自キャラの更新
	player_->UpDate(viewProjection_);

	const float kEnemySpeed = 0.5f;
	Vector3 velocity(0, 0, kEnemySpeed);

	enemy_->Update(velocity);

	skydome_->Update();

	//デバッグカメラの更新
	debugCamera_->Update();
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
	// 3Dモデル描画
	
	//自キャラの描画
	player_->Draw(viewProjection_);

	//エネミーの描画
	enemy_->Draw(viewProjection_);

	//天球の描画
	skydome_->Draw(viewProjection_);

	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->DrawLine3d(
	  Vector3(0, 0, 0), Vector3(10, 0, 10), Vector4(255, 255, 0, 0));

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {

	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	#pragma region

	//敵の座標
	posA = enemy_->GetWorldPos();

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {

		posB = bullet->GetWorldPos();

		/*float pos = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		            (posB.z - posA.z) * (posB.z - posA.z);*/

		Vector3 distance = posB - posA;

		float R1 = 500;
		float R2 = 100;

		/* if (
		  (distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <=
		  (R1 + R2)) {
			enemy_->OnCollision();

			bullet->OnCollision();
		}*/

		 if (
		  (distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <=
		  (R1 + R2) * 2) {
			enemy_->OnCollision();

			bullet->OnCollision();
		}

		/*float R = (R1 + R2) * 1000;

		if (R >= pos) {
			enemy_->OnCollision();

			bullet->OnCollision();
			}*/
	}

	#pragma endregion

}