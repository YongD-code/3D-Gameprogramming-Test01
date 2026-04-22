#include "stdafx.h"
#include "GameFramework.h"
#include "Resource.h"
#include "resource1.h"

CGameFramework::CGameFramework()
{
	_tcscpy_s(m_pszFrameRate, _T("김용현-2022184010(3D게임1-과제01) ("));
}

CGameFramework::~CGameFramework()
{
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	::srand(timeGetTime());

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	m_dwWindowStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	::GetWindowRect(m_hWnd, &m_rcWindowed);

	BuildFrameBuffer();
	BuildObjects();

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	UpdateViewport(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

	ChangeState(GAMESTATE::LOADING);

	m_hLoadingBitmap = (HBITMAP)::LoadImage(m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1),IMAGE_BITMAP,0, 0,LR_CREATEDIBSECTION);
	m_hMenuBitmap = (HBITMAP)::LoadImage(m_hInstance, MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	m_hSkyBitmap = (HBITMAP)::LoadImage(m_hInstance, MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	m_Sound.SetSoundOn(true);

	return(true);
}

void CGameFramework::BuildFrameBuffer()
{
	HDC hDC = ::GetDC(m_hWnd);

	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);

	m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, (rcClient.right - rcClient.left) + 1, (rcClient.bottom - rcClient.top) + 1);
	::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	HBRUSH hBrush = ::CreateSolidBrush(dwColor);
	::FillRect(m_hDCFrameBuffer, &rcClient, hBrush);
	::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	HDC hDC = ::GetDC(m_hWnd);
	::BitBlt(hDC, 0, 0,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		m_hDCFrameBuffer, 0, 0, SRCCOPY);
	::ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::ResizeFrameBuffer(int nWidth, int nHeight)
{
	if (m_hBitmapFrameBuffer)
	{
		::DeleteObject(m_hBitmapFrameBuffer);
		m_hBitmapFrameBuffer = NULL;
	}

	HDC hDC = ::GetDC(m_hWnd);

	if (!m_hDCFrameBuffer)
	{
		m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	}

	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
	::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);

	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::UpdateViewport(int nWidth, int nHeight)
{
	if (!m_pPlayer || !m_pPlayer->m_pCamera) return;
	if (nWidth <= 0 || nHeight <= 0) return;

	m_pPlayer->m_pCamera->SetViewport(0, 0, nWidth, nHeight, 0.0f, 1.0f);
	m_pPlayer->m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, 60.0f);
}

void CGameFramework::ChangeState(GAMESTATE eState)
{
	m_eGameState = eState;

	switch (m_eGameState)
	{
	case GAMESTATE::LOADING:
		m_Loading = 0.0f;
		m_Sound.StopBGM();
		m_Sound.PlayBGM(_T("loading.mp3"));
		break;

	case GAMESTATE::STARTMENU:
		m_StartMenuIdx = 0;
		m_Sound.StopBGM();
		m_Sound.PlayBGM(_T("menu.mp3"));
		break;

	case GAMESTATE::SETTINGS:
		m_SettingMenuIdx = 0;
		break;

	case GAMESTATE::INGAME:
		m_Sound.StopBGM();
		m_Sound.PlayBGM(_T("game.mp3"));
		break;

	case GAMESTATE::FINISH:
		m_Sound.StopBGM();
		m_Sound.PlayBGM(_T("menu.wav"));
		break;

	case GAMESTATE::EXIT:
		m_Sound.StopBGM();
		::PostQuitMessage(0);
		break;
	}
}

void CGameFramework::UpdateLoading(float fTimeElapsed)
{
	m_Loading += fTimeElapsed;

	if (m_Loading >= 5.0f)
	{
		ChangeState(GAMESTATE::STARTMENU);
	}
}

void CGameFramework::DrawCenteredText(const RECT& rc, LPCTSTR pszText, int y, int nHeight, COLORREF color, bool bBold)
{
	HFONT hFont = ::CreateFont(
		nHeight, 0, 0, 0,
		bBold ? FW_BOLD : FW_NORMAL,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		_T("Malgun Gothic")
	);

	HFONT hOldFont = (HFONT)::SelectObject(m_hDCFrameBuffer, hFont);

	::SetTextColor(m_hDCFrameBuffer, color);
	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);

	RECT rcText = rc;
	rcText.top = y;
	rcText.bottom = y + nHeight + 20;

	::DrawText(m_hDCFrameBuffer, pszText, -1, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	::SelectObject(m_hDCFrameBuffer, hOldFont);
	::DeleteObject(hFont);
}

