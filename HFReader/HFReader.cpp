// HFReader.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>
#include "lib\mi.h"

#include <iostream>

using namespace std;

HANDLE    hComm;
HINSTANCE m_hInstMaster;
BYTE deviceAddress = 0x00;

int loadDLL(){
	m_hInstMaster = LoadLibrary(L"mi.dll");;
	if(!m_hInstMaster)
    {
        return -1;
    }


	(FARPROC &)API_OpenComm   = GetProcAddress(m_hInstMaster,"API_OpenComm");
    (FARPROC &)API_CloseComm  = GetProcAddress(m_hInstMaster,"API_CloseComm");
    (FARPROC &)API_SetDeviceAddress   = GetProcAddress(m_hInstMaster,"API_SetDeviceAddress");
    (FARPROC &)API_SetBandrate        = GetProcAddress(m_hInstMaster,"API_SetBandrate");
    (FARPROC &)API_ControlLED      = GetProcAddress(m_hInstMaster,"API_ControlLED");
    (FARPROC &)API_ControlBuzzer      = GetProcAddress(m_hInstMaster,"API_ControlBuzzer");
    (FARPROC &)API_GetSerNum          = GetProcAddress(m_hInstMaster,"API_GetSerNum");
    (FARPROC &)API_SetSerNum          = GetProcAddress(m_hInstMaster,"API_SetSerNum");
    (FARPROC &)ReadUserInfo          = GetProcAddress(m_hInstMaster,"ReadUserInfo");
    (FARPROC &)WriteUserInfo          = GetProcAddress(m_hInstMaster,"WriteUserInfo");
    (FARPROC &)GetVersionNum          = GetProcAddress(m_hInstMaster,"GetVersionNum");

    (FARPROC &)ISO15693_Inventory = GetProcAddress(m_hInstMaster,"ISO15693_Inventory");
    (FARPROC &)API_ISO15693Read  = GetProcAddress(m_hInstMaster,"API_ISO15693Read");
    (FARPROC &)API_ISO15693Write  = GetProcAddress(m_hInstMaster,"API_ISO15693Write");
    (FARPROC &)API_ISO15693Lock  = GetProcAddress(m_hInstMaster,"API_ISO15693Lock");
    (FARPROC &)ISO15693StayQuiet  = GetProcAddress(m_hInstMaster,"ISO15693StayQuiet");
    (FARPROC &)ISO15693Select  = GetProcAddress(m_hInstMaster,"ISO15693Select");
    (FARPROC &)ResetToReady  = GetProcAddress(m_hInstMaster,"ResetToReady");
    (FARPROC &)WriteAFI  = GetProcAddress(m_hInstMaster,"WriteAFI");
    (FARPROC &)LockAFI  = GetProcAddress(m_hInstMaster,"LockAFI");
    (FARPROC &)WriteDSFID  = GetProcAddress(m_hInstMaster,"WriteDSFID");
    (FARPROC &)LockDSFID  = GetProcAddress(m_hInstMaster,"LockDSFID");
    (FARPROC &)ISO15693_GetSysInfo  = GetProcAddress(m_hInstMaster,"ISO15693_GetSysInfo");
    (FARPROC &)ISO15693_GetMulSecurity  = GetProcAddress(m_hInstMaster,"ISO15693_GetMulSecurity");
    (FARPROC &)API_ISO15693TransCOSCmd  = GetProcAddress(m_hInstMaster,"API_ISO15693TransCOSCmd");


    (FARPROC &)MF_Request     = GetProcAddress(m_hInstMaster,"MF_Request");
    (FARPROC &)MF_Anticoll    = GetProcAddress(m_hInstMaster,"MF_Anticoll");
    (FARPROC &)MF_Select      = GetProcAddress(m_hInstMaster,"MF_Select");
    (FARPROC &)MF_Halt        = GetProcAddress(m_hInstMaster,"MF_Halt");
    (FARPROC &)MF_Restore     = GetProcAddress(m_hInstMaster,"MF_Restore");

    (FARPROC &)API_PCDRead    = GetProcAddress(m_hInstMaster,"API_PCDRead");
    (FARPROC &)API_PCDWrite   = GetProcAddress(m_hInstMaster,"API_PCDWrite");
    (FARPROC &)API_PCDInitVal = GetProcAddress(m_hInstMaster,"API_PCDInitVal");
    (FARPROC &)API_PCDDec     = GetProcAddress(m_hInstMaster,"API_PCDDec");
    (FARPROC &)API_PCDInc     = GetProcAddress(m_hInstMaster,"API_PCDInc");
    (FARPROC &)GET_SNR        = GetProcAddress(m_hInstMaster,"GET_SNR");
    (FARPROC &)API_ISO14443TypeATransCOSCmd = GetProcAddress(m_hInstMaster,"API_ISO14443TypeATransCOSCmd");



    (FARPROC &)RequestType_B  = GetProcAddress(m_hInstMaster,"RequestType_B");
    (FARPROC &)AntiType_B     = GetProcAddress(m_hInstMaster,"AntiType_B");
    (FARPROC &)SelectType_B   = GetProcAddress(m_hInstMaster,"SelectType_B");
    (FARPROC &)Request_AB     = GetProcAddress(m_hInstMaster,"Request_AB");
    (FARPROC &)API_ISO14443TypeBTransCOSCmd = GetProcAddress(m_hInstMaster,"API_ISO14443TypeBTransCOSCmd");


    //(FARPROC &)RDM_GetSnr     = GetProcAddress(m_hInstMaster,"RDM_GetSnr");


    if( API_OpenComm      == NULL ||
        API_CloseComm     == NULL ||
        API_SetDeviceAddress     == NULL ||
        API_SetBandrate   == NULL ||
        API_ControlLED    == NULL ||
        API_ControlBuzzer == NULL ||
        API_GetSerNum     == NULL ||
        API_SetSerNum     == NULL ||
        ReadUserInfo      == NULL ||
        WriteUserInfo     == NULL ||
        GetVersionNum     == NULL ||

        ISO15693_Inventory == NULL ||
        API_ISO15693Read   == NULL ||
        API_ISO15693Write  == NULL ||
        API_ISO15693Lock   == NULL ||
        ISO15693StayQuiet  == NULL ||
        ISO15693Select     == NULL ||
        ResetToReady       == NULL ||
        WriteAFI           == NULL ||
        LockAFI            == NULL ||
        WriteDSFID         == NULL ||
        LockDSFID          == NULL ||
        ISO15693_GetSysInfo      == NULL ||
        ISO15693_GetMulSecurity  == NULL ||
        API_ISO15693TransCOSCmd  == NULL ||

        MF_Request        == NULL ||
        MF_Anticoll       == NULL ||
        MF_Select         == NULL ||
        MF_Halt           == NULL ||
        MF_Restore        == NULL ||

        API_PCDRead       == NULL ||
        API_PCDWrite      == NULL ||
        API_PCDInitVal    == NULL ||
        API_PCDDec        == NULL ||
        API_PCDInc        == NULL ||
        GET_SNR           == NULL ||
        API_ISO14443TypeATransCOSCmd == NULL ||


        RequestType_B     == NULL ||
        AntiType_B        == NULL ||
        SelectType_B      == NULL ||
        Request_AB        == NULL ||
        API_ISO14443TypeBTransCOSCmd  == NULL
        //RDM_GetSnr        == NULL ||
         )
        {
			cout<<"pro failed"<<endl;
                return 0;
        }
	return -1;
}

