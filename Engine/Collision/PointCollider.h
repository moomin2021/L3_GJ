#pragma once
#include "BaseCollider2D.h"
#include "CollisionPrimitive.h"

class PointCollider : public BaseCollider2D, public Point
{
#pragma region �����o�ϐ�
private:

#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	PointCollider(Vector2 pos = {0.0f, 0.0f});

	// �X�V����
	void Update() override;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// ���W��ݒ�
	inline void SetPosition(const Vector2& pos) { Point::pos = pos; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// ���W���擾
	inline const Vector2& GetPosition() { return Point::pos; }
#pragma endregion
};