void CGameFramework::DrawTopLeftText(LPCTSTR pszText, int x, int y, int nHeight, COLORREF color, bool bBold)
{
	HFONT hFont = ::CreateFont(
		nHeight, 0, 0, 0,
		bBold ? FW_BOLD : FW_NORMAL,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		_T("Malgun Gothic")
	);

	HFONT hOldFont = (HFONT)::SelectObject(m_hDCFrameBuffer, hFont);

	::SetTextColor(m_hDCFrameBuffer, color);
	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);

	RECT rcText;
	rcText.left = x;
	rcText.top = y;
	rcText.right = x + 300;
	rcText.bottom = y + nHeight + 20;

	::DrawText(m_hDCFrameBuffer, pszText, -1, &rcText, DT_LEFT | DT_TOP | DT_SINGLELINE);

	::SelectObject(m_hDCFrameBuffer, hOldFont);
	::DeleteObject(hFont);
}

void CGameFramework::DrawMenuItem(LPCTSTR pszText, int y, bool bSelected,COLORREF baseColor)
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	COLORREF color = bSelected ? baseColor : RGB(230,230,230);
	DrawCenteredText(rcClient, pszText, y, 28, color, bSelected);
}

void CGameFramework::DrawSettingItem(LPCTSTR pszLeft, LPCTSTR pszRight, int y, bool bSelected)
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	int fontSize = max(20, height / 24);

	HFONT hFont = ::CreateFont(
		fontSize, 0, 0, 0,
		bSelected ? FW_BOLD : FW_NORMAL,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		_T("Malgun Gothic")
	);

	HFONT hOldFont = (HFONT)::SelectObject(m_hDCFrameBuffer, hFont);
	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
	::SetTextColor(m_hDCFrameBuffer, bSelected ? RGB(255, 220, 0) : RGB(230, 230, 230));

	int marginX = width / 5;

	RECT rcLeft = rcClient;
	rcLeft.left = marginX;
	rcLeft.right = width / 2;

	RECT rcRight = rcClient;
	rcRight.left = width / 2;
	rcRight.right = width - marginX;

	rcLeft.top = y;
	rcLeft.bottom = y + fontSize + 20;
	rcRight.top = y;
	rcRight.bottom = y + fontSize + 20;

	::DrawText(m_hDCFrameBuffer, pszLeft, -1, &rcLeft, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	::DrawText(m_hDCFrameBuffer, pszRight, -1, &rcRight, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	::SelectObject(m_hDCFrameBuffer, hOldFont);
	::DeleteObject(hFont);
}

void CGameFramework::DrawBitmap(HBITMAP hBitmap)
{
	if (!hBitmap) return;

	BITMAP bmp;
	::GetObject(hBitmap, sizeof(BITMAP), &bmp);

	HDC hMemDC = ::CreateCompatibleDC(m_hDCFrameBuffer);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	::SetStretchBltMode(m_hDCFrameBuffer, HALFTONE);
	::SetBrushOrgEx(m_hDCFrameBuffer, 0, 0, NULL);

	::StretchBlt(
		m_hDCFrameBuffer,
		0, 0,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		hMemDC,
		0, 0,
		bmp.bmWidth,
		bmp.bmHeight,
		SRCCOPY
	);

	::SelectObject(hMemDC, hOldBitmap);
	::DeleteDC(hMemDC);
}

void CGameFramework::RenderMenuBackground()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	if (m_hMenuBitmap)
	{
		DrawBitmap(m_hMenuBitmap);
	}
	else
	{
		HBRUSH hBrush = ::CreateSolidBrush(RGB(18, 24, 40));
		::FillRect(m_hDCFrameBuffer, &rcClient, hBrush);
		::DeleteObject(hBrush);
	}

	HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(70, 90, 140));
	HPEN hOldPen = (HPEN)::SelectObject(m_hDCFrameBuffer, hPen);

	::SelectObject(m_hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
}