int unloadDLL(){
	if(m_hInstMaster)
    {
        FreeLibrary(m_hInstMaster);
        API_OpenComm      = NULL;
        API_CloseComm     = NULL;
        API_SetDeviceAddress     = NULL;
        API_SetBandrate   = NULL;
        API_ControlLED    = NULL;
        API_ControlBuzzer = NULL;
        API_GetSerNum     = NULL;
        API_SetSerNum     = NULL;
        ReadUserInfo      = NULL;
        WriteUserInfo     = NULL;
        GetVersionNum     = NULL;

        ISO15693_Inventory = NULL;
        API_ISO15693Read   = NULL;
        API_ISO15693Write  = NULL;
        API_ISO15693Lock   = NULL;
        ISO15693StayQuiet  = NULL;
        ISO15693Select     = NULL;
        ResetToReady       = NULL;
        WriteAFI           = NULL;
        LockAFI            = NULL;
        WriteDSFID         = NULL;
        LockDSFID          = NULL;
        ISO15693_GetSysInfo      = NULL;
        ISO15693_GetMulSecurity  = NULL;
        API_ISO15693TransCOSCmd  = NULL;

        MF_Request        = NULL;
        MF_Anticoll       = NULL;
        MF_Select         = NULL;
        MF_Halt           = NULL;
        MF_Restore        = NULL;

        API_PCDRead       = NULL;
        API_PCDWrite      = NULL;
        API_PCDInitVal    = NULL;
        API_PCDDec        = NULL;
        API_PCDInc        = NULL;
        GET_SNR           = NULL;
        API_ISO14443TypeATransCOSCmd           = NULL;


        RequestType_B     = NULL;
        AntiType_B        = NULL;
        SelectType_B      = NULL;
        Request_AB        = NULL;
        API_ISO14443TypeBTransCOSCmd = NULL;
        return -1;
    }
    return 0;
}

