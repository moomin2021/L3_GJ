#include "Camera.h"
#include "Util.h"
#include "WinAPI.h"

#include <DirectXMath.h>

using namespace DirectX;

float Camera::shakeTime_ = 0.0f;
float Camera::shakePower_ = 0.0f;
bool Camera::isShake_ = false;
uint64_t Camera::startShake_ = 0;

Camera::Camera()
{
	// �E�B���h�E�T�C�Y�擾
	float winWidth = static_cast<float>(WinAPI::GetInstance()->GetWidth());	// ����
	float winHeight = static_cast<float>(WinAPI::GetInstance()->GetHeight());	// �c��

	// �������e�s��̌v�Z
	XMMATRIX mat = XMMatrixPerspectiveFovLH(
		Util::Degree2Radian(45.0f),	// �㉺��p45�x
		winWidth / winHeight,		// �A�X�y�N�g��i��ʉ���/��ʏc���j
		0.1f, 1000.0f);				// �O�[�A���[

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			matPerspectivePro_.m[i][j] = mat.r[i].m128_f32[j];
		}
	}

#pragma region ���s���e�v�Z
	// �P�ʍs�����
	matOrthoGraphicPro_ = Matrix4Identity();

	// ��ʃT�C�Y�ɍ��킹�Čv�Z
	matOrthoGraphicPro_.m[0][0] = 2.0f / winWidth;
	matOrthoGraphicPro_.m[1][1] = -2.0f / winHeight;

	// ��ʔ����ړ������ĉ�ʍ�������_�ɂ���
	matOrthoGraphicPro_.m[3][0] = -1.0f;
	matOrthoGraphicPro_.m[3][1] = 1.0f;
#pragma endregion
}

void Camera::Update() {
	if (isShake_) {
		float rndX = Util::GetRandomFloat(0.0f, 1.0f) * 2 - 1;
		float rndY = Util::GetRandomFloat(0.0f, 1.0f) * 2 - 1;

		// �s���J�n����̌o�ߎ���
		float elapsedTime = (Util::GetTimrMSec() - startShake_) / 1000.0f;

		float rate = Util::Clamp(elapsedTime / shakeTime_, 1.0f, 0.0f);

		Vector3 result = { (shakePower_ * (1.0f - rate)) * rndX, (shakePower_ * (1.0f - rate)) * rndY, 0.0f };
		eye_ = result;

		if (elapsedTime >= shakeTime_) {
			eye_ = { 0.0f, 0.0f, 0.0f };
			isShake_ = false;
		}
	}

	// �r���[�s��X�V
	UpdateMatView();
}

void Camera::SetShake(float time, float power)
{
	shakeTime_ = time;
	shakePower_ = power;
	isShake_ = true;
	startShake_ = Util::GetTimrMSec();
}

void Camera::UpdateMatView()
{
	// Z�������߂�
	Vector3 zAxis = target_ - eye_;
	zAxis.normalize();// ���K��

	// X�������߂�
	Vector3 xAxis = up_.cross(zAxis);
	xAxis.normalize();// ���K��

	// Y�������߂�
	Vector3 yAxis = zAxis.cross(xAxis);
	yAxis.normalize();// ���K��

	// ��]�s������߂�
	Matrix4 matRot = {
		{
			{xAxis.x, xAxis.y, xAxis.z, 0.0f},
			{yAxis.x, yAxis.y, yAxis.z, 0.0f},
			{zAxis.x, zAxis.y, zAxis.z, 0.0f},
			{   0.0f,    0.0f,    0.0f, 1.0f}
		}
	};

	// �t�s��ɂ���
	matRot = Matrix4Inverse(matRot);

	// ���s�ړ����������߂�
	Vector3 parallelMove = {};

	// �t�J�������W
	Vector3 reverseEye = -eye_;

	// �v�Z
	parallelMove = {
		reverseEye.dot(xAxis),
		reverseEye.dot(yAxis),
		reverseEye.dot(zAxis)
	};

	// �r���[�s�񊮐�
	matView_ = {
		{
			{matRot.m[0][0], matRot.m[0][1], matRot.m[0][2], 0.0f},
			{matRot.m[1][0], matRot.m[1][1], matRot.m[1][2], 0.0f},
			{matRot.m[2][0], matRot.m[2][1], matRot.m[2][2], 0.0f},
			{parallelMove.x, parallelMove.y, parallelMove.z, 1.0f}
		}
	};
}