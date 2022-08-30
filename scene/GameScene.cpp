#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

Matrix4 ScaleMatrix4(Matrix4 matWorld, Vector3 scale);

Matrix4 MoveMatrix4(Matrix4 matWorld, Vector3 translation);

Matrix4 RotationZMatrix4(Matrix4 matWorld, Vector3 rotation);
Matrix4 RotationYMatrix4(Matrix4 matWorld, Vector3 rotation);

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	rotaCount = 0;

	rotaPlayer = 0;

	count = 0;

	rotaTime = 0;

	//ワールドトランスフォームの初期化
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	worldTransform_.translation_ = {0, 0, 0};

	worldTransform_.scale_ = {0.5,0.5,0.2};

	Matrix4 matRotZ = MathUtility::Matrix4Identity();
	Matrix4 matRotY = MathUtility::Matrix4Identity();

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//視点の移動ベクトル
	moveTarget = Vector3(0, 0, 0);

	rotaTarget = Vector3(0, 0, 0);

	worldTransform_.Initialize();

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

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//カメラ視点座標を設定
	viewProjection_.eye = {0.0f, 0.0f, -10.0f};

	//カメラ注視点座標を設定
	viewProjection_.target = {0, 0, 0};

	viewProjection_.target = worldTransform_.translation_;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	float rSpeed = 0;
	float kSpeed = 0.1f;

	//視点の移動速さ
	float EyeSpeed = 0.0f;
	float sEyeSpeed = 0.0f;

	float fSpeed = 0.02f;
	float sSpeed = 0.02f;
	float speed = 0.02f;

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	Matrix4 matRotZ = MathUtility::Matrix4Identity();
	Matrix4 matRotY = MathUtility::Matrix4Identity();

	kSpeed = 0.1f;

	// ----- 回避(回転モーション) ----- 
	if (input_->PushKey(DIK_SPACE)) {
		rotaTime = 20;
	}

	if (rotaTime>0) {
		rotaTime--;
		rSpeed = 3.14 / 10;
		worldTransform_.rotation_.z -= rSpeed;
		if (rotaTime <= 0) {
			rSpeed = 0;
			worldTransform_.rotation_.z = 0;
			rotaTime = 0;
		}
	}
	
	// ----- 視点変更(視点回転) ----- 
	if (input_->PushKey(DIK_LEFT)) {
		if (rotaCount == 0) {
			rSpeed = 3.14 / 2;
			rotaPlayer += rSpeed;
			rSpeed = 0;
			if (rotaPlayer >= 6.28) {
				rotaPlayer = 0;
			}
			rotaCount++;
		}

	} else if (input_->PushKey(DIK_RIGHT)) {
		if (rotaCount == 0) {
			rSpeed = 3.14 / 2;
			rotaPlayer -= rSpeed;
			rSpeed = 0;
			if (rotaPlayer <= -6.28) {
				rotaPlayer = 0;
			}
			rotaCount++;
		}
	}

	moveTarget = {sinf(worldTransform_.rotation_.y), 0, cosf(worldTransform_.rotation_.y)};
	rotaTarget = {sinf(rotaPlayer), 0, cosf(rotaPlayer)};


	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += kSpeed;
	} else if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= kSpeed;
	}

	/*if (input_->PushKey(DIK_A)) {
		if (rotaPlayer / 1.57 == 0) {
			worldTransform_.translation_.x -= kSpeed;
		} else if (rotaPlayer / 1.57 == 1 || rotaPlayer / 1.57 == -3) {
			worldTransform_.translation_.z -= kSpeed;
		} else if (rotaPlayer / 1.57 == 2 || rotaPlayer / 1.57 == -2) {
			worldTransform_.translation_.x += kSpeed;
		} else if (rotaPlayer / 1.57 == 3 || rotaPlayer / 1.57 == -1) {
			worldTransform_.translation_.z += kSpeed;
		}
	} else if (input_->PushKey(DIK_D)) {
		if (rotaPlayer / 1.57 == 0) {
			worldTransform_.translation_.x += kSpeed;
		} else if (rotaPlayer / 1.57 == 1 || rotaPlayer / 1.57 == -3) {
			worldTransform_.translation_.z += kSpeed;
		} else if (rotaPlayer / 1.57 == 2 || rotaPlayer / 1.57 == -2) {
			worldTransform_.translation_.x -= kSpeed;
		} else if (rotaPlayer / 1.57 == 3 || rotaPlayer / 1.57 == -1) {
			worldTransform_.translation_.z -= kSpeed;
		}
	}*/

	/*if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.x += moveTarget.x * kSpeed;
		worldTransform_.translation_.z += moveTarget.z * kSpeed;
	} else if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.x += moveTarget.x * -kSpeed;
		worldTransform_.translation_.z += moveTarget.z * -kSpeed;
	}*/

	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x += rotaTarget.z * kSpeed;
		worldTransform_.translation_.z += rotaTarget.x * kSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += rotaTarget.z * -kSpeed;
		worldTransform_.translation_.z += rotaTarget.x * -kSpeed;
	}

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	worldTransform_.matWorld_ = ScaleMatrix4(worldTransform_.matWorld_, worldTransform_.scale_);

	worldTransform_.matWorld_ =
	  RotationZMatrix4(worldTransform_.matWorld_, worldTransform_.rotation_);

	worldTransform_.matWorld_ =
	  RotationYMatrix4(worldTransform_.matWorld_, worldTransform_.rotation_);

	worldTransform_.matWorld_ =
	  MoveMatrix4(worldTransform_.matWorld_, worldTransform_.translation_);

	//カメラ追従
	viewProjection_.eye.x = -rotaTarget.x * 10 + worldTransform_.translation_.x;

	viewProjection_.eye.z = -rotaTarget.z * 10 + worldTransform_.translation_.z;

	viewProjection_.target = {
	  rotaTarget.x * 10 + worldTransform_.translation_.x, 0,
	  rotaTarget.z * 10 + worldTransform_.translation_.z};

	count++;
	if (count == 60) {
		rotaCount = 0;
		count = 0;
	}

	//行列の再計算
	viewProjection_.UpdateMatrix();

	worldTransform_.TransferMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "transform: %f, %f, %f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

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
	// model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	/*for (size_t i = 0; i < _countof(worldTransform_); i++) {
	    model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}*/

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