int openComm(char *com,int bp){
	if(hComm)
            return -1;
    hComm = API_OpenComm(com, bp);
    if(!hComm)
            return 2;
    return 0;
}

int closeComm(){
	if(hComm)
            API_CloseComm(hComm);
    hComm = NULL;
    return -1;
}


int ISO15693_Scan(int &cardNum, BYTE **buffer){
	BYTE buf[256];
    BYTE num[1];
    int result = 0;
    memset(buf, 0x00, 256);
    memset(num, 0x00, 1);

    if (!hComm)
            return -1;

    if((result = ISO15693_Inventory(hComm, deviceAddress,
                                    num,
                                    buf) != 0x00)){
        return result;
    }

    cardNum = (int)num[0];

    for(int i=0;i<cardNum;i++){
        buffer[i][0] = buf[i*10];
        buffer[i][1] = buf[i*10+1];
        for(int j=0;j<8;j++){
             buffer[i][j+2] = buf[i*10+9-j];
        }

    }

    return 0;
}


int ISO15693_Read(unsigned char flags, int blk_add, int num_blk,
                                               unsigned char *uid,
                                               unsigned char *buffer){

    if (!hComm)
            return -1;

    return API_ISO15693Read(hComm, deviceAddress, flags, blk_add, num_blk, uid, buffer);

}



int ISO15693_Write(unsigned char flags, int blk_add, int num_blk,
                                               unsigned char *uid,
                                               unsigned char *data){

    if (!hComm)
            return -1;

    return API_ISO15693Write(hComm, deviceAddress, flags, blk_add, num_blk, uid, data);
}

int ISO15693_Lock(BYTE flags, int num_blk, BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return API_ISO15693Lock(hComm, deviceAddress, flags, num_blk, uid, buffer);
}

int ISO1443A_GET_SNR(BYTE mode, BYTE halt, BYTE *snr, BYTE *value){
    if (!hComm)
            return -1;
    return GET_SNR(hComm, deviceAddress, mode, halt, snr, value);
}

int ISO1443A_Read(BYTE mode, BYTE blk_add, BYTE num_blk, BYTE *snr, BYTE *buffer){
    if (!hComm)
            return -1;

    return API_PCDRead(hComm, deviceAddress, mode, blk_add, num_blk, snr, buffer);
}

int ISO1443A_Write(BYTE mode, BYTE blk_add, BYTE num_blk, BYTE *snr, BYTE *buffer){
    if (!hComm)
            return -1;

    return API_PCDWrite(hComm, deviceAddress, mode, blk_add, num_blk, snr, buffer);
}

void print(unsigned char* str, int length){
	char *c;
	for(int i=0;i<length;i++){
		c = new char[3];
		sprintf(c, "%02X", str[i]);
		c[2] = '\0';
		cout<<c<<" ";
		delete c;
	}
}


int ISO1443A_InitVal(BYTE mode, BYTE SectNum, BYTE *snr, int *value){
    if (!hComm)
            return -1;

    return API_PCDInitVal(hComm, deviceAddress, mode, SectNum, snr, value);
}

int ISO1443A_Dec(BYTE mode, BYTE SectNum, BYTE *snr, int *value){
    if (!hComm)
            return -1;

    return API_PCDDec(hComm, deviceAddress, mode, SectNum, snr, value);
}

int ISO1443A_Inc(BYTE mode, BYTE SectNum, BYTE *snr, int *value){
    if (!hComm)
            return -1;

    return API_PCDInc(hComm, deviceAddress, mode, SectNum, snr, value);
}

int init(BYTE sect, BYTE* buffer){
	BYTE *key = new BYTE[6];
	key[0] = 0xFF;
	key[1] = 0xFF;
	key[2] = 0xFF;
	key[3] = 0xFF;
	key[4] = 0xFF;
	key[5] = 0xFF;


    int *val = new int[4];
	val[0] = 0;
	val[1] = 0;
	val[2] = 0;
	val[3] = 0;
    BYTE *valByte = (BYTE *)val;
    valByte[3] = buffer[0];
    valByte[2] = buffer[1];
    valByte[1] = buffer[2];
    valByte[0] = buffer[3];


    if(ISO1443A_InitVal(0x00, sect, key, val) == 0){
		return 0;
	}

	return -1;
}

