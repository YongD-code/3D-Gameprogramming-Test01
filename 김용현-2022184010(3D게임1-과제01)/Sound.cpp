#include "stdafx.h"
#include "Sound.h"

CSound::CSound()
{
}

CSound::~CSound()
{
	StopBGM();
}

void CSound::SetSoundOn(bool bOn)
{
	m_SoundOn = bOn;

	if (!m_SoundOn)
	{
		StopBGM();
	}
	else
	{
		if (_tcslen(m_szCurrentBGM) > 0 && !m_bBGMPlaying)
		{
			PlayBGM(m_szCurrentBGM);
		}
	}
}

bool CSound::IsSoundOn() const
{
	return m_SoundOn;
}

void CSound::PlayEffect(LPCTSTR pszFileName)
{
	if (!m_SoundOn) return;
	if (!pszFileName || !pszFileName[0]) return;
	::PlaySound(pszFileName, NULL, SND_FILENAME | SND_ASYNC);
}

void CSound::PlayBGM(LPCTSTR pszFileName)
{
	if (!m_SoundOn) return;
	if (!pszFileName || !pszFileName[0]) return;

	StopBGM();

	_tcscpy_s(m_szCurrentBGM, pszFileName);

	TCHAR szOpenCmd[512];
	TCHAR szPlayCmd[128];

	_stprintf_s(szOpenCmd, _T("open \"%s\" alias bgm"), pszFileName);
	_stprintf_s(szPlayCmd, _T("play bgm repeat"));

	::mciSendString(szOpenCmd, NULL, 0, NULL);
	::mciSendString(szPlayCmd, NULL, 0, NULL);

	m_bBGMPlaying = true;
}

void CSound::StopBGM()
{
	::mciSendString(_T("stop bgm"), NULL, 0, NULL);
	::mciSendString(_T("close bgm"), NULL, 0, NULL);
	m_bBGMPlaying = false;
}