#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"

/// <summary>
/// 敵
/// </summary>
class Enemy {
  public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initilize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Vector3 velocity);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	Vector3 EnemyPos() const { return worldTransformEnemy_.translation_; }

	Vector3 GetWorldPos();

	void OnCollision();

 private:
	//ワールド変換データ
	WorldTransform worldTransformEnemy_;

	Vector3 worldTransformEnemyPos_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	uint32_t textureHandle3_ = 0;

	//モデル
	Model* model_ = nullptr;

	//速度
	Vector3 velocity_;

	Vector3 velocity2_;
	
	Vector3 velocity3_;

	enum class Phase {
		Approch,
		Leave,
		Death,
	};

	//フェーズ
	Phase phase_ = Phase::Approch;
	//Phase phase_ = Phase::Death;

};