int increment(BYTE sect, BYTE* buffer){
	BYTE *key = new BYTE[6];
	key[0] = 0xFF;
	key[1] = 0xFF;
	key[2] = 0xFF;
	key[3] = 0xFF;
	key[4] = 0xFF;
	key[5] = 0xFF;

	int *val = new int[4];
	val[0] = 0;
	val[1] = 0;
	val[2] = 0;
	val[3] = 0;
    
	BYTE *valByte = (BYTE *)val;
    valByte[12] = buffer[0];
    valByte[8] = buffer[1];
    valByte[4] = buffer[2];
    valByte[0] = buffer[3];

	 if(ISO1443A_Inc(0x00, sect, key, val) == 0){
		return 0;
	}

	 return -1;
}

int decrement(BYTE sect, BYTE* buffer){
	BYTE *key = new BYTE[6];
	key[0] = 0xFF;
	key[1] = 0xFF;
	key[2] = 0xFF;
	key[3] = 0xFF;
	key[4] = 0xFF;
	key[5] = 0xFF;

	int *val = new int[4];
	val[0] = 0;
	val[1] = 0;
	val[2] = 0;
	val[3] = 0;
    
	BYTE *valByte = (BYTE *)val;
    valByte[12] = buffer[0];
    valByte[8] = buffer[1];
    valByte[4] = buffer[2];
    valByte[0] = buffer[3];

	if(ISO1443A_Dec(0x00, sect, key, val) == 0){
		return 0;
	}
	return -1;
}

void readValue(BYTE sect){
	BYTE *key = new BYTE[6];
	key[0] = 0xFF;
	key[1] = 0xFF;
	key[2] = 0xFF;
	key[3] = 0xFF;
	key[4] = 0xFF;
	key[5] = 0xFF;

	BYTE blk_num = sect * 4 + 1;
    BYTE *buffer2 = new BYTE[16];
	if(ISO1443A_Read(0x00, blk_num, 0x01, key, buffer2) == 0){
		cout<<"read:";
		print(buffer2, 4);
		cout<<endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	loadDLL();

	//if(openComm("\\\\.\\COM16", 9600) != 0){
	if(openComm("COM8", 9600) != 0){
		cout<<"open comm failed!"<<endl;
	}

	int num = 0;
    BYTE **cardIDs = new BYTE*[16];
    for(int i=0;i<16;i++){
        cardIDs[i] = new BYTE[10];
    }

	cout<<"HF 15693:"<<endl;
	if(ISO15693_Scan(num, cardIDs) == 0){
		for(int i=0;i<num;i++){
			cout<<"card ID:";
			print(cardIDs[i]+2, 8);
			

            cout<<"AFI = ";
			print(cardIDs[i], 1);
			

			cout<<"DSFID = ";
			print(cardIDs[i]+1, 1);
			cout<<endl;
			
        }
	}
	
    BYTE *cardNum = new BYTE[1];
    BYTE *cardID = new BYTE[4];

	cout<<"HF 14443A:"<<endl;
	if(ISO1443A_GET_SNR(0x52, 0x00, cardNum, cardID) == 0){
		cout<<"card id:";
		print(cardID, 4);
		cout<<endl;

	}


	BYTE *buffer = new BYTE[4];
	buffer[0] = 0x11;
	buffer[1] = 0x22;
	buffer[2] = 0x33;
	buffer[3] = 0x44;
	if(init(0x03, buffer) == 0){
		cout<<"init value: ";
		print(buffer, 4);
		cout<<endl;
	}

	readValue(0x03);
	cout<<endl;
	
	buffer[0] = 0x22;
	buffer[1] = 0x22;
	buffer[2] = 0x22;
	buffer[3] = 0x22;

	if(increment(0x03, buffer) == 0){
		cout<<"increment value:";
		print(buffer, 4);
		cout<<endl;
	}
	
	readValue(0x03);
	cout<<endl;

	buffer[0] = 0x11;
	buffer[1] = 0x11;
	buffer[2] = 0x11;
	buffer[3] = 0x11;

	if(decrement(0x03, buffer) == 0){
		cout<<"decrement value:";
		print(buffer, 4);
		cout<<endl;
	}

	readValue(0x03);
	cout<<endl;

	return 0;
}

