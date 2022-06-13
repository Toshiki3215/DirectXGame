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

	//wT.rotation_ = {0.0f, 0.0f, 0.0f};


	worldTransform_.rotation_ = {0.0f, 3.14/36, 0.0f};

	Matrix4 matRotY = MathUtility::Matrix4Identity();

	/*matRotY.m[0][0] = cosf(wT.rotation_.y);
	matRotY.m[0][2] = sinf(wT.rotation_.y);
	matRotY.m[2][0] = sinf(wT.rotation_.y);
	matRotY.m[2][2] = -cosf(wT.rotation_.y);*/

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	worldTransform_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280,720);

	viewProjection_ = debugCamera_->GetViewProjection();

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//Z軸回転行列を宣言
	Matrix4 matRotZ;

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
	Vector3 move = Vector3();

	Vector3 moveO = Vector3();

	/*Vector3 frontV = Vector3
	  (wT.rotation_.x - viewProjection_.eye.x, wT.rotation_.y - viewProjection_.eye.y,
	   wT.rotation_.z - viewProjection_.eye.z);*/

	Vector3 frontV = Vector3(
	  worldTransform_.rotation_.x - viewProjection_.eye.x,
	  worldTransform_.rotation_.y - viewProjection_.eye.y,
	  worldTransform_.rotation_.z - viewProjection_.eye.z);

	//Vector3 frontRot = (sinf(worldTransform_.rotation_.y))

	//視点の移動速さ
	const float kEyeSpeed = 0.2f;

	const float kSpeed = 0.2f;
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	Matrix4 matRotY = MathUtility::Matrix4Identity();

	int mode = 1;

	if (mode == 1) {

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_UP)) {
			move.z += kEyeSpeed;
		} else if (input_->PushKey(DIK_DOWN)) {
			move.z -= kEyeSpeed;
		}

		//←、→で回転
		if (input_->PushKey(DIK_RIGHT)) {
			matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
			matRotY.m[0][2] = -sinf(worldTransform_.rotation_.y);
			matRotY.m[2][0] = sinf(worldTransform_.rotation_.y);
			matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);
		} else if (input_->PushKey(DIK_LEFT)) {
			matRotY.m[0][0] = cosf(-worldTransform_.rotation_.y);
			matRotY.m[0][2] = -sinf(-worldTransform_.rotation_.y);
			matRotY.m[2][0] = sinf(-worldTransform_.rotation_.y);
			matRotY.m[2][2] = cosf(-worldTransform_.rotation_.y);
		}

		/*if (input_->PushKey(DIK_RIGHT)) {
			matRotY.m[0][0] = cosf(wT.rotation_.y);
			matRotY.m[0][2] = -sinf(wT.rotation_.y);
			matRotY.m[2][0] = sinf(wT.rotation_.y);
			matRotY.m[2][2] = cosf(wT.rotation_.y);
		} else if (input_->PushKey(DIK_LEFT)) {
			matRotY.m[0][0] = cosf(-wT.rotation_.y);
			matRotY.m[0][2] = -sinf(-wT.rotation_.y);
			matRotY.m[2][0] = sinf(-wT.rotation_.y);
			matRotY.m[2][2] = cosf(-wT.rotation_.y);
		}*/

		if (input_->PushKey(DIK_W)) {
			matTrans.m[3][2] = 0.2f;
			//matTrans.m[3][2] *= frontV.z;
		} else if (input_->PushKey(DIK_S)) {
			matTrans.m[3][2] = -0.2f;
			//matTrans.m[3][2] *= frontV.z;
		}

		if (input_->PushKey(DIK_A)) {
			matTrans.m[3][0] = -0.2f;
		} else if (input_->PushKey(DIK_D)) {
			matTrans.m[3][0] = 0.2f;
			;
		}

		if (input_->PushKey(DIK_E)) {
		
			mode = 0;
		}

	} else if (mode == 0) {

		if (input_->PushKey(DIK_W)) {
			matTrans.m[3][2] = 0.2f;
		} else if (input_->PushKey(DIK_S)) {
			matTrans.m[3][2] = -0.2f;
		}

		if (input_->PushKey(DIK_A)) {
			matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
			matRotY.m[0][2] = -sinf(worldTransform_.rotation_.y);
			matRotY.m[2][0] = sinf(worldTransform_.rotation_.y);
			matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);
		} else if (input_->PushKey(DIK_D)) {
			matRotY.m[0][0] = cosf(-worldTransform_.rotation_.y);
			matRotY.m[0][2] = -sinf(-worldTransform_.rotation_.y);
			matRotY.m[2][0] = sinf(-worldTransform_.rotation_.y);
			matRotY.m[2][2] = cosf(-worldTransform_.rotation_.y);
		}

		if (input_->PushKey(DIK_E)) {

			mode = 1;
		}

	}


	//視点移動(ベクトルの加算)
	viewProjection_.eye += move;

	//回転
	worldTransform_.matWorld_ *= matRotY;

	viewProjection_.eye.x =
	  (worldTransform_.matWorld_.m[0][0] * cosf(worldTransform_.rotation_.y) +
	   worldTransform_.matWorld_.m[0][2] * sinf(worldTransform_.rotation_.y)) *
	  5;

	viewProjection_.eye.z =
	  (worldTransform_.matWorld_.m[0][0] * -sinf(worldTransform_.rotation_.y) +
	   worldTransform_.matWorld_.m[0][2] * cosf(worldTransform_.rotation_.y)) *
	  5;


	//viewProjection_.eye.z = -cosf(worldTransform_.rotation_.y) * 5;

	//移動
	//matTrans *= frontV;

	worldTransform_.matWorld_ *= matTrans;



	//行列の再計算
	viewProjection_.UpdateMatrix();

	worldTransform_.TransferMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:( %f, %f, %f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	/*debugText_->Printf(
	  "mode:( %f)", mode);*/

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
	//model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
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
