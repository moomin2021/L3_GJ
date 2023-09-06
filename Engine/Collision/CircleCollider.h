#pragma once
#include "BaseCollider2D.h"
#include "CollisionPrimitive.h"

class CircleCollider : public BaseCollider2D, public Circle
{
#pragma region �����o�ϐ�
private:
	// �I�t�Z�b�g
	Vector2 offset_ = { 0.0f, 0.0f };
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	CircleCollider(Vector2 offset = { 0.0f, 0.0f }, float radius = 1.0f);

	// �X�V����
	void Update() override;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// �I�t�Z�b�g��ݒ�
	inline void SetOffset(const Vector2& offset) { offset_ = offset; }

	// ���a��ݒ�
	inline void SetRadius(float radius) { Circle::radius = radius; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// �I�t�Z�b�g���擾
	inline const Vector2& GetOffset() { return offset_; }

	// ���a���擾
	inline float GetRadius() { return Circle::radius; }
#pragma endregion
};