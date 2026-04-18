#pragma once

#include "GameObject.h"
#include "Player.h"
#include <vector>

class CScene
{
public:
	CScene();
	virtual ~CScene();

	CPlayer						*m_pPlayer = NULL;

	int							m_nObjects = 0;
	std::vector<std::unique_ptr<CGameObject>>	m_ppObjects;
	std::unique_ptr<CWallsObject>				m_pWallsObject;

	virtual void BuildObjects();
	virtual void ReleaseObjects();

	void CheckObjectByObjectCollisions();
	void CheckObjectByWallCollisions();
	void CheckPlayerByWallCollision();
	void CheckObjectByBulletCollisions();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	CGameObject *PickObjectPointedByCursor(int xClient, int yClient, CCamera *pCamera);
};