void CGameFramework::RenderLoadingScreen()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	if (m_hLoadingBitmap)
	{
		DrawBitmap(m_hLoadingBitmap);
	}
	else
	{
		HBRUSH hBrush = ::CreateSolidBrush(RGB(10, 10, 20));
		::FillRect(m_hDCFrameBuffer, &rcClient, hBrush);
		::DeleteObject(hBrush);
	}

	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	double titleY = height / 2.5;
	double percentY = titleY + height / 3;
	double guideY = percentY + height / 8;

	int titleSize = max(32, height / 12);
	int percentSize = max(24, height / 20);
	int guideSize = max(24, height / 36);

	DrawCenteredText(rcClient, _T("LOADING..."), titleY, titleSize, RGB(0, 0, 0), true);

	int nPercent = (int)(m_Loading * 20.0f);
	if (nPercent > 100) nPercent = 100;

	TCHAR szLoading[64];
	_stprintf_s(szLoading, _T("%d%%"), nPercent);

	DrawCenteredText(rcClient, szLoading, percentY, percentSize, RGB(0, 0, 0), false);
	DrawCenteredText(rcClient, _T("잠시 후 메뉴로 이동합니다"), guideY, guideSize, RGB(0, 0, 0), false);
}

void CGameFramework::RenderStartMenu()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	RenderMenuBackground();

	int titleY = height / 6;
	int subtitleY = height / 6 + height / 10;

	int menuBaseY = height / 2 - height / 12;
	int menuGap = max(40, height / 10);

	int guideY = height - height / 6;

	int titleSize = max(32, height / 12);
	int subSize = max(18, height / 28);
	int menuSize = max(40, height / 18);
	int guideSize = max(24, height / 40);

	DrawCenteredText(rcClient, _T("3D GAME PROJECT 01"), titleY, titleSize, RGB(255, 255, 255), true);

	DrawCenteredText(
		rcClient,
		_T("START"),
		menuBaseY,
		menuSize,
		(m_StartMenuIdx == 0) ? RGB(255, 255, 255) : RGB(0, 0, 255),
		true
	);

	DrawCenteredText(
		rcClient,
		_T("SETTING"),
		menuBaseY + menuGap,
		menuSize,
		(m_StartMenuIdx == 1) ? RGB(255, 255, 255) : RGB(0, 255, 0),
		true
	);

	DrawCenteredText(
		rcClient,
		_T("QUIT"),
		menuBaseY + menuGap * 2,
		menuSize,
		(m_StartMenuIdx == 2) ? RGB(255, 255, 255) : RGB(255, 0, 0),
		true
	);

	DrawCenteredText(rcClient, _T("방향키: 이동 / Enter: 선택"), guideY, guideSize, RGB(255, 0, 0), true);
}

void CGameFramework::RenderSettingsMenu()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	RenderMenuBackground();

	int titleY = height / 6;
	int subtitleY = titleY + height / 10;

	int itemBaseY = height / 2 - height / 10;
	int itemGap = max(45, height / 9);

	int guideY = height - height / 8;

	int titleSize = max(30, height / 13);
	int subSize = max(16, height / 30);
	int guideSize = max(14, height / 40);

	DrawCenteredText(rcClient, _T("SETTINGS"), titleY, titleSize, RGB(255, 255, 255), true);

	DrawSettingItem(_T("SOUND"), m_SoundOn ? _T("ON") : _T("OFF"), itemBaseY, (m_SettingMenuIdx == 0));
	DrawSettingItem(_T("FULLSCREEN"), m_Fullscreen ? _T("ON") : _T("OFF"), itemBaseY + itemGap, (m_SettingMenuIdx == 1));
	DrawSettingItem(_T("BACK"), _T("START MENU"), itemBaseY + itemGap * 2, (m_SettingMenuIdx == 2));

	DrawCenteredText(rcClient, _T("방향키 : 이동 / 좌우 or ENTER : 변경 / ESC : 메뉴로"), guideY, guideSize, RGB(255, 0, 0), false);
}

void CGameFramework::ExecuteStartMenu()
{
	switch (m_StartMenuIdx)
	{
	case 0:
		ChangeState(GAMESTATE::INGAME);
		break;
	case 1:
		ChangeState(GAMESTATE::SETTINGS);
		break;
	case 2:
		ChangeState(GAMESTATE::EXIT);
		break;
	}
}

