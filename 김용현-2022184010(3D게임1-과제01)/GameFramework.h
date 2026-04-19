#pragma once

//-----------------------------------------------------------------------------
// File: GameFramework.h
//-----------------------------------------------------------------------------

#include "Timer.h"
#include "Scene.h"
#include "Player.h"

enum class GAMESTATE
{
	LOADING,
	STARTMENU,
	SETTINGS,
	INGAME,
	EXIT
};

class CGameFramework
{
public:
	CGameFramework(void);
	~CGameFramework(void);

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void FrameAdvance();

	void SetActive(bool bActive) { m_bActive = bActive; }

private:
	HINSTANCE						m_hInstance = NULL;
	HWND							m_hWnd = NULL;

	bool							m_bActive = true;

	CGameTimer						m_GameTimer;

	HDC								m_hDCFrameBuffer = NULL;
	HBITMAP							m_hBitmapFrameBuffer = NULL;

	std::unique_ptr<CPlayer>		m_pPlayer;
	std::unique_ptr<CScene>			m_pScene;
	CGameObject* m_pSelectedObject = NULL;

private:
	GAMESTATE						m_eGameState = GAMESTATE::LOADING;

	float							m_Loading = 0.0f;

	int								m_StartMenuIdx = 0;   // 0: Start, 1: Setting, 2: Quit
	int								m_SettingMenuIdx = 0; // 0: Sound, 1: Fullscreen, 2: Back

	bool							m_SoundOn = true;
	bool							m_Fullscreen = false;

	RECT							m_rcWindowed = {};
	DWORD							m_dwWindowStyle = WS_OVERLAPPEDWINDOW;

	//메뉴, 로딩, 배경 비트맵 후에 추가 예정
	HBITMAP							m_hMenuBitmap = NULL;
	HBITMAP							m_hLoadingBitmap = NULL;
	HBITMAP							m_hSkyBitmap = NULL;

public:
	void BuildFrameBuffer();
	void ResizeFrameBuffer(int nWidth, int nHeight);
	void UpdateViewport(int nWidth, int nHeight);

	void ClearFrameBuffer(DWORD dwColor);
	void PresentFrameBuffer();

	void BuildObjects();
	void ReleaseObjects();
	void ProcessInput();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void ChangeState(GAMESTATE eState);
	void UpdateLoading(float fTimeElapsed);

	void RenderLoadingScreen();
	void RenderStartMenu();
	void RenderSettingsMenu();
	void RenderMenuBackground();

	void DrawCenteredText(const RECT& rc, LPCTSTR pszText, int y, int nHeight, COLORREF color, bool bBold);
	void DrawMenuItem(LPCTSTR pszText, int y, bool bSelected, COLORREF baseColor);
	void DrawSettingItem(LPCTSTR pszLeft, LPCTSTR pszRight, int y, bool bSelected);

	void ExecuteStartMenu();
	void ExecuteSettingsMenu();
	void ToggleFullscreen();

	void DrawBitmap(HBITMAP hBitmap);

	POINT							m_ptOldCursorPos;

	_TCHAR							m_pszFrameRate[50];
};