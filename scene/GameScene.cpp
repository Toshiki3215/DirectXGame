#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <cassert> 

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
	delete debugCamera_;
}

Matrix4 ScaleMatrix4(Matrix4 matWorld, Vector3 scale);

Matrix4 RotationXMatrix4(Matrix4 matWorld, Vector3 rotation);

Matrix4 RotationYMatrix4(Matrix4 matWorld, Vector3 rotation);

Matrix4 RotationZMatrix4(Matrix4 matWorld, Vector3 rotation);

Matrix4 MoveMatrix4(Matrix4 matWorld, Vector3 translation);

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.scale_ = {1, 1, 1};

	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	worldTransform_.translation_ = {0, 0, 0};

	Matrix4 matScale;

	Matrix4 matRotY = MathUtility::Matrix4Identity();

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//視点の移動ベクトル
	moveTarget = Vector3(0, 0, 0);


	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	////カメラ視点座標を設定
	//viewProjection_.eye = {0.0f, 0.0f, -10.0f};

	////カメラ注視点座標を設定
	//viewProjection_.target = {0, 0, 0};

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280,720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
}

void GameScene::Update() {

	//スケール
	float scaleSize = 0.2f;

	//回転
	float rSpeed = 3.14 / 90;

	//移動
	float kSpeed = 0.1f;

	//↑,↓でY軸拡大縮小
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.scale_.y += scaleSize;
	} else if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.scale_.y -= scaleSize;
	}

	//←,→でX軸拡大縮小
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.scale_.x -= scaleSize;
	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.scale_.x += scaleSize;
	}

	// A,Dで回転(Y軸回転)
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += rSpeed;
	} else if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= rSpeed;
	}

	moveTarget = {sinf(worldTransform_.rotation_.y), 0, cosf(worldTransform_.rotation_.y)};

	// W,Sで移動
	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.x += moveTarget.x * kSpeed;
		worldTransform_.translation_.z += moveTarget.z * kSpeed;
	} else if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.x += moveTarget.x * -kSpeed;
		worldTransform_.translation_.z += moveTarget.z * -kSpeed;
	}

	//行列変換
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	worldTransform_.matWorld_ = ScaleMatrix4(worldTransform_.matWorld_, worldTransform_.scale_);

	worldTransform_.matWorld_ = RotationYMatrix4(worldTransform_.matWorld_, worldTransform_.rotation_);

	worldTransform_.matWorld_ = MoveMatrix4(worldTransform_.matWorld_, worldTransform_.translation_);

	//行列の再計算
	worldTransform_.TransferMatrix();

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
	//3Dモデル描画
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	/*PrimitiveDrawer::GetInstance()->DrawLine3d(
	  Vector3(0, 0, 0), Vector3(500, 200, 500), Vector4(255, 0, 0, 0));*/

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

Matrix4 ScaleMatrix4(Matrix4 matWorld, Vector3 scale) {
	Matrix4 matScale = MathUtility::Matrix4Identity();

	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;

	return matWorld *= matScale;
}

Matrix4 RotationXMatrix4(Matrix4 matWorld, Vector3 rotation) {
	Matrix4 matRotX = MathUtility::Matrix4Identity();

	matRotX.m[1][1] = cosf(rotation.x);
	matRotX.m[1][2] = sinf(rotation.x);
	matRotX.m[2][1] = -sinf(rotation.x);
	matRotX.m[2][2] = cosf(rotation.x);

	return matWorld *= matRotX;
}

Matrix4 RotationYMatrix4(Matrix4 matWorld, Vector3 rotation) {
	Matrix4 matRotY = MathUtility::Matrix4Identity();

	matRotY.m[0][0] = cosf(rotation.y);
	matRotY.m[0][2] = -sinf(rotation.y);
	matRotY.m[2][0] = sinf(rotation.y);
	matRotY.m[2][2] = cosf(rotation.y);

	return matWorld *= matRotY;
}

Matrix4 RotationZMatrix4(Matrix4 matWorld, Vector3 rotation) {
	Matrix4 matRotZ = MathUtility::Matrix4Identity();

	matRotZ.m[0][0] = cosf(rotation.z);
	matRotZ.m[0][1] = sinf(rotation.z);
	matRotZ.m[1][0] = -sinf(rotation.z);
	matRotZ.m[1][1] = cosf(rotation.z);

	return matWorld *= matRotZ;
}

Matrix4 MoveMatrix4(Matrix4 matWorld, Vector3 translation) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;

	return matWorld *= matTrans;
}