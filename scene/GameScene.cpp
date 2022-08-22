#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	//自キャラの解放
	delete player_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = TextureManager::Load("mario.jpg");

	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//// 3Dモデルの生成
	//model_ = Model::Create();

	////ワールドトランスフォームの初期化
	//worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	//worldTransform_.translation_ = {0, 0, 0};

	//worldTransform_.scale_ = {0.5,0.5,0.2};

	//Matrix4 matRotZ = MathUtility::Matrix4Identity();
	//Matrix4 matRotY = MathUtility::Matrix4Identity();

	//Matrix4 matTrans = MathUtility::Matrix4Identity();

	////視点の移動ベクトル
	//moveTarget = Vector3(0, 0, 0);

	//rotaTarget = Vector3(0, 0, 0);

	//worldTransform_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	viewProjection_ = debugCamera_->GetViewProjection();

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//カメラ視点座標を設定
	viewProjection_.eye = {0.0f, 0.0f, -10.0f};

	//カメラ注視点座標を設定
	viewProjection_.target = {0, 0, 0};

	//viewProjection_.target = worldTransform_.translation_;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	//自キャラの更新
	player_->UpDate();

	//カメラ追従
	/*viewProjection_.eye.x = -rotaTarget.x * 10 + worldTransform_.translation_.x;

	viewProjection_.eye.z = -rotaTarget.z * 10 + worldTransform_.translation_.z;

	viewProjection_.target = {
	  rotaTarget.x * 10 + worldTransform_.translation_.x, 0,
	  rotaTarget.z * 10 + worldTransform_.translation_.z};*/

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ用表示
	/*debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "transform: %f, %f, %f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);*/

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
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	
	//自キャラの描画
	player_->Draw();

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
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}