#include "stdafx.h"
#include "GameFramework.h"

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

	// m_hMenuBitmap = (HBITMAP)::LoadImage(NULL, _T("menu_bg.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	// m_hLoadingBitmap = (HBITMAP)::LoadImage(NULL, _T("loading_bg.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);


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
		break;
	case GAMESTATE::STARTMENU:
		m_StartMenuIdx = 0;
		break;
	case GAMESTATE::SETTINGS:
		m_SettingMenuIdx = 0;
		break;
	case GAMESTATE::INGAME:
		break;
	case GAMESTATE::EXIT:
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

	HFONT hFont = ::CreateFont(
		26, 0, 0, 0,
		bSelected ? FW_BOLD : FW_NORMAL,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		_T("Malgun Gothic")
	);

	HFONT hOldFont = (HFONT)::SelectObject(m_hDCFrameBuffer, hFont);
	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
	::SetTextColor(m_hDCFrameBuffer, bSelected ? RGB(255, 220, 0) : RGB(230, 230, 230));

	RECT rcLeft = rcClient;
	rcLeft.left = 120;
	rcLeft.right = rcClient.right / 2;

	RECT rcRight = rcClient;
	rcRight.left = rcClient.right / 2;
	rcRight.right = rcClient.right - 120;

	rcLeft.top = y;
	rcLeft.bottom = y + 40;
	rcRight.top = y;
	rcRight.bottom = y + 40;

	::DrawText(m_hDCFrameBuffer, pszLeft, -1, &rcLeft, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	::DrawText(m_hDCFrameBuffer, pszRight, -1, &rcRight, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	::SelectObject(m_hDCFrameBuffer, hOldFont);
	::DeleteObject(hFont);
}

void CGameFramework::RenderMenuBackground()
{

	// DrawBitmap(m_hMenuBitmap);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	HBRUSH hBrush = ::CreateSolidBrush(RGB(18, 24, 40));
	::FillRect(m_hDCFrameBuffer, &rcClient, hBrush);
	::DeleteObject(hBrush);

	HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(70, 90, 140));
	HPEN hOldPen = (HPEN)::SelectObject(m_hDCFrameBuffer, hPen);

	::SelectObject(m_hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
}

void CGameFramework::RenderLoadingScreen()
{

	// DrawBitmap(m_hLoadingBitmap);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	RenderMenuBackground();

	DrawCenteredText(rcClient, _T("LOADING..."), 170, 42, RGB(255, 255, 255), true);

	int nPercent = (int)(m_Loading * 20.0f);
	if (nPercent > 100) nPercent = 100;

	TCHAR szLoading[64];
	_stprintf_s(szLoading, _T("%d%%"), nPercent);

	DrawCenteredText(rcClient, szLoading, 240, 26, RGB(210, 210, 210), false);
	DrawCenteredText(rcClient, _T("잠시 후 시작 메뉴로 이동합니다"), 300, 20, RGB(180, 180, 180), false);
}

void CGameFramework::RenderStartMenu()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	RenderMenuBackground();

	DrawCenteredText(rcClient, _T("3D GAME PROJECT 01"), 80, 52, RGB(255, 255, 255), true);
	DrawCenteredText(rcClient, _T("메뉴를 선택하세요"), 145, 22, RGB(190, 190, 190), false);

	DrawMenuItem(_T("START"), 220, (m_StartMenuIdx == 0),RGB(255,0,0));
	DrawMenuItem(_T("SETTING"), 270, (m_StartMenuIdx == 1), RGB(0, 255, 0));
	DrawMenuItem(_T("EXIT"), 320, (m_StartMenuIdx == 2), RGB(0, 0, 255));

	DrawCenteredText(rcClient, _T("방향키: 이동 / Enter: 선택"), 400, 18, RGB(170, 170, 170), false);
}

void CGameFramework::RenderSettingsMenu()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	RenderMenuBackground();

	DrawCenteredText(rcClient, _T("SETTINGS"), 80, 48, RGB(255, 255, 255), true);

	DrawSettingItem(_T("SOUND"), m_SoundOn ? _T("ON") : _T("OFF"), 230, (m_SettingMenuIdx == 0));
	DrawSettingItem(_T("FULLSCREEN"), m_Fullscreen ? _T("ON") : _T("OFF"), 280, (m_SettingMenuIdx == 1));
	DrawSettingItem(_T("BACK"), _T("START MENU"), 330, (m_SettingMenuIdx == 2));

	DrawCenteredText(rcClient, _T("방향키 : 이동 / 좌우 : 변경 / ESC : 메뉴로"), 410, 18, RGB(170, 170, 170), false);
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
		break;
	case 1:
		ToggleFullscreen();
		break;
	case 2:
		ChangeState(GAMESTATE::STARTMENU);
		break;
	}
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_eGameState != GAMESTATE::INGAME) return;

	switch (nMessageID)
	{
	case WM_RBUTTONDOWN:
		m_pSelectedObject = m_pScene->PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pPlayer->m_pCamera.get());
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
			static_cast<CAirplanePlayer*>(m_pPlayer.get())->FireBullet(m_pSelectedObject);
			break;
		default:
			m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
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

	m_pScene = std::make_unique<CScene>();
	m_pScene->BuildObjects();

	m_pScene->m_pPlayer = m_pPlayer.get();
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
}

void CGameFramework::ProcessInput()
{
	if (m_eGameState != GAMESTATE::INGAME) return;

	static UCHAR pKeyBuffer[256];
	DWORD dwDirection = 0;

	if (GetKeyboardState(pKeyBuffer))
	{
		if (pKeyBuffer['W'] & 0xF0) dwDirection |= DIR_FORWARD;
		if (pKeyBuffer['S'] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeyBuffer['A'] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeyBuffer['D'] & 0xF0) dwDirection |= DIR_RIGHT;
		if (pKeyBuffer['Z'] & 0xF0) dwDirection |= DIR_UP;
		if (pKeyBuffer['C'] & 0xF0) dwDirection |= DIR_DOWN;
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

	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	{
		if (cxDelta || cyDelta)
		{
			if (pKeyBuffer[VK_RBUTTON] & 0xF0)
				m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
			else
				m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
		}
		if (dwDirection) m_pPlayer->Move(dwDirection, 0.15f);
	}

	m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
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

		m_pPlayer->Animate(fTimeElapsed);
		m_pScene->Animate(fTimeElapsed);

		ClearFrameBuffer(RGB(255, 255, 255));

		m_pScene->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera.get());
		m_pPlayer->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera.get());

		PresentFrameBuffer();

		m_GameTimer.GetFrameRate(m_pszFrameRate + 27, 22);
		::SetWindowText(m_hWnd, m_pszFrameRate);
		break;

	case GAMESTATE::EXIT:
		::PostQuitMessage(0);
		break;
	}
}