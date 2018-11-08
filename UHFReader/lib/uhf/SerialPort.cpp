//////////////////////////////////////////////////////////////////////////
/// COPYRIGHT NOTICE
/// Copyright (c) 2011, �Ϻ���ͨ��ѧ ������ʵ����  ����Ȩ������
/// All rights reserved.
///
/// @file    SerialPort.cpp  
/// @brief   ����ͨ�����ʵ���ļ�
///
/// ���ļ�Ϊ����ͨ�����ʵ�ִ���
///
/// @version 1.0   
/// @author  �½� 
/// @E-mail��chjie@sjtu.edu.cn
/// @date    2011/04/19
///
///  �޶�˵����
//////////////////////////////////////////////////////////////////////////

#include "SerialPort.h"
#include <process.h>
#include <iostream>

using namespace std;

/** �߳��˳���־ */ 
bool CSerialPort::s_bExit = false;
///** heartBeat�߳��˳���־ */
//bool CSerialPort::s_bHBExit = false;
/** ������������ʱ,sleep���´β�ѯ�����ʱ��,��λ:�� */ 
const UINT SLEEP_TIME_INTERVAL = 5;
//char buffer[100];

char *buffer;
int accum;
//bool ocpMark;
//bool flag = false;
//bool invFlag = false;

CSerialPort::CSerialPort(const char *portName)
: m_hListenThread(INVALID_HANDLE_VALUE)//�����Ա������ֵ
{

//        if(pro_Flag!=0){
//            return;}

        m_hComm = INVALID_HANDLE_VALUE;//INVALID_HANDLE_VALUE
	m_hListenThread = INVALID_HANDLE_VALUE;
        o_eCode = 0;
        buffer = NULL;

	InitializeCriticalSection(&m_csCommunicationSync);

    if (!InitPort(portName))
	{
            if(5==o_eCode){
            OcpMark::instance()->setVal(true);
            }
            std::cout << "initPort fail !" << std::endl;
//            exit(-1);
	}
	else
	{
		std::cout << "initPort success !" << std::endl;
	}

	if (!OpenListenThread())
	{
            std::cout << "OpenListenThread fail !" << std::endl;
	}
	else
        {
            std::cout << "OpenListenThread success !" << std::endl;
	}
}

CSerialPort::~CSerialPort(void)//��Ӧ���캯������Ϊ�����������ڶ�������ʱ����
{
	CloseListenTread();
	ClosePort();
        delete [] buffer;
        OcpMark::instance()->setVal(false);
	DeleteCriticalSection(&m_csCommunicationSync);
}

bool CSerialPort::InitPort( const char *portName /*= 1*/,UINT baud /*= CBR_115200*/,char parity /*= 'N'*/,
						    UINT databits /*= 8*/, UINT stopsbits /*= 1*/,DWORD dwCommEvents /*= EV_RXCHAR*/ )
{

	/** ��ʱ����,���ƶ�����ת��Ϊ�ַ�����ʽ,�Թ���DCB�ṹ */ 
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

	/** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */ 
    if (!openPort(portName))
	{
		return false;
	}

	/** �����ٽ�� */ 
	EnterCriticalSection(&m_csCommunicationSync);

	/** �Ƿ��д����� */ 
	BOOL bIsSuccess = TRUE;

    /** �ڴ˿���������������Ļ�������С,���������,��ϵͳ������Ĭ��ֵ.
	 *  �Լ����û�������Сʱ,Ҫע�������Դ�һЩ,���⻺�������
	 */
	/*if (bIsSuccess )
	{
		bIsSuccess = SetupComm(m_hComm,10,10);
	}*/

	/** ���ô��ڵĳ�ʱʱ��,����Ϊ0,��ʾ��ʹ�ó�ʱ���� */
	COMMTIMEOUTS  CommTimeouts;
        CommTimeouts.ReadIntervalTimeout         = 0;
        CommTimeouts.ReadTotalTimeoutMultiplier  = 3;
        CommTimeouts.ReadTotalTimeoutConstant    = 2000;
        CommTimeouts.WriteTotalTimeoutMultiplier = 3;
        CommTimeouts.WriteTotalTimeoutConstant   = 2000;
	if ( bIsSuccess)
	{
		bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);
	}

	DCB  dcb;
	if ( bIsSuccess )
	{
		// ��ANSI�ַ���ת��ΪUNICODE�ַ���
		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNum] ;
		if (!MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
		{
			bIsSuccess = TRUE;
		}

		/** ��ȡ��ǰ�������ò���,���ҹ��촮��DCB���� */ 
		bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb) ;
		/** ����RTS flow���� */ 
		dcb.fRtsControl = RTS_CONTROL_ENABLE; 

		/** �ͷ��ڴ�ռ� */ 
		delete [] pwText;
	}

	if ( bIsSuccess )
	{
		/** ʹ��DCB�������ô���״̬ */ 
		bIsSuccess = SetCommState(m_hComm, &dcb);
	}
		
	/**  ��մ��ڻ����� */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/** �뿪�ٽ�� */ 
	LeaveCriticalSection(&m_csCommunicationSync);

	return bIsSuccess==TRUE;
}

