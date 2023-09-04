#pragma once
#include "CollisionPrimitive.h"

class Collision
{
public:
	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphere0"> ��0 </param>
	/// <param name="sphere1"> ��1 </param>
	/// <param name="inter">  </param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphere0, const Sphere& sphere1);

	/// <summary>
	/// ���ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="sphere"> �� </param>
	/// <param name="plane"> ���� </param>
	/// <param name="inter"> ��_(���ʏ�̍ŋߐړ_) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point"> �_ </param>
	/// <param name="traiangle"> �O�p�` </param>
	/// <param name="closest"> �ŋߐړ_(�o�͗p) </param>
	/// <returns>  </returns>
	static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓����蔻��`�F�b�N
	/// </summary>
	/// <param name="sphere"> �� </param>
	/// <param name="triangle"> �O�p�` </param>
	/// <param name="inter"> ��_(�O�p�`��̍ŋߐړ_) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr, Vector3* reject = nullptr);

	/// <summary>
	/// ���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="ray"> ���C </param>
	/// <param name="plane"> ���� </param>
	/// <param name="distance"> ����(�o�͗p) </param>
	/// <param name="inter"> ��_(�o�͗p) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="ray"> ���C </param>
	/// <param name="triangle"> �O�p�` </param>
	/// <param name="distance"> ����(�o�͗p) </param>
	/// <param name="inter"> ��_(�o�͗p) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// ���C�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="ray"> ���C </param>
	/// <param name="sphere"> �� </param>
	/// <param name="distance"> ����(�o�͗p) </param>
	/// <param name="inter"> ��_(�o�͗p) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// �~�Ɖ~�̓����蔻��
	/// </summary>
	/// <param name="circle0"> �~0 </param>
	/// <param name="circle1"> �~1 </param>
	/// <returns></returns>
	static bool CheckCircle2Circle(const Circle& circle0, const Circle& circle1);

	/// <summary>
	/// �l�p�`�Ǝl�p�`�̓����蔻��
	/// </summary>
	/// <param name="box0"> �l�p�`0 </param>
	/// <param name="box1"> �l�p�`1 </param>
	/// <returns></returns>
	static bool CheckBox2Box(const Box& box0, const Box& box1);

	/// <summary>
	/// �~�Ǝl�p�`�̓����蔻��
	/// </summary>
	/// <param name="circle"> �~ </param>
	/// <param name="box"> �l�p�` </param>
	/// <returns></returns>
	static bool CheckCircle2Box(const Circle& circle, const Box& box);

	/// <summary>
	/// �_�Ǝl�p�`�̓����蔻��
	/// </summary>
	/// <param name="point"> �_ </param>
	/// <param name="box"> �l�p�` </param>
	/// <returns></returns>
	static bool CheckPoint2Box(const Point& point, const Box& box);

	/// <summary>
	/// �_�Ɖ~�̓����蔻��
	/// </summary>
	/// <param name="point"> �_ </param>
	/// <param name="circle"> �~ </param>
	/// <returns></returns>
	static bool CheckPoint2Circle(const Point& point, const Circle& circle);
};