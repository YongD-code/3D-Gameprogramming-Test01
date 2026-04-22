#pragma once
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class CSound
{
public:
	CSound();
	~CSound();

	void SetSoundOn(bool bOn);
	bool IsSoundOn() const;

	void PlayEffect(LPCTSTR pszFileName);
	void PlayBGM(LPCTSTR pszFileName);
	void StopBGM();

private:
	bool m_SoundOn = true;
	bool  m_bBGMPlaying = false;
	TCHAR m_szCurrentBGM[MAX_PATH] = { 0 };
};