bool CSerialPort::InitPort( const char *portName ,const LPDCB& plDCB )
{
	/** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */ 
    if (!openPort(portName))
	{
		return false;
	}
	
	/** �����ٽ�� */ 
	EnterCriticalSection(&m_csCommunicationSync);

	/** ���ô��ڲ��� */ 
	if (!SetCommState(m_hComm, plDCB))
	{
		return false;
	}

	/**  ��մ��ڻ����� */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/** �뿪�ٽ�� */ 
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

void CSerialPort::ClosePort()
{
	/** ����д��ڱ��򿪣��ر��� */
	if( m_hComm != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hComm );
		m_hComm = INVALID_HANDLE_VALUE;
	}
}

bool CSerialPort::openPort( const char *portName )
{
	/** �����ٽ�� */ 
	EnterCriticalSection(&m_csCommunicationSync);

	/** ��ָ���Ĵ��� */ 
        m_hComm = CreateFileA(portName,		                /** �豸��,COM1,COM2�� */
						 GENERIC_READ | GENERIC_WRITE,  /** ����ģʽ,��ͬʱ��д */   
						 0,                             /** ����ģʽ,0��ʾ������ */ 
					     NULL,							/** ��ȫ������,һ��ʹ��NULL */ 
					     OPEN_EXISTING,					/** �ò�����ʾ�豸�������,���򴴽�ʧ�� */ 
                                                 0,
						 0);    

	/** �����ʧ�ܣ��ͷ���Դ������ */ 
        if (m_hComm == INVALID_HANDLE_VALUE || m_hComm == ((HANDLE)-1))
	{
                o_eCode = GetLastError();
		LeaveCriticalSection(&m_csCommunicationSync);
//                cout<<"exit3"<<endl;
		return false;
	}

	/** �˳��ٽ��� */ 
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

bool CSerialPort::OpenListenThread()
{
	/** ����߳��Ƿ��Ѿ������� */ 
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** �߳��Ѿ����� */ 
		return false;
	}

	s_bExit = false;
	/** �߳�ID */ 
	UINT threadId;
	/** �����������ݼ����߳� */ 
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);
	if (!m_hListenThread)
	{
		return false;
	}

	/** �����̵߳����ȼ�,������ͨ�߳� */ 
        if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))
        {
                return false;
        }

	return true;
}

bool CSerialPort::CloseListenTread()
{	
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** ֪ͨ�߳��˳� */ 
		s_bExit = true;

		/** �ȴ��߳��˳� */ 
		Sleep(10);

		/** ���߳̾����Ч */ 
		CloseHandle( m_hListenThread );
		m_hListenThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

//bool CSerialPort::OpenHeartBeatThread()
//{
//        /** ����߳��Ƿ��Ѿ������� */
//        if (m_hHeartBeatThread != INVALID_HANDLE_VALUE)
//        {
//                /** �߳��Ѿ����� */
//                return false;
//        }

//        s_bExit = false;
//        /** �߳�ID */
//        UINT threadId;
//        /** �����������ݼ����߳� */
//        m_hHeartBeatThread = (HANDLE)_beginthreadex(NULL, 0, HeartBeatThread, this, 0, &threadId);
//        if (!m_hHeartBeatThread)
//        {
//                return false;
//        }

//        /** �����̵߳����ȼ�,������ͨ�߳� */
//        if (!SetThreadPriority(m_hHeartBeatThread, THREAD_PRIORITY_ABOVE_NORMAL))
//        {
//                return false;
//        }

//        return true;
//}

//bool CSerialPort::CloseHeartBeatThread()
//{
//        if (m_hHeartBeatThread != INVALID_HANDLE_VALUE)
//        {
//                /** ֪ͨ�߳��˳� */
//                s_bHBExit = true;

//                /** �ȴ��߳��˳� */
//                Sleep(10);

//                /** ���߳̾����Ч */
//                CloseHandle( m_hHeartBeatThread );
//                m_hHeartBeatThread = INVALID_HANDLE_VALUE;
//        }
//        return true;
//}

UINT CSerialPort::GetBytesInCOM()
{
	DWORD dwError = 0;	/** ������ */ 
	COMSTAT  comstat;   /** COMSTAT�ṹ��,��¼ͨ���豸��״̬��Ϣ */ 
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	/** �ڵ���ReadFile��WriteFile֮ǰ,ͨ�������������ǰ�����Ĵ����־ */ 
	if ( ClearCommError(m_hComm, &dwError, &comstat) )
	{
		BytesInQue = comstat.cbInQue; /** ��ȡ�����뻺�����е��ֽ��� */ 
	}

	return BytesInQue;
}

UINT WINAPI CSerialPort::ListenThread( void* pParam )
{
	/** �õ������ָ�� */ 
	CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);
	
	/** ��ȡ���뻺�����е����ݲ������ʾ */
	char cRecved = 0x00;
	accum = -1;
	//char command;
//        char length = 0;
//	char tagNum;
//	char num;
	//char accLen = 0;
	//char commandLength = 0;
        if(buffer != NULL){
            delete []buffer;
        }
	buffer = new char[1000];
	// �߳�ѭ��,��ѯ��ʽ��ȡ��������
	while (!pSerialPort->s_bExit) 
	{
		UINT BytesInQue = pSerialPort->GetBytesInCOM();
		/** ����������뻺������������,����Ϣһ���ٲ�ѯ */ 
		/** ���Inventory��־λΪtrue����һ�ֽ����ٽ���*/
		//if ( BytesInQue == 0 && flag1 ==false)
		if ( BytesInQue == 0)
		{
			Sleep(SLEEP_TIME_INTERVAL);
			continue;
                }

		do
		{
			cRecved = 0x00;
			if(pSerialPort->ReadChar(cRecved))
			{	
				accum++;
                                buffer[accum] = cRecved;
			}
		}while(--BytesInQue);
		//accLen+=(m+1);
		//cout<<"BytesInQue"<<BytesInQue<<endl;
		/*��ʹbreakʣ����ֽڻ����ڴ�����ȡ��*/
		//UINT BytesInQue1 = pSerialPort->GetBytesInCOM();
		//if(m == length -1 && invFlag ==false){
		//	m = -1;
		//	flag = true;
		//}
		//if(invFlag ==true){
		//	if(accLen<buffer[1])
		//		continue;
		//	tagNum =buffer[2];
		//	for(;tagNum>0;tagNum--){
		//		//length = buffer[1];
		//		commandLength+=buffer[1+commandLength];
		//		num = buffer[2+commandLength];
		//		if((tagNum-1) !=num)
		//			break;
		//	}
		//	
		//	if((tagNum-1) != num &&(tagNum!=0&&num!=0))
		//		continue;
		//	//if(commandLength <m+1){
		//	if(accLen<commandLength)
		//		continue;
		//	m=-1;
		//	flag=true;
		//}

	}
	return 0;
}


