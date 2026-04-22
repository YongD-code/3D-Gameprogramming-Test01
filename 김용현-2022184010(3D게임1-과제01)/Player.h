#pragma once

#include "GameObject.h"
#include "Sound.h"
#include <memory>
#include <array>

enum class CAMERAMODE { FIRSTPERSON, THIRDPERSON };


class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

	XMFLOAT3					m_xmf3Position;
	XMFLOAT3					m_xmf3Right;
	XMFLOAT3					m_xmf3Up;
	XMFLOAT3					m_xmf3Look;

	XMFLOAT3					m_xmf3CameraOffset;
	XMFLOAT3					m_xmf3Velocity;
	float						m_fFriction = 125.0f;

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;

	std::unique_ptr<CCamera>	m_pCamera;

	void SetPosition(float x, float y, float z);
	void LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);

	void Move(DWORD dwDirection, float fDistance);
	void Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void SetCameraOffset(const XMFLOAT3& xmf3CameraOffset);
	void Update(float fTimeElapsed = 0.016f);

	CAMERAMODE m_eCameraMode = CAMERAMODE::THIRDPERSON;
	void ChangeCameraMode();
	void ApplyCameraMode();

	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};

#define BULLETS					30

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer();
	virtual ~CAirplanePlayer();

	float						m_fBulletEffectiveRange = 150.0f;
	std::array<std::unique_ptr<CBulletObject>,BULLETS>	m_ppBullets;
	CSound* m_pSound = NULL;

	void FireBullet(CGameObject *pSelectedObject);

	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};