void CGameFramework::ToggleFullscreen()
{
	if (!m_Fullscreen)
	{
		::GetWindowRect(m_hWnd, &m_rcWindowed);

		MONITORINFO mi = { sizeof(MONITORINFO) };
		if (::GetMonitorInfo(::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST), &mi))
		{
			::SetWindowLong(m_hWnd, GWL_STYLE, m_dwWindowStyle & ~WS_OVERLAPPEDWINDOW);

			int nWidth = mi.rcMonitor.right - mi.rcMonitor.left;
			int nHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;

			::SetWindowPos(
				m_hWnd,
				HWND_TOP,
				mi.rcMonitor.left,
				mi.rcMonitor.top,
				nWidth,
				nHeight,
				SWP_FRAMECHANGED | SWP_SHOWWINDOW
			);

			ResizeFrameBuffer(nWidth, nHeight);
			UpdateViewport(nWidth, nHeight);

			::ShowWindow(m_hWnd, SW_MAXIMIZE);
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}

		m_Fullscreen = true;
	}
	else
	{
		::SetWindowLong(m_hWnd, GWL_STYLE, m_dwWindowStyle);

		::SetWindowPos(
			m_hWnd,
			HWND_NOTOPMOST,
			m_rcWindowed.left,
			m_rcWindowed.top,
			m_rcWindowed.right - m_rcWindowed.left,
			m_rcWindowed.bottom - m_rcWindowed.top,
			SWP_FRAMECHANGED | SWP_SHOWWINDOW
		);

		::ShowWindow(m_hWnd, SW_RESTORE);

		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		int nWidth = rcClient.right - rcClient.left;
		int nHeight = rcClient.bottom - rcClient.top;

		ResizeFrameBuffer(nWidth, nHeight);
		UpdateViewport(nWidth, nHeight);

		::InvalidateRect(m_hWnd, NULL, TRUE);

		m_Fullscreen = false;
	}
}

void CGameFramework::ExecuteSettingsMenu()
{
	switch (m_SettingMenuIdx)
	{
	case 0:
		m_SoundOn = !m_SoundOn;
		m_Sound.SetSoundOn(m_SoundOn);

		if (m_SoundOn)
		{
			if (m_eGameState == GAMESTATE::STARTMENU || m_eGameState == GAMESTATE::SETTINGS)
				m_Sound.PlayBGM(_T("menu.mp3"));
			else if (m_eGameState == GAMESTATE::LOADING)
				m_Sound.PlayBGM(_T("loading.mp3"));

			else if (m_eGameState == GAMESTATE::INGAME)
				m_Sound.PlayBGM(_T("game.mp3"));
		}
		break;

	case 1:
		ToggleFullscreen();
		break;

	case 2:
		ChangeState(GAMESTATE::STARTMENU);
		break;
	}
}

void CGameFramework::RenderFinishScreen()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	RenderMenuBackground();

	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	int titleY = height / 4;
	int infoY = height - height/3;
	int guideY = height - height / 6;

	int titleSize = max(36, height / 10);
	int infoSize = max(36, height / 22);
	int guideSize = max(36, height / 36);

	DrawCenteredText(rcClient, _T("FINISH!!!!!!"), titleY, titleSize, RGB(255, 255, 0), true);

	TCHAR szResult[128];
	_stprintf_s(szResult, _T("개수 : %d"), m_nDestroyedCount);
	DrawCenteredText(rcClient, szResult, infoY, infoSize, RGB(255, 0, 255), true);

	DrawCenteredText(rcClient, _T("Enter 또는 ESC : 메뉴로"), guideY, guideSize, RGB(255, 0, 0), false);
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_eGameState != GAMESTATE::INGAME) return;

	switch (nMessageID)
	{
	case WM_RBUTTONDOWN:
		if (m_pSelectedObject) m_pSelectedObject->SetSelected(false);
		m_pSelectedObject = m_pScene->PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pPlayer->m_pCamera.get());
		if (m_pSelectedObject) m_pSelectedObject->SetSelected(true);
		break;
	case WM_LBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (nMessageID != WM_KEYDOWN) return;

	switch (m_eGameState)
	{
	case GAMESTATE::LOADING:
		if (wParam == VK_RETURN)
			ChangeState(GAMESTATE::STARTMENU);
		break;

	case GAMESTATE::STARTMENU:
		switch (wParam)
		{
		case VK_UP:
			m_StartMenuIdx--;
			if (m_StartMenuIdx < 0) m_StartMenuIdx = 2;
			break;
		case VK_DOWN:
			m_StartMenuIdx++;
			if (m_StartMenuIdx > 2) m_StartMenuIdx = 0;
			break;
		case VK_RETURN:
			ExecuteStartMenu();
			break;
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		}
		break;

	case GAMESTATE::SETTINGS:
		switch (wParam)
		{
		case VK_UP:
			m_SettingMenuIdx--;
			if (m_SettingMenuIdx < 0) m_SettingMenuIdx = 2;
			break;
		case VK_DOWN:
			m_SettingMenuIdx++;
			if (m_SettingMenuIdx > 2) m_SettingMenuIdx = 0;
			break;
		case VK_LEFT:
		case VK_RIGHT:
		case VK_RETURN:
			ExecuteSettingsMenu();
			break;
		case VK_ESCAPE:
			ChangeState(GAMESTATE::STARTMENU);
			break;
		}
		break;

	case GAMESTATE::INGAME:
		switch (wParam)
		{
		case VK_ESCAPE:
			ChangeState(GAMESTATE::STARTMENU);
			break;
		case VK_RETURN:
			break;
		case VK_SPACE:
			m_EventQueue.push({ EVENTTYPE::FIRE_BULLET, -1 });
			break;
		case 'V':
			m_pPlayer->ChangeCameraMode();
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			int index = int(wParam - '1');
			m_EventQueue.push({ EVENTTYPE::EXPLODE_ONE, index });
			break;
		}
		case 'B':
			m_EventQueue.push({ EVENTTYPE::EXPLODE_ALL, -1 });
			break;
		default:
			break;
		}
		break;

	case GAMESTATE::FINISH:
		switch (wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			ChangeState(GAMESTATE::STARTMENU);
			break;
		}
		break;

	case GAMESTATE::EXIT:
		::PostQuitMessage(0);
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
			m_GameTimer.Stop();
		else
			m_GameTimer.Start();
		break;
	}
	case WM_SIZE:
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

