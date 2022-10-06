#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include<memory>
#include<list>

/// <summary>
/// 自キャラ
/// </summary>
class Player {
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void UpDate(ViewProjection viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

  private:
	  //ワールド変換データ
	  WorldTransform worldTransform_;

	  //テクスチャハンドル
	  uint32_t textureHandle_ = 0;

	  //モデル
	  Model* model_ = nullptr;

	  Input* input_ = nullptr;

	  DebugText* debugText_ = nullptr;

	  float rotaCount;

	  float rotaPlayer;
	  float count;

	  float rotaTime;

	  Vector3 moveTarget;
	  Vector3 rotaTarget;

	  Vector3 playerPos;

	  Player* player_ = nullptr;

	  //弾
	  std::list<std::unique_ptr<PlayerBullet>> bullets_;
};