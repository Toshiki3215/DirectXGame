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

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	mode = 0;

	//ワールドトランスフォームの初期化
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	Matrix4 matRotY = MathUtility::Matrix4Identity();

	Matrix4 matTrans = MathUtility::Matrix4Identity();

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

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	//視点の移動ベクトル
	Vector3 moveTarget = Vector3(0,0,0);

	float rSpeed = 3.14 / 36;
	const float kSpeed = 0.1f;

	//視点の移動速さ
	float EyeSpeed = 0.0f;
	float sEyeSpeed = 0.0f;

	float fSpeed = 0.02f;
	float sSpeed = 0.02f;
	float speed = 0.02f;

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	Matrix4 matRotY = MathUtility::Matrix4Identity();

	if (mode == 0) {

		if (input_->PushKey(DIK_RIGHT)) {
			sEyeSpeed = 0.2f;
		} else if (input_->PushKey(DIK_LEFT)) {
			sEyeSpeed = -0.2f;
		}

		if (input_->PushKey(DIK_UP)) {
			EyeSpeed = 0.2f;
		} else if (input_->PushKey(DIK_DOWN)) {
			EyeSpeed = -0.2f;
		}

		Vector3 frontV = Vector3(
		  viewProjection_.target.x - viewProjection_.eye.x,
		  viewProjection_.target.y - viewProjection_.eye.y,
		  viewProjection_.target.z - viewProjection_.eye.z);

		Vector3 sideV = Vector3(
		  1 * frontV.z - 0 * frontV.y, 0 * frontV.x - 0 * frontV.z, 0 * frontV.y - 1 * frontV.x);

		if (input_->PushKey(DIK_W)) {
			sSpeed *= frontV.x;
			speed *= frontV.y;
			fSpeed *= frontV.z;
			matTrans.m[3][0] = sSpeed;
			matTrans.m[3][1] = speed;
			matTrans.m[3][2] = fSpeed;
		} else if (input_->PushKey(DIK_S)) {
			sSpeed *= frontV.x;
			speed *= frontV.y;
			fSpeed *= frontV.z;
			matTrans.m[3][0] = -sSpeed;
			matTrans.m[3][1] = -speed;
			matTrans.m[3][2] = -fSpeed;
		}

		if (input_->PushKey(DIK_A)) {
			sSpeed *= sideV.x;
			fSpeed *= sideV.z;
			matTrans.m[3][0] = -sSpeed;
			matTrans.m[3][2] = -fSpeed;
		} else if (input_->PushKey(DIK_D)) {
			sSpeed *= sideV.x;
			fSpeed *= sideV.z;
			matTrans.m[3][0] = sSpeed;
			matTrans.m[3][2] = fSpeed;
		}

		if (input_->PushKey(DIK_E)) {
			mode = 1;
		}

	}

	if (mode == 1) {

		//←、→で回転
		if (input_->PushKey(DIK_D)) {
			matRotY.m[0][0] = cosf(rSpeed);
			matRotY.m[0][2] = -sinf(rSpeed);
			matRotY.m[2][0] = sinf(rSpeed);
			matRotY.m[2][2] = cosf(rSpeed);
		} else if (input_->PushKey(DIK_A)) {
			matRotY.m[0][0] = cosf(-rSpeed);
			matRotY.m[0][2] = -sinf(-rSpeed);
			matRotY.m[2][0] = sinf(-rSpeed);
			matRotY.m[2][2] = cosf(-rSpeed);
		}

		moveTarget = {sinf(worldTransform_.rotation_.y), 0, cosf(worldTransform_.rotation_.y)};

		float targetX = sinf(worldTransform_.rotation_.y);

		if (input_->PushKey(DIK_W)) {
			worldTransform_.translation_.x += moveTarget.x * kSpeed;
			worldTransform_.translation_.z += moveTarget.z * kSpeed;
		} else if (input_->PushKey(DIK_S)) {
			worldTransform_.translation_.x += moveTarget.x * -kSpeed;
			worldTransform_.translation_.z += moveTarget.z * -kSpeed;
		}

		Matrix4 Matrix4RotationX();

		if (input_->PushKey(DIK_E)) {
			mode = 0;
		}

	}

	//視点移動(ベクトルの加算)
	viewProjection_.target.x += sEyeSpeed;
	viewProjection_.target.y += EyeSpeed;

	//移動
	worldTransform_.matWorld_ *= matTrans;
	worldTransform_.matWorld_ *= matRotY;

	//行列の再計算
	viewProjection_.UpdateMatrix();

	worldTransform_.TransferMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	//debugText_->Printf("mode: %f ", mode);

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

//typedef struct 