void CGameFramework::BuildObjects()
{
	CAirplaneMesh* pAirplaneMesh = new CAirplaneMesh(6.0f, 6.0f, 1.0f);

	m_pPlayer = std::make_unique<CAirplanePlayer>();
	m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
	m_pPlayer->SetMesh(pAirplaneMesh);
	m_pPlayer->SetColor(RGB(0, 0, 255));
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
	static_cast<CAirplanePlayer*>(m_pPlayer.get())->m_pSound = &m_Sound;

	m_pScene = std::make_unique<CScene>();
	m_pScene->BuildObjects();

	m_pScene->m_pPlayer = m_pPlayer.get();
	m_pScene->m_pDestroyedCount = &m_nDestroyedCount;
	m_pScene->m_pSound = &m_Sound;
}

void CGameFramework::ReleaseObjects()
{
	m_pScene.reset();
	m_pPlayer.reset();
	m_pSelectedObject = nullptr;
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
	if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);

	if (m_hWnd) DestroyWindow(m_hWnd);
	if (m_hMenuBitmap) ::DeleteObject(m_hMenuBitmap);
	if (m_hLoadingBitmap) ::DeleteObject(m_hLoadingBitmap);
	if (m_hSkyBitmap) ::DeleteObject(m_hSkyBitmap);
	m_Sound.StopBGM();
}

