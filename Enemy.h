#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"

/// <summary>
/// �G
/// </summary>
class Enemy {
  public:

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initilize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Vector3 velocity);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	Vector3 EnemyPos() const { return worldTransformEnemy_.translation_; }

	Vector3 GetWorldPos();

	void OnCollision();

 private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransformEnemy_;

	Vector3 worldTransformEnemyPos_;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	uint32_t textureHandle3_ = 0;

	//���f��
	Model* model_ = nullptr;

	//���x
	Vector3 velocity_;

	Vector3 velocity2_;
	
	Vector3 velocity3_;

	enum class Phase {
		Approch,
		Leave,
		Death,
	};

	//�t�F�[�Y
	Phase phase_ = Phase::Approch;
	//Phase phase_ = Phase::Death;

};

