#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <cassert> 
#include <random>

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

	Matrix4 matScale;

	Matrix4 matRotY = MathUtility::Matrix4Identity();

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//乱数シード生成器
	std::random_device seed_gen;

	std::random_device seed_gen2;

	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	std::mt19937_64 engine2(seed_gen2());

	//乱数範囲
	std::uniform_real_distribution<float> dist(0, 3.14);

	std::uniform_real_distribution<float> dist2(-10, 10);

	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float value = dist(engine);

	float value2 = dist2(engine2);

	//ワールドトランスフォームの初期化
	for (WorldTransform& worldTransform : worldTransforms_) {

		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		worldTransform.scale_ = {5, 5, 5};

		worldTransform.rotation_ = {0.0f, 0.0f, 0.0f};

		worldTransform.translation_ = {0.0f,-20.0f,0.0f};

		worldTransform.translation_.x += 5.0f;

		/*for (int i=0;i<100;i++) {
			worldTransforms_[i].translation_.x += 5.0f;
		
		}*/

		// worldTransforms_[i].matWorld_ = MathUtility::Matrix4Identity();

		//行列の再計算
		worldTransform.TransferMatrix();
	}

	//for (int i = 1; i < 100;i++)
	//{
	//	//ワールドトランスフォームの初期化
	//	worldTransforms_[i].Initialize();

	//	worldTransforms_[i].scale_ = {5, 5, 5};

	//	worldTransforms_[i].rotation_ = {0.0f, 0.0f, 0.0f};

	//	worldTransforms_[i].translation_ = {0.0f, 0.0f, 0.0f};

	//	worldTransforms_[i].translation_.x = 5 * i;

	//	// worldTransforms_[i].matWorld_ = MathUtility::Matrix4Identity();

	//	//行列の再計算
	//	// worldTransform.TransferMatrix();
	//}

	////ワールドトランスフォームの初期化
	//for (WorldTransform& worldTransform:worldTransforms_)
	//{

	//	//ワールドトランスフォームの初期化
	//	worldTransform.Initialize();

	//	worldTransform.scale_ = {1, 1, 1};

	//	worldTransform.rotation_ = {0.0f, value, 0.0f};

	//	worldTransform.translation_ = {value2, value2, value2};

	//	//worldTransforms_[i].matWorld_ = MathUtility::Matrix4Identity();

	//	//行列の再計算
	//	//worldTransform.TransferMatrix();
	//}

	//視点の移動ベクトル
	moveTarget = Vector3(0, 0, 0);

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
		worldTransforms_[0].scale_.y += scaleSize;
	} else if (input_->PushKey(DIK_DOWN)) {
		worldTransforms_[0].scale_.y -= scaleSize;
	}

	//←,→でX軸拡大縮小
	if (input_->PushKey(DIK_LEFT)) {
		worldTransforms_[0].scale_.x -= scaleSize;
	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransforms_[0].scale_.x += scaleSize;
	}

	// A,Dで回転(Y軸回転)
	if (input_->PushKey(DIK_D)) {
		worldTransforms_[0].rotation_.y += rSpeed;
	} else if (input_->PushKey(DIK_A)) {
		worldTransforms_[0].rotation_.y -= rSpeed;
	}

	moveTarget = {sinf(worldTransforms_[0].rotation_.y), 0, cosf(worldTransforms_[0].rotation_.y)};

	// W,Sで移動
	if (input_->PushKey(DIK_W)) {
		worldTransforms_[0].translation_.x += moveTarget.x * kSpeed;
		worldTransforms_[0].translation_.z += moveTarget.z * kSpeed;
	} else if (input_->PushKey(DIK_S)) {
		worldTransforms_[0].translation_.x += moveTarget.x * -kSpeed;
		worldTransforms_[0].translation_.z += moveTarget.z * -kSpeed;
	}

	//for (WorldTransform& worldTransform : worldTransforms_) {
	//	worldTransform.rotation_ = {0.0f, value, 0.0f};

	//	worldTransform.translation_ = {value2, value2, value2};

	//	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	//	worldTransform.matWorld_ =
	//	  ScaleMatrix4(worldTransform.matWorld_, worldTransform.scale_);

	//	worldTransform.matWorld_ =
	//	  RotationYMatrix4(worldTransform.matWorld_, worldTransform.rotation_);

	//	worldTransform.matWorld_ =
	//	  MoveMatrix4(worldTransform.matWorld_, worldTransform.translation_);

	//	//行列の再計算
	//	worldTransform.TransferMatrix();
	//}
	
	//行列変換
	worldTransforms_[0].matWorld_ = MathUtility::Matrix4Identity();

	worldTransforms_[0].matWorld_ = ScaleMatrix4(worldTransforms_[0].matWorld_, worldTransforms_[0].scale_);

	worldTransforms_[0].matWorld_ = RotationYMatrix4(worldTransforms_[0].matWorld_, worldTransforms_[0].rotation_);

	worldTransforms_[0].matWorld_ = MoveMatrix4(worldTransforms_[0].matWorld_, worldTransforms_[0].translation_);

	//行列の再計算
	worldTransforms_[0].TransferMatrix();

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
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, debugCamera_->GetViewProjection(), textureHandle_);
	
	}

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