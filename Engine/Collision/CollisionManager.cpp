#include "CollisionManager.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "RayCollider.h"
#include "MeshCollider.h"

CollisionManager::~CollisionManager()
{
	// �R���C�_�[�폜
	colliders_.clear();
}

void CollisionManager::CheckAllCollision()
{
	// �R���C�_�[�X�V
	for (auto& it : colliders_) {
		it->Update();
	}

	// �������肷�邽�߂ɗp��
	std::forward_list<BaseCollider*>::iterator itA = colliders_.begin();;
	std::forward_list<BaseCollider*>::iterator itB;

	// ��������`�F�b�N
	for (; itA != colliders_.end(); ++itA) {
		itB = itA;
		++itB;

		// ���C��������
		if ((*itA)->GetShapeType() == SHAPE_RAY) {
			// �Փ˃t���O
			bool result = false;

			// �ł��߂��R���C�_�[�̏��
			float distance = FLT_MAX;				// �ł��ŏ��̋���
			Vector3 inter = { 0.0f, 0.0f, 0.0f };	// �ł��߂������ɂ���R���C�_�[�Ƃ̌�_
			BaseCollider* it_hit = nullptr;			// �ł��߂��R���C�_�[

			// �����p�̃C�e���[�^�[
			std::forward_list<BaseCollider*>::iterator it = colliders_.begin();

			// �Փ˔���̈����̂��߂Ƀ��C�ɕϊ�
			Ray* ray = dynamic_cast<Ray*>(*itA);

			// ��������`�F�b�N
			for (; it != colliders_.end(); ++it) {
				// ����������Ȃ���΃X�L�b�v
				if (!((*itA)->attribute_ & (*it)->attribute_)) continue;

				// ���̏ꍇ
				if ((*it)->GetShapeType() == SHAPE_SPHERE) {
					// �Փ˔���̈����̂��߂ɋ��ɕϊ�
					Sphere* sphere = dynamic_cast<Sphere*>(*it);

					// ���肵���Ƃ��̃f�[�^
					float tempDistance = 0.0f;
					Vector3 tempInter = { 0.0f, 0.0f, 0.0f };

					// �Փ˂��Ă��Ȃ������珜�O
					if (!Collision::CheckRay2Sphere(*ray, *sphere, &tempDistance, &tempInter)) continue;

					// �������ŏ��łȂ���Ώ��O
					if (tempDistance >= distance) continue;

					// �ł��߂��R���C�_�[�Ȃ̂ŏ���ۑ�
					result = true;
					distance = tempDistance;
					inter = tempInter;
					it_hit = *it;
				}

				else if ((*it)->GetShapeType() == SHAPE_MESH) {
					// �Փ˔���̈����̂��߂Ƀ��b�V���R���C�_�[�ɕϊ�
					MeshCollider* meshCol = dynamic_cast<MeshCollider*>(*it);

					// ���肵���Ƃ��̃f�[�^
					float tempDistance = 0.0f;
					Vector3 tempInter = { 0.0f, 0.0f, 0.0f };

					// �Փ˂��Ă��Ȃ������珜�O
					if (!meshCol->CheckCollisionRay(*ray, &tempDistance, &tempInter)) continue;

					// �������ŏ��łȂ���Ώ��O
					if (tempDistance >= distance) continue;

					// �ł��߂��R���C�_�[�Ȃ̂ŏ���ۑ�
					result = true;
					distance = tempDistance;
					inter = tempInter;
					it_hit = *it;
				}
			}

			// �Փ˂��Ă�����
			if (result) {
				// ���C
				RayCollider* rayCol = dynamic_cast<RayCollider*>(*itA);
				rayCol->SetIsHit(true);
				rayCol->SetInter(inter);
				rayCol->SetDistance(distance);
				rayCol->SetHitCollider(it_hit);

				// ��
				if (it_hit->GetShapeType() == SHAPE_SPHERE) {
					SphereCollider* sphereCol = dynamic_cast<SphereCollider*>(it_hit);
					sphereCol->SetIsHit(true);
					sphereCol->SetInter(inter);
					sphereCol->SetHitCollider(*itA);
				}

				// ���b�V��
				else if (it_hit->GetShapeType() == SHAPE_MESH) {
					MeshCollider* meshCol = dynamic_cast<MeshCollider*>(it_hit);
					meshCol->SetIsHit(true);
					meshCol->SetInter(inter);
					meshCol->SetHitCollider(*itA);
				}
			}
		}

		// ���C�ȊO��������
		else {
			for (; itB != colliders_.end(); ++itB) {
				// ����������Ȃ���Ώ��O
				if (!((*itA)->attribute_ & (*itB)->attribute_)) continue;

				// �Ƃ��ɋ�
				if ((*itA)->GetShapeType() == SHAPE_SPHERE && (*itB)->GetShapeType() == SHAPE_SPHERE) {
					// �Փ˔���̈����̂��߂ɋ��ɕϊ�
					Sphere* sphereA = dynamic_cast<Sphere*>(*itA);
					Sphere* sphereB = dynamic_cast<Sphere*>(*itB);

					// �Փ˔���
					if (Collision::CheckSphere2Sphere(*sphereA, *sphereB)) {
						SphereCollider* sphereColA = dynamic_cast<SphereCollider*>(*itA);
						sphereColA->SetIsHit(true);
						sphereColA->SetHitCollider(*itB);

						SphereCollider* sphereColB = dynamic_cast<SphereCollider*>(*itB);
						sphereColB->SetIsHit(true);
						sphereColB->SetHitCollider(*itA);
					}
				}

				// ���b�V���Ƌ�
				else if ((*itA)->GetShapeType() == SHAPE_MESH && (*itB)->GetShapeType() == SHAPE_SPHERE) {
					// �Փ˔���̈����̂��߂Ƀ��b�V���R���C�_�[�Ƌ��ɕϊ�
					MeshCollider* meshCol = dynamic_cast<MeshCollider*>(*itA);
					Sphere* sphere = dynamic_cast<Sphere*>(*itB);

					// ���肵���Ƃ��̃f�[�^
					Vector3 inter = { 0.0f, 0.0f, 0.0f };
					Vector3 reject = { 0.0f, 0.0f, 0.0f };

					if (meshCol->CheckCollisionSphere(*sphere, &inter, &reject)) {
						meshCol->SetIsHit(true);
						meshCol->SetInter(inter);
						meshCol->AddReject(-reject);
						meshCol->SetHitCollider(*itB);

						// �Փ˔���̈����̂��߂ɋ��R���C�_�[�ɕϊ�
						SphereCollider* sphereCol = dynamic_cast<SphereCollider*>(*itB);
						sphereCol->SetIsHit(true);
						sphereCol->SetInter(inter);
						sphereCol->AddReject(reject);
						sphereCol->SetHitCollider(*itA);
					}
				}

				// ���ƃ��b�V��
				else if ((*itA)->GetShapeType() == SHAPE_SPHERE && (*itB)->GetShapeType() == SHAPE_MESH) {
					// �Փ˔���̈����̂��߂Ƀ��b�V���R���C�_�[�Ƌ��ɕϊ�
					Sphere* sphere = dynamic_cast<Sphere*>(*itA);
					MeshCollider* meshCol = dynamic_cast<MeshCollider*>(*itB);

					// ���肵���Ƃ��̃f�[�^
					Vector3 inter = { 0.0f, 0.0f, 0.0f };
					Vector3 reject = { 0.0f, 0.0f, 0.0f };

					if (meshCol->CheckCollisionSphere(*sphere, &inter)) {

						// �Փ˔���̈����̂��߂ɋ��R���C�_�[�ɕϊ�
						SphereCollider* sphereCol = dynamic_cast<SphereCollider*>(*itA);
						sphereCol->SetIsHit(true);
						sphereCol->SetInter(inter);
						sphereCol->AddReject(reject);
						sphereCol->SetHitCollider(*itB);

						meshCol->SetIsHit(true);
						meshCol->SetInter(inter);
						meshCol->AddReject(-reject);
						meshCol->SetHitCollider(*itA);
					}
				}
			}
		}
	}
}

CollisionManager* CollisionManager::GetInstance()
{
	// �C���X�^���X����
	static CollisionManager inst;

	// �C���X�^���X��Ԃ�
	return &inst;
}