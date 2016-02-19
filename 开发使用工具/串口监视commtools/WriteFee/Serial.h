// Serial.h: interface for the CSerial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIAL_H__59871226_E8C9_11D2_8F49_0080AD8400EE__INCLUDED_)
#define AFX_SERIAL_H__59871226_E8C9_11D2_8F49_0080AD8400EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
class CSerial  
{
public:
	CSerial();
	virtual ~CSerial();

	DWORD Write(LPCVOID lpBuffer, DWORD dwRequest);
	DWORD Read(LPVOID lpBuffer, DWORD dwRequest);
	DWORD SafeWrite(LPCVOID lpBuffer, DWORD dwRequest);
	DWORD SafeRead(LPVOID lpBuffer, DWORD dwRequest);
	DWORD GetInBufferCount();
	DWORD GetOutBufferCount();
	BOOL  Open();
	void  Close();
	BOOL  GetStatus();
	BOOL  SetStatus();
	BOOL  IsOpen();

public:
	HANDLE GetHandle();
	void CopyHandle(HANDLE handle);
	BOOL Purge(DWORD dwFlag);
	int    m_nComID;
	int    m_nBaudRate;
	int    m_nDataBits;
	int    m_nStopBits;
	int    m_nParity;

protected:
	HANDLE m_hCom;
	CMutex m_mutexRead;
	CMutex m_mutexWrite;
};

#endif // !defined(AFX_SERIAL_H__59871226_E8C9_11D2_8F49_0080AD8400EE__INCLUDED_)