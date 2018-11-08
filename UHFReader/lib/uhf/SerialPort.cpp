//////////////////////////////////////////////////////////////////////////
/// COPYRIGHT NOTICE
/// Copyright (c) 2011, 上海交通大学 物联网实验室  （版权声明）
/// All rights reserved.
///
/// @file    SerialPort.cpp  
/// @brief   串口通信类的实现文件
///
/// 本文件为串口通信类的实现代码
///
/// @version 1.0   
/// @author  陈杰 
/// @E-mail：chjie@sjtu.edu.cn
/// @date    2011/04/19
///
///  修订说明：
//////////////////////////////////////////////////////////////////////////

#include "SerialPort.h"
#include <process.h>
#include <iostream>

using namespace std;

/** 线程退出标志 */ 
bool CSerialPort::s_bExit = false;
///** heartBeat线程退出标志 */
//bool CSerialPort::s_bHBExit = false;
/** 当串口无数据时,sleep至下次查询间隔的时间,单位:秒 */ 
const UINT SLEEP_TIME_INTERVAL = 5;
//char buffer[100];

char *buffer;
int accum;
//bool ocpMark;
//bool flag = false;
//bool invFlag = false;

CSerialPort::CSerialPort(const char *portName)
: m_hListenThread(INVALID_HANDLE_VALUE)//类给成员变量赋值
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

CSerialPort::~CSerialPort(void)//对应构造函数，称为析构函数，在对象销毁时调用
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

	/** 临时变量,将制定参数转化为字符串形式,以构造DCB结构 */ 
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

	/** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */ 
    if (!openPort(portName))
	{
		return false;
	}

	/** 进入临界段 */ 
	EnterCriticalSection(&m_csCommunicationSync);

	/** 是否有错误发生 */ 
	BOOL bIsSuccess = TRUE;

    /** 在此可以设置输入输出的缓冲区大小,如果不设置,则系统会设置默认值.
	 *  自己设置缓冲区大小时,要注意设置稍大一些,避免缓冲区溢出
	 */
	/*if (bIsSuccess )
	{
		bIsSuccess = SetupComm(m_hComm,10,10);
	}*/

	/** 设置串口的超时时间,均设为0,表示不使用超时限制 */
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
		// 将ANSI字符串转换为UNICODE字符串
		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNum] ;
		if (!MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
		{
			bIsSuccess = TRUE;
		}

		/** 获取当前串口配置参数,并且构造串口DCB参数 */ 
		bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb) ;
		/** 开启RTS flow控制 */ 
		dcb.fRtsControl = RTS_CONTROL_ENABLE; 

		/** 释放内存空间 */ 
		delete [] pwText;
	}

	if ( bIsSuccess )
	{
		/** 使用DCB参数配置串口状态 */ 
		bIsSuccess = SetCommState(m_hComm, &dcb);
	}
		
	/**  清空串口缓冲区 */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/** 离开临界段 */ 
	LeaveCriticalSection(&m_csCommunicationSync);

	return bIsSuccess==TRUE;
}

bool CSerialPort::InitPort( const char *portName ,const LPDCB& plDCB )
{
	/** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */ 
    if (!openPort(portName))
	{
		return false;
	}
	
	/** 进入临界段 */ 
	EnterCriticalSection(&m_csCommunicationSync);

	/** 配置串口参数 */ 
	if (!SetCommState(m_hComm, plDCB))
	{
		return false;
	}

	/**  清空串口缓冲区 */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/** 离开临界段 */ 
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

void CSerialPort::ClosePort()
{
	/** 如果有串口被打开，关闭它 */
	if( m_hComm != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hComm );
		m_hComm = INVALID_HANDLE_VALUE;
	}
}

