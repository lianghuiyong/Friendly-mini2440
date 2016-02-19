// Serial.cpp: implementation of the CSerial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Serial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerial::CSerial()
{
	m_nComID = -1;
	m_nBaudRate = 57600;	//9600;
	m_nDataBits = 8;
	m_nParity = NOPARITY;
	m_nStopBits = ONESTOPBIT;

	m_hCom = INVALID_HANDLE_VALUE;
}

CSerial::~CSerial()
{
    if (IsOpen())
        Close();
}

BOOL CSerial::Open()
{
    if (m_nComID == -1)
        return FALSE;

	CString strPort;
	strPort.Format("COM%d",m_nComID + 1);

	m_hCom = CreateFile( 
		strPort, 
		GENERIC_READ | GENERIC_WRITE,
		0,                    // exclusive access
		NULL,                 // no security attrs
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// |
		//                  FILE_FLAG_OVERLAPPED, // overlapped I/O
		NULL );

	if (m_hCom == INVALID_HANDLE_VALUE) 
		return FALSE;

	// purge any information in the buffer
	PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
    SetupComm(m_hCom,2048,512);

	COMMTIMEOUTS CommTimeOuts;
    memset(&CommTimeOuts, 0, sizeof(COMMTIMEOUTS));
    /*
	CommTimeOuts.ReadIntervalTimeout = 2000;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 2000 ;
	CommTimeOuts.ReadTotalTimeoutConstant = 5000;

	CommTimeOuts.WriteTotalTimeoutMultiplier = 1500;
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts( m_hCom, &CommTimeOuts ) ;
*/
	DCB        dcb ;
	dcb.DCBlength = sizeof( DCB ) ;

	GetCommState( m_hCom, &dcb ) ;

	dcb.BaudRate = m_nBaudRate;
	dcb.ByteSize = m_nDataBits;
	dcb.StopBits = m_nStopBits;

	dcb.Parity = m_nParity;
	dcb.fParity = m_nParity == 0 ? FALSE : TRUE;

    // setup hardware flow control

    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;

    // setup software flow control

    dcb.fInX = dcb.fOutX = FALSE ;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;

	dcb.fBinary = TRUE;

	if (!SetCommState( m_hCom, &dcb ))
	{
		CloseHandle(m_hCom);
		m_hCom = INVALID_HANDLE_VALUE;
		return FALSE;
	}

	return TRUE;
}

void CSerial::Close()
{
    if (!IsOpen())
        return;

	PurgeComm(m_hCom,-1);
	CloseHandle(m_hCom);

	m_hCom = INVALID_HANDLE_VALUE;
}

DWORD CSerial::Write(LPCVOID lpBuffer, DWORD dwRequest)
{
	if (m_hCom == INVALID_HANDLE_VALUE) 
		return 0;

    if (dwRequest == 0)
        return 0;

    DWORD dwWritten;

    if (!WriteFile( m_hCom, lpBuffer, dwRequest, &dwWritten, NULL))
        return 0;

    return dwWritten;
}

DWORD CSerial::Read(LPVOID lpBuffer, DWORD dwRequest)
{
	if(m_hCom == INVALID_HANDLE_VALUE) 
		return 0;

    DWORD dwCount = GetInBufferCount();

    if (dwRequest > dwCount)
        dwRequest = dwCount;

    if (dwRequest == 0)
        return 0;
	
	DWORD dwRead;

    if (!ReadFile( m_hCom, lpBuffer, dwRequest, &dwRead, NULL))
        return 0;

	return dwRead;
}

DWORD CSerial::SafeWrite(LPCVOID lpBuffer, DWORD dwRequest)
{
	if (m_hCom == INVALID_HANDLE_VALUE) 
		return 0;

	if (dwRequest == 0)
		return 0;

	CSingleLock sLock(&m_mutexWrite, TRUE);
	DWORD dwWritten;
	
	if (!WriteFile( m_hCom, lpBuffer, dwRequest, &dwWritten, NULL))
		return 0;
	
	return dwWritten;
}

DWORD CSerial::SafeRead(LPVOID lpBuffer, DWORD dwRequest)
{
	if(m_hCom == INVALID_HANDLE_VALUE) 
		return 0;

	DWORD dwCount = GetInBufferCount();
	
	if (dwRequest > dwCount)
		dwRequest = dwCount;
	
	if (dwRequest == 0)
		return 0;
	
	CSingleLock sLock(&m_mutexRead, TRUE);
	DWORD dwRead;
	
	if (!ReadFile( m_hCom, lpBuffer, dwRequest, &dwRead, NULL))
		return 0;
	
	return dwRead;
}

DWORD CSerial::GetInBufferCount()
{
	COMSTAT state;
	DWORD err;
	ClearCommError(m_hCom, &err, &state);

	return state.cbInQue;
}

BOOL CSerial::GetStatus()
{
	DCB        dcb;
	dcb.DCBlength = sizeof( DCB ) ;
	if (!GetCommState( m_hCom, &dcb ))
		return FALSE;
	
	m_nBaudRate = dcb.BaudRate; 
	m_nDataBits = dcb.ByteSize;
	m_nStopBits = dcb.StopBits;

	m_nParity = dcb.Parity;

	return TRUE;
}
BOOL CSerial::SetStatus()
{
	DCB        dcb ;
	dcb.DCBlength = sizeof( DCB ) ;

	GetCommState( m_hCom, &dcb ) ;

	dcb.BaudRate = m_nBaudRate;
	dcb.ByteSize = m_nDataBits;
	dcb.StopBits = m_nStopBits;

	dcb.Parity = m_nParity;
	dcb.fParity = m_nParity == 0 ? FALSE : TRUE;

	return SetCommState( m_hCom, &dcb );
}


DWORD CSerial::GetOutBufferCount()
{
	COMSTAT state;
	DWORD err;
	ClearCommError(m_hCom, &err, &state);

	return state.cbOutQue;
}


BOOL CSerial::IsOpen()
{
	return m_hCom != INVALID_HANDLE_VALUE;

}




BOOL CSerial::Purge(DWORD dwFlag)
{
    return ::PurgeComm(m_hCom, dwFlag);

}

void CSerial::CopyHandle(HANDLE handle)
{
	m_hCom = handle;
}

HANDLE CSerial::GetHandle()
{
	return m_hCom;
}
