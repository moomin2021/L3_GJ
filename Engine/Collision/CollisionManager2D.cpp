#include "CollisionManager2D.h"
#include "Collision.h"

CollisionManager2D::~CollisionManager2D()
{
	// �R���C�_�[�폜
	colliders_.clear();
}

void CollisionManager2D::CheckAllCollision()
{
	// �R���C�_�[�X�V
	for (auto& it : colliders_) {
		it->Update();
	}

	// �������肷�邽�߂ɗp��
	std::forward_list<BaseCollider2D*>::iterator itA = colliders_.begin();;
	std::forward_list<BaseCollider2D*>::iterator itB;

	// ��������`�F�b�N
	for (; itA != colliders_.end(); ++itA) {
		itB = itA;
		++itB;

		for (; itB != colliders_.end(); ++itB) {
			// ����������Ȃ���Ώ��O
			if (!((*itA)->attribute_ & (*itB)->attribute_)) continue;

			// �Ƃ��ɋ�`
			if ((*itA)->GetShapeType() == SHAPE_BOX && (*itB)->GetShapeType() == SHAPE_BOX) {
				// �Փ˔���̈����̂��߂ɋ��ɕϊ�
				Box* boxA = dynamic_cast<Box*>(*itA);
				Box* boxB = dynamic_cast<Box*>(*itB);

				// �Փ˔���
				if (Collision::CheckBox2Box(*boxA, *boxB)) {
					(*itA)->SetIsHit(true);
					(*itA)->SetHitCollider(*itB);

					(*itB)->SetIsHit(true);
					(*itB)->SetHitCollider(*itA);
				}
			}

			// �Ƃ��ɉ~
			else if ((*itA)->GetShapeType() == SHAPE_CIRCLE && (*itB)->GetShapeType() == SHAPE_CIRCLE) {
				// �Փ˔���̈����̂��߂ɋ��ɕϊ�
				Circle* cirA = dynamic_cast<Circle*>(*itA);
				Circle* cirB = dynamic_cast<Circle*>(*itB);

				// �Փ˔���
				if (Collision::CheckCircle2Circle(*cirA, *cirB)) {
					(*itA)->SetIsHit(true);
					(*itA)->SetHitCollider(*itB);

					(*itB)->SetIsHit(true);
					(*itB)->SetHitCollider(*itA);
				}
			}

			// �_�Ƌ�`
			else if ((*itA)->GetShapeType() == SHAPE_POINT && (*itB)->GetShapeType() == SHAPE_BOX) {
				// �Փ˔���̈����̂��߂ɋ��ɕϊ�
				Point* point = dynamic_cast<Point*>(*itA);
				Box* box = dynamic_cast<Box*>(*itB);

				// �Փ˔���
				if (Collision::CheckPoint2Box(*point, *box)) {
					(*itA)->SetIsHit(true);
					(*itA)->SetHitCollider(*itB);

					(*itB)->SetIsHit(true);
					(*itB)->SetHitCollider(*itA);
				}
			}

			// ��`�Ɠ_
			else if ((*itA)->GetShapeType() == SHAPE_BOX && (*itB)->GetShapeType() == SHAPE_POINT) {
				// �Փ˔���̈����̂��߂ɋ��ɕϊ�
				Box* box = dynamic_cast<Box*>(*itA);
				Point* point = dynamic_cast<Point*>(*itB);

				// �Փ˔���
				if (Collision::CheckPoint2Box(*point, *box)) {
					(*itA)->SetIsHit(true);
					(*itA)->SetHitCollider(*itB);

					(*itB)->SetIsHit(true);
					(*itB)->SetHitCollider(*itA);
				}
			}

			// �_�Ɖ~
			else if ((*itA)->GetShapeType() == SHAPE_POINT && (*itB)->GetShapeType() == SHAPE_CIRCLE) {
				// �Փ˔���̈����̂��߂ɋ��ɕϊ�
				Point* point = dynamic_cast<Point*>(*itA);
				Circle* cir = dynamic_cast<Circle*>(*itB);

				// �Փ˔���
				if (Collision::CheckPoint2Circle(*point, *cir)) {
					(*itA)->SetIsHit(true);
					(*itA)->SetHitCollider(*itB);

					(*itB)->SetIsHit(true);
					(*itB)->SetHitCollider(*itA);
				}
			}

			// �~�Ɠ_
			else if ((*itA)->GetShapeType() == SHAPE_CIRCLE && (*itB)->GetShapeType() == SHAPE_POINT) {
				// �Փ˔���̈����̂��߂ɋ��ɕϊ�
				Circle* cir = dynamic_cast<Circle*>(*itA);
				Point* point = dynamic_cast<Point*>(*itB);

				// �Փ˔���
				if (Collision::CheckPoint2Circle(*point, *cir)) {
					(*itA)->SetIsHit(true);
					(*itA)->SetHitCollider(*itB);

					(*itB)->SetIsHit(true);
					(*itB)->SetHitCollider(*itA);
				}
			}

			// ��`�Ɖ~
			else if ((*itA)->GetShapeType() == SHAPE_BOX && (*itB)->GetShapeType() == SHAPE_CIRCLE) {
				// �Փ˔���̈����̂��߂ɋ��ɕϊ�
				Box* box = dynamic_cast<Box*>(*itA);
				Circle* cir = dynamic_cast<Circle*>(*itB);

				// �Փ˔���
				if (Collision::CheckCircle2Box(*cir, *box)) {
					(*itA)->SetIsHit(true);
					(*itA)->SetHitCollider(*itB);

					(*itB)->SetIsHit(true);
					(*itB)->SetHitCollider(*itA);
				}
			}

			// �~�Ƌ�`
			else if ((*itA)->GetShapeType() == SHAPE_CIRCLE && (*itB)->GetShapeType() == SHAPE_BOX) {
				// �Փ˔���̈����̂��߂ɋ��ɕϊ�
				Circle* cir = dynamic_cast<Circle*>(*itA);
				Box* box = dynamic_cast<Box*>(*itB);

				// �Փ˔���
				if (Collision::CheckCircle2Box(*cir, *box)) {
					(*itA)->SetIsHit(true);
					(*itA)->SetHitCollider(*itB);

					(*itB)->SetIsHit(true);
					(*itB)->SetHitCollider(*itA);
				}
				}
		}
	}
}

CollisionManager2D* CollisionManager2D::GetInstance()
{
	// �C���X�^���X����
	static CollisionManager2D inst;

	// �C���X�^���X��Ԃ�
	return &inst;
}