void CGameFramework::ProcessInput()
{
	if (m_eGameState != GAMESTATE::INGAME) return;

	static UCHAR pKeyBuffer[256];
	DWORD dwDirection = 0;
	float fRollDelta = 0.0f;
	if (GetKeyboardState(pKeyBuffer))
	{
		if (pKeyBuffer['W'] & 0xF0) dwDirection |= DIR_FORWARD;
		if (pKeyBuffer['S'] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeyBuffer['A'] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeyBuffer['D'] & 0xF0) dwDirection |= DIR_RIGHT;
		if (pKeyBuffer['Z'] & 0xF0) dwDirection |= DIR_UP;
		if (pKeyBuffer['C'] & 0xF0) dwDirection |= DIR_DOWN;
		if (pKeyBuffer['E'] & 0xF0) fRollDelta -= 2.0f; 
		if (pKeyBuffer['Q'] & 0xF0) fRollDelta += 2.0f; 
	}

	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;

	if (GetCapture() == m_hWnd)
	{
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);
		cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
		cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}

	float fMovespeed = 0.15f;
	if (pKeyBuffer[VK_SHIFT] & 0xF0) fMovespeed = 0.3f;

	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	{
		if (cxDelta || cyDelta)
		{
			if (pKeyBuffer[VK_RBUTTON] & 0xF0)
				m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
			else
				m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
		}
		if (fRollDelta != 0.0f) m_pPlayer->Rotate(0.0f, 0.0f, fRollDelta);
		if (dwDirection) m_pPlayer->Move(dwDirection, fMovespeed);
	}

	m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::ProcessEventQueue()
{
	while (!m_EventQueue.empty())
	{
		GameEvent event = m_EventQueue.front();
		m_EventQueue.pop();

		switch (event.eType)
		{
		case EVENTTYPE::FIRE_BULLET:
		{
			static_cast<CAirplanePlayer*>(m_pPlayer.get())->FireBullet(m_pSelectedObject);
			break;
		}
		case EVENTTYPE::EXPLODE_ONE:
		{
			if (m_pScene && event.nObjectIndex >= 0 &&
				event.nObjectIndex < static_cast<int>(m_pScene->m_ppObjects.size()))
			{
				CExplosiveObject* pExplosiveObject =
					static_cast<CExplosiveObject*>(m_pScene->m_ppObjects[event.nObjectIndex].get());
				if (!pExplosiveObject->m_bBlowingUp)
				{
					pExplosiveObject->m_bBlowingUp = true;
					if (m_SoundOn)
					{
						m_Sound.PlayEffect(_T("explosion.wav"));
					}
				}
			}
			break;
		}
		case EVENTTYPE::EXPLODE_ALL:
		{
			if (m_pScene)
			{
				for (auto& object : m_pScene->m_ppObjects)
				{
					CExplosiveObject* pExplosiveObject =
						static_cast<CExplosiveObject*>(object.get());
					if (!pExplosiveObject->m_bBlowingUp)
					{
						pExplosiveObject->m_bBlowingUp = true;
						if (m_SoundOn)
						{
							m_Sound.PlayEffect(_T("explosion.wav"));
						}
					}
				}
			}
			break;
		}
		}
	}
}

void CGameFramework::FrameAdvance()
{
	if (!m_bActive) return;

	m_GameTimer.Tick(0.0f);
	float fTimeElapsed = m_GameTimer.GetTimeElapsed();

	switch (m_eGameState)
	{
	case GAMESTATE::LOADING:
		UpdateLoading(fTimeElapsed);
		ClearFrameBuffer(RGB(10, 10, 20));
		RenderLoadingScreen();
		PresentFrameBuffer();
		break;

	case GAMESTATE::STARTMENU:
		ClearFrameBuffer(RGB(18, 24, 40));
		RenderStartMenu();
		PresentFrameBuffer();
		break;

	case GAMESTATE::SETTINGS:
		ClearFrameBuffer(RGB(18, 24, 40));
		RenderSettingsMenu();
		PresentFrameBuffer();
		break;

	case GAMESTATE::INGAME:
		ProcessInput();
		ProcessEventQueue();

		m_pPlayer->Animate(fTimeElapsed);
		m_pScene->Animate(fTimeElapsed);

		if (m_nDestroyedCount >= m_nFinishTarget)
		{
			ChangeState(GAMESTATE::FINISH);
			break;
		}

		ClearFrameBuffer(RGB(255, 255, 255));

		m_pScene->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera.get());
		m_pPlayer->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera.get());

		TCHAR szCount[64];
		_stprintf_s(szCount, _T("COUNT : %d / %d"), m_nDestroyedCount, m_nFinishTarget);
		DrawTopLeftText(szCount, 20, 20, 24, RGB(255, 255, 255), true);

		PresentFrameBuffer();

		m_GameTimer.GetFrameRate(m_pszFrameRate + 27, 22);
		::SetWindowText(m_hWnd, m_pszFrameRate);

		if (m_pSelectedObject)
		{
			CExplosiveObject* pExplosiveObject = static_cast<CExplosiveObject*>(m_pSelectedObject);
			if (pExplosiveObject->m_bBlowingUp)
			{
				m_pSelectedObject->SetSelected(false);
				m_pSelectedObject = NULL;
			}
		}
		break;

	case GAMESTATE::FINISH:
		ClearFrameBuffer(RGB(18, 24, 40));
		RenderFinishScreen();
		PresentFrameBuffer();
		break;

	case GAMESTATE::EXIT:
		::PostQuitMessage(0);
		break;
	}
}