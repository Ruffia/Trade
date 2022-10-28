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
// 使用WIN32 API 打开当COM号大于10的时候，会出现打开错误或失败，一般解决的办法是人工修改USB 串口的com的名称让它的com号是单位数（如： COM1~COM9）
// 但这样较麻烦。而且在设备接入大量串口设备时，没办法解决此问题。
//
// 产生这种奇怪现象的原因是：微软预定义的标准设备中含有“COM1”-“COM9”。
// 所以，“COM1”-“COM9”作为文件名传递给函数时操作系统会自动地将之解析为相应的设备。
// 但对于COM10及以上的串口，“COM10”之类的文件名系统只视之为一般意义上的文件，而非串行设备。
//
// 为了增加对COM10及以上串行端口的支持，微软规定：
// 如果要访问这样的设备，应使用这样的文件名（以COM10为例）：\\.\COM10（后试验发现，COM1-9也可用此格式）
// 所以，对于COM10及以上的串口，CreateFile()的调用样式应调整如下：
//
// CreateFile(
//			"\\\\.\\COM10",		// 定义串口名，实际格式化时，需要反斜杠数翻倍，格式化出来的才是“\\.\COM10”
//			fdwAccess,			// 存取模式（读写）
//			0,					// 共享模式：必须设置为0，表示设备独占使用
//			NULL,				// 保密性
//			OPEN_EXISTING,		// 必须设置为OPEN_EXISTING
//			0,					// 文件属性，如果是异步模式，可设置为 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
//			NULL				// 模版，串口设备必须设置为NULL
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

