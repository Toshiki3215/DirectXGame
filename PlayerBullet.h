#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet 
{
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initilize(Model* model, const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(WorldTransform worldTransform);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	Vector3 BulletPos() const { return worldTransformBullet_.translation_; }

	Vector3 GetWorldPos();

	void OnCollision();

private:
	//ワールド変換データ
	WorldTransform worldTransformBullet_;

	Vector3 worldTransformBulletPos_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//モデル
	Model* model_ = nullptr;

	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

};
