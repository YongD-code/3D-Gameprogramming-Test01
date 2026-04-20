#include "stdafx.h"
#include "Sound.h"

CSound::CSound()
{
}

CSound::~CSound()
{
	StopBGM();
}

void CSound::SetSoundOn(bool on)
{
	m_SoundOn = on;

	if (!m_SoundOn)
	{
		StopBGM();
	}
}

bool CSound::IsSoundOn() const
{
	return m_SoundOn;
}

void CSound::PlayEffect(LPCTSTR pszFileName)
{
	if (!m_SoundOn) return;

	::PlaySound(pszFileName,NULL,SND_FILENAME | SND_ASYNC);
}

void CSound::PlayBGM(LPCTSTR pszFileName)
{
	if (!m_SoundOn) return;

	::PlaySound(pszFileName,NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void CSound::StopBGM()
{
	::PlaySound(NULL, NULL, 0);
}