//UINT WINAPI CSerialPort::ListenThread( void* pParam )
//{
//        /** �õ������ָ�� */
//        CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);

//        /** ��ȡ���뻺�����е����ݲ������ʾ */
//        char cRecved = 0x00;
//        accum = -1;
//        if(buffer != NULL){
//            delete []buffer;
//        }
//        buffer = new char[1000];
//        // �߳�ѭ��,��ѯ��ʽ��ȡ��������
//        while (!pSerialPort->s_bExit)
//        {
//                UINT BytesInQue = pSerialPort->GetBytesInCOM();
//                /** ����������뻺������������,����Ϣһ���ٲ�ѯ */
//                /** ���Inventory��־λΪtrue����һ�ֽ����ٽ���*/
//                //if ( BytesInQue == 0 && flag1 ==false)
//                if ( BytesInQue == 0)
//                {
//                        Sleep(SLEEP_TIME_INTERVAL);
//                        continue;
//                }

//                do
//                {
//                        cRecved = 0x00;
//                        if(pSerialPort->ReadChar(cRecved))
//                        {
//                                accum++;
//                                buffer[accum] = cRecved;
//                        }
//                }while(--BytesInQue);
//        }
//        return 0;
//}
//void CSerialPort::flush(){
//	m=-1;
//	rdLen = 0;
//}
//
//bool CSerialPort::read(int beg, int len, char arr[]){
//	while(len<m+1)
//		Sleep(1);
//	//arr = new char[len];
//	for(int i=beg;i<beg+len;i++)
//		arr[i] = buffer[i+rdLen];
//	rdLen+=len;
//}

bool CSerialPort::ReadChar( char &cRecved )//����cRecved������
{
	BOOL  bResult     = TRUE;
	DWORD BytesRead   = 0;
	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** �ٽ������� */ 
	EnterCriticalSection(&m_csCommunicationSync);

	/** �ӻ�������ȡһ���ֽڵ����� */ 
	bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);
	if ((!bResult))
	{ 
		/** ��ȡ������,���Ը��ݸô�����������ԭ�� */ 
                //DWORD dwError = GetLastError();

		/** ��մ��ڻ����� */ 
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** �뿪�ٽ��� */ 
	LeaveCriticalSection(&m_csCommunicationSync);

	return (BytesRead == 1);

}

bool CSerialPort::WriteData( unsigned char* pData, unsigned int length )
{
	//for(int i=0;i<sizeof(buffer);i++)
	//	buffer[i] ='\0';
	BOOL   bResult     = TRUE;
	DWORD  BytesToSend = 0;
	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** �ٽ������� */ 
	EnterCriticalSection(&m_csCommunicationSync);

	/** �򻺳���д��ָ���������� */ 
	bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);

	if (!bResult)  
	{
                //DWORD dwError = GetLastError();
		/** ��մ��ڻ����� */ 
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** �뿪�ٽ��� */ 
	LeaveCriticalSection(&m_csCommunicationSync);


	return true;
}


