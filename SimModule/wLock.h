///////////////////////////////////////////////////////////////
// 작성자 : 원순철 (wonsch6207@hanmail.net)
// 날  짜 : 최초작성 2011-??-??, 최종수정 2011-09-21
///////////////////////////////////////////////////////////////

#pragma once

class CwLock
{
public:
	CwLock()									{InitializeCriticalSection(&CS);}
	~CwLock()									{DeleteCriticalSection(&CS);}

	bool TryLock()								{return TryEnterCriticalSection(&CS) != FALSE;}
	void Lock()									{EnterCriticalSection(&CS);}
	void UnLock()								{LeaveCriticalSection(&CS);}

	CRITICAL_SECTION							CS;
};

class CwLockAuto
{
public:
	CwLockAuto(CRITICAL_SECTION *_CS)			{CS = _CS; EnterCriticalSection(CS);}
	CwLockAuto(CwLock *wLock)					{CS = &wLock->CS; EnterCriticalSection(CS);}
	~CwLockAuto()								{LeaveCriticalSection(CS);}

	CRITICAL_SECTION							*CS;
};