bool CSerialPort::openPort( const char *portName )
{
	/** 进入临界段 */ 
	EnterCriticalSection(&m_csCommunicationSync);

	/** 打开指定的串口 */ 
        m_hComm = CreateFileA(portName,		                /** 设备名,COM1,COM2等 */
						 GENERIC_READ | GENERIC_WRITE,  /** 访问模式,可同时读写 */   
						 0,                             /** 共享模式,0表示不共享 */ 
					     NULL,							/** 安全性设置,一般使用NULL */ 
					     OPEN_EXISTING,					/** 该参数表示设备必须存在,否则创建失败 */ 
                                                 0,
						 0);    

	/** 如果打开失败，释放资源并返回 */ 
        if (m_hComm == INVALID_HANDLE_VALUE || m_hComm == ((HANDLE)-1))
	{
                o_eCode = GetLastError();
		LeaveCriticalSection(&m_csCommunicationSync);
//                cout<<"exit3"<<endl;
		return false;
	}

	/** 退出临界区 */ 
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

bool CSerialPort::OpenListenThread()
{
	/** 检测线程是否已经开启了 */ 
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** 线程已经开启 */ 
		return false;
	}

	s_bExit = false;
	/** 线程ID */ 
	UINT threadId;
	/** 开启串口数据监听线程 */ 
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);
	if (!m_hListenThread)
	{
		return false;
	}

	/** 设置线程的优先级,高于普通线程 */ 
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
		/** 通知线程退出 */ 
		s_bExit = true;

		/** 等待线程退出 */ 
		Sleep(10);

		/** 置线程句柄无效 */ 
		CloseHandle( m_hListenThread );
		m_hListenThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

//bool CSerialPort::OpenHeartBeatThread()
//{
//        /** 检测线程是否已经开启了 */
//        if (m_hHeartBeatThread != INVALID_HANDLE_VALUE)
//        {
//                /** 线程已经开启 */
//                return false;
//        }

//        s_bExit = false;
//        /** 线程ID */
//        UINT threadId;
//        /** 开启串口数据监听线程 */
//        m_hHeartBeatThread = (HANDLE)_beginthreadex(NULL, 0, HeartBeatThread, this, 0, &threadId);
//        if (!m_hHeartBeatThread)
//        {
//                return false;
//        }

//        /** 设置线程的优先级,高于普通线程 */
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
//                /** 通知线程退出 */
//                s_bHBExit = true;

//                /** 等待线程退出 */
//                Sleep(10);

//                /** 置线程句柄无效 */
//                CloseHandle( m_hHeartBeatThread );
//                m_hHeartBeatThread = INVALID_HANDLE_VALUE;
//        }
//        return true;
//}

UINT CSerialPort::GetBytesInCOM()
{
	DWORD dwError = 0;	/** 错误码 */ 
	COMSTAT  comstat;   /** COMSTAT结构体,记录通信设备的状态信息 */ 
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	/** 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 */ 
	if ( ClearCommError(m_hComm, &dwError, &comstat) )
	{
		BytesInQue = comstat.cbInQue; /** 获取在输入缓冲区中的字节数 */ 
	}

	return BytesInQue;
}

UINT WINAPI CSerialPort::ListenThread( void* pParam )
{
	/** 得到本类的指针 */ 
	CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);
	
	/** 读取输入缓冲区中的数据并输出显示 */
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
	// 线程循环,轮询方式读取串口数据
	while (!pSerialPort->s_bExit) 
	{
		UINT BytesInQue = pSerialPort->GetBytesInCOM();
		/** 如果串口输入缓冲区中无数据,则休息一会再查询 */ 
		/** 如果Inventory标志位为true，等一轮结束再接受*/
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
		/*即使break剩余的字节还会在串口中取得*/
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
//        /** 得到本类的指针 */
//        CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);

//        /** 读取输入缓冲区中的数据并输出显示 */
//        char cRecved = 0x00;
//        accum = -1;
//        if(buffer != NULL){
//            delete []buffer;
//        }
//        buffer = new char[1000];
//        // 线程循环,轮询方式读取串口数据
//        while (!pSerialPort->s_bExit)
//        {
//                UINT BytesInQue = pSerialPort->GetBytesInCOM();
//                /** 如果串口输入缓冲区中无数据,则休息一会再查询 */
//                /** 如果Inventory标志位为true，等一轮结束再接受*/
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

bool CSerialPort::ReadChar( char &cRecved )//这里cRecved是引用
{
	BOOL  bResult     = TRUE;
	DWORD BytesRead   = 0;
	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** 临界区保护 */ 
	EnterCriticalSection(&m_csCommunicationSync);

	/** 从缓冲区读取一个字节的数据 */ 
	bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);
	if ((!bResult))
	{ 
		/** 获取错误码,可以根据该错误码查出错误原因 */ 
                //DWORD dwError = GetLastError();

		/** 清空串口缓冲区 */ 
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */ 
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

	/** 临界区保护 */ 
	EnterCriticalSection(&m_csCommunicationSync);

	/** 向缓冲区写入指定量的数据 */ 
	bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);

	if (!bResult)  
	{
                //DWORD dwError = GetLastError();
		/** 清空串口缓冲区 */ 
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */ 
	LeaveCriticalSection(&m_csCommunicationSync);


	return true;
}


