#pragma once
#include "BaseCollider2D.h"
#include "CollisionPrimitive.h"

class PointCollider : public BaseCollider2D, public Point
{
#pragma region �����o�ϐ�
private:
	// �I�t�Z�b�g
	Vector2 offset_ = { 0.0f, 0.0f };
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	PointCollider(const Vector2& offset);

	// �X�V����
	void Update() override;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// �I�t�Z�b�g��ݒ�
	inline void SetOffset(const Vector2& offset) { offset_ = offset; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// �I�t�Z�b�g���擾
	inline const Vector2& GetOffset() { return offset_; }
#pragma endregion
};