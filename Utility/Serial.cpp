// Serial.cpp

#include "stdafx.h"
#include "Serial.h"

CSerial::CSerial()
{
	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );
	m_hIDComDev = NULL;
	m_bOpened = FALSE;
}

CSerial::~CSerial()
{
	Close();
}

/////////////////////////////////////////////////////////////////////
// ʹ��WIN32 API �򿪵�COM�Ŵ���10��ʱ�򣬻���ִ򿪴����ʧ�ܣ�һ�����İ취���˹��޸�USB ���ڵ�com������������com���ǵ�λ�����磺 COM1~COM9��
// ���������鷳���������豸������������豸ʱ��û�취��������⡣
//
// ����������������ԭ���ǣ�΢��Ԥ����ı�׼�豸�к��С�COM1��-��COM9����
// ���ԣ���COM1��-��COM9����Ϊ�ļ������ݸ�����ʱ����ϵͳ���Զ��ؽ�֮����Ϊ��Ӧ���豸��
// ������COM10�����ϵĴ��ڣ���COM10��֮����ļ���ϵͳֻ��֮Ϊһ�������ϵ��ļ������Ǵ����豸��
//
// Ϊ�����Ӷ�COM10�����ϴ��ж˿ڵ�֧�֣�΢��涨��
// ���Ҫ�����������豸��Ӧʹ���������ļ�������COM10Ϊ������\\.\COM10�������鷢�֣�COM1-9Ҳ���ô˸�ʽ��
// ���ԣ�����COM10�����ϵĴ��ڣ�CreateFile()�ĵ�����ʽӦ�������£�
//
// CreateFile(
//			"\\\\.\\COM10",		// ���崮������ʵ�ʸ�ʽ��ʱ����Ҫ��б������������ʽ�������Ĳ��ǡ�\\.\COM10��
//			fdwAccess,			// ��ȡģʽ����д��
//			0,					// ����ģʽ����������Ϊ0����ʾ�豸��ռʹ��
//			NULL,				// ������
//			OPEN_EXISTING,		// ��������ΪOPEN_EXISTING
//			0,					// �ļ����ԣ�������첽ģʽ��������Ϊ FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
//			NULL				// ģ�棬�����豸��������ΪNULL
//			);
//
// C/C++/C#:	hPort = CreateFile("\\\\.\\COM15", ...); 
// VB:			hPort = CreateFile("\\.\COM15", ...); 
// Delphi:		hPort := CreateFile('\\.\COM15', ...);
/////////////////////////////////////////////////////////////////////

BOOL CSerial::Open( int nPort, int nBaud, int nData, int nStop )
{
	if( m_bOpened ) return( TRUE );

	char szPort[50];
	char szComParams[50];
	DCB dcb;

	wsprintf( szPort, "\\\\.\\COM%d", nPort );
	m_hIDComDev = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
	if( m_hIDComDev == NULL || m_hIDComDev == INVALID_HANDLE_VALUE )
	{
		DWORD dwError = GetLastError();
		TRACE( "CreateFile failed! error = %ld", dwError );
		return( FALSE );
	}

	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts( m_hIDComDev, &CommTimeOuts );

	wsprintf( szComParams, "COM%d:%d,n,%d,%d", nPort, nBaud, nData, nStop );

	m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	dcb.DCBlength = sizeof( DCB );
	GetCommState( m_hIDComDev, &dcb );
	dcb.BaudRate = nBaud;
	dcb.ByteSize = nData;	//8;
	switch( nStop )
	{
	case	0:
	case	1:
		dcb.StopBits = ONESTOPBIT;
		break;
	case	2:
		dcb.StopBits = TWOSTOPBITS;
		break;
	default:
		dcb.StopBits = ONE5STOPBITS;
		break;
	}
	unsigned char ucSet;
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_DTRDSR ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_RTSCTS ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_XONXOFF ) != 0 );
	if( !SetCommState( m_hIDComDev, &dcb ) ||
		!SetupComm( m_hIDComDev, 10000, 10000 ) ||
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL ){
		DWORD dwError = GetLastError();
		TRACE( "SetCommState or SetupComm failed! error = %ld", dwError );
		if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
		if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
		CloseHandle( m_hIDComDev );
		return( FALSE );
		}

	m_bOpened = TRUE;

	return( m_bOpened );
}

BOOL CSerial::Close( void )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( TRUE );

	if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
	if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
	CloseHandle( m_hIDComDev );
	m_bOpened = FALSE;
	m_hIDComDev = NULL;

	return( TRUE );
}

BOOL CSerial::WriteCommByte( unsigned char ucByte )
{
	BOOL bWriteStat;
	DWORD dwBytesWritten;

	bWriteStat = WriteFile( m_hIDComDev, (LPSTR) &ucByte, 1, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) ){
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) dwBytesWritten = 0;
		else{
			GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, FALSE );
			m_OverlappedWrite.Offset += dwBytesWritten;
			}
		}

	return( TRUE );
}

int CSerial::SendData( const char *buffer, int size )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	DWORD dwBytesWritten = 0;
	int i;
	for( i=0; i<size; i++ ){
		WriteCommByte( buffer[i] );
		dwBytesWritten++;
		}

	return( (int) dwBytesWritten );
}

int CSerial::SendDataEx( const BYTE *buffer, int size )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	BOOL bWriteStat;
	DWORD dwBytesWritten = 0;
	bWriteStat = WriteFile( m_hIDComDev, buffer, size, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) ){
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) dwBytesWritten = 0;
		else{
			GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, FALSE );
			m_OverlappedWrite.Offset += dwBytesWritten;
		}
	}

	return( (int) dwBytesWritten );
}

int CSerial::SendDataEx2( const char *buffer, int size )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	BOOL bWriteStat;
	DWORD dwBytesWritten = 0;
	bWriteStat = WriteFile( m_hIDComDev, buffer, size, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) ){
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) dwBytesWritten = 0;
		else{
			GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, FALSE );
			m_OverlappedWrite.Offset += dwBytesWritten;
		}
	}

	return( (int) dwBytesWritten );
}

int CSerial::ReadDataWaiting( void )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	DWORD dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );

	return( (int) ComStat.cbInQue );
}

int CSerial::ReadData( void *buffer, int limit )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	if( !ComStat.cbInQue ) return( 0 );

	dwBytesRead = (DWORD) ComStat.cbInQue;
	if( limit < (int) dwBytesRead ) dwBytesRead = (DWORD) limit;

	bReadStatus = ReadFile( m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead );
	if( !bReadStatus ){
		if( GetLastError() == ERROR_IO_PENDING ){
			WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
			return( (int) dwBytesRead );
			}
		return( 0 );
		}

	return( (int) dwBytesRead );
}

