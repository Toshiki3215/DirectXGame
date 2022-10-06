#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Skydome {
  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransformDome_;

	//���f��
	Model* model_ = nullptr;

};