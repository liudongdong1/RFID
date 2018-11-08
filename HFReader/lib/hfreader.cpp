#include "hfreader.h"
#include "mi.h"
#include "iostream"
#include "reader/ui/util/readertype.h"

using namespace std;

HFReader::HFReader(char *com, int bp)
{
    m_hInstMaster = NULL;
    hComm = NULL;
    if(!LoadDll()){
        cout<<"load dll failed!"<<endl;
        return;
    }

    OpenComm(com, bp);
    strcpy(this->comPort, com);
    this->bp = bp;

    cout<<"comPort="<<comPort<<endl;
    cout<<"bp="<<bp<<endl;
    this->protocol = HF_ISO15693;
    deviceAddress = 0x00;
    ledTime = 16;
    ledCount = 3;
    buzzerTime = 16;
    buzzerCount = 3;
}

HFReader::HFReader(char *com, int bp, int protocol){
    HFReader(com, bp);
    this->protocol = protocol;

}

HFReader::~HFReader()
{
    if(hComm)
            CloseComm();
    CloseDll();
}

int HFReader::LoadDll()
{
    m_hInstMaster = LoadLibrary(L"mi.dll");	// Loaded 'E:\EXAMEX4\RMD\mi.dll', no matching symbolic information found.
    cout<<m_hInstMaster<<endl;
    if(!m_hInstMaster)
    {
        return 0;
    }
    cout<<m_hInstMaster<<endl;
    cout<<"load dll"<<endl;

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
                return 0;
        }
  /* */
    return -1;
}
int HFReader::CloseDll()
{
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
int HFReader::OpenComm(char *com,int bp)
{
    cout<<"open com"<<com<<"\t"<<bp<<endl;
        if(hComm)
                return -1;
        hComm = API_OpenComm(com, bp);
        cout<<"api open com"<<endl;
        if(!hComm)
                return 2;
        buzzer();
        return 0;
}
int HFReader::CloseComm()
{
        if(hComm)
              API_CloseComm(hComm);
        hComm = NULL;
        return -1;
}



/*
int HFReader::inventory(int &cardNum, BYTE *buffer){
    BYTE buf[256];
    BYTE num[6];
    int result = 0;
    memset(buf,0,256);
    memset(num,0,6);

    if((result = ISO15693_Inventory(hComm, deviceAddress,
                                    num,
                                    buf) != 0x00)){
        return result;
    }

    cardNum = (int)num[0];
    for(int i=0;i<10*cardNum;i++){
        buffer[i] = buf[i];
    }

    cout<<hex<<(int)num[0]<<" "<<(int)num[1]<<" "<<(int)num[2]<<" "<<(int)num[3]<<endl;
    cout<<hex<<(int)buffer[0]<<" "<<(int)buffer[1]<<" "<<(int)buffer[2]<<" "<<(int)buffer[3]<<" "<<(int)buffer[4]<<" "<<(int)buffer[5]<<" "<<(int)buffer[6]<<" "<<(int)buffer[7]<<" "<<(int)buffer[8]<<" "<<(int)buffer[9]<<endl;
    cout<<hex<<(int)buffer[10]<<" "<<(int)buffer[11]<<" "<<(int)buffer[12]<<" "<<(int)buffer[13]<<" "<<(int)buffer[14]<<" "<<(int)buffer[15]<<" "<<(int)buffer[16]<<" "<<(int)buffer[17]<<" "<<(int)buffer[18]<<" "<<(int)buffer[19]<<endl;
    cout<<hex<<(int)buffer[20]<<" "<<(int)buffer[21]<<" "<<(int)buffer[22]<<" "<<(int)buffer[23]<<" "<<(int)buffer[24]<<" "<<(int)buffer[25]<<" "<<(int)buffer[26]<<" "<<(int)buffer[27]<<" "<<(int)buffer[28]<<" "<<(int)buffer[29]<<endl;

    return 0;
}
*/


int HFReader::ISO15693_Scan(int &cardNum, BYTE **buffer){
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


int HFReader::ISO15693_Read(unsigned char flags, int blk_add, int num_blk,
                                               unsigned char *uid,
                                               unsigned char *buffer){

    if (!hComm)
            return -1;

    return API_ISO15693Read(hComm, deviceAddress, flags, blk_add, num_blk, uid, buffer);

}



int HFReader::ISO15693_Write(unsigned char flags, int blk_add, int num_blk,
                                               unsigned char *uid,
                                               unsigned char *data){

    if (!hComm)
            return -1;

    return API_ISO15693Write(hComm, deviceAddress, flags, blk_add, num_blk, uid, data);
}

int HFReader::ISO15693_Lock(BYTE flags, int num_blk, BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return API_ISO15693Lock(hComm, deviceAddress, flags, num_blk, uid, buffer);
}


int HFReader::ISO15693_StayQuiet(BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return ISO15693StayQuiet(hComm, deviceAddress, 0x22, uid, buffer);

}

int HFReader::ISO15693_Select(BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return ISO15693Select(hComm, deviceAddress, 0x22, uid, buffer);
}

int HFReader::ISO15693_ResetToReady(BYTE flags, BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return ResetToReady(hComm, deviceAddress, flags, uid, buffer);

}


int HFReader::ISO15693_WriteAFI(BYTE flags, BYTE afi, BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return WriteAFI(hComm, deviceAddress, flags, afi, uid, buffer);

}

int HFReader::ISO15693_LockAFI(BYTE flags, BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return LockAFI(hComm, deviceAddress, flags, uid, buffer);

}

int HFReader::ISO15693_WriteDSFID(BYTE flags, BYTE DSFID, BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return WriteDSFID(hComm, deviceAddress, flags, DSFID, uid, buffer);

}

int HFReader::ISO15693_LockDSFID(BYTE flags, BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return LockDSFID(hComm, deviceAddress, flags, uid, buffer);
}

int HFReader::ISO15693_GetSystemInfo(BYTE flags, BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return ISO15693_GetSysInfo(hComm, deviceAddress, flags, uid, buffer);
}


int HFReader::ISO15693_GetMultiSecurity(BYTE flags, BYTE blk_add, BYTE num_blk, BYTE *uid, BYTE *buffer){
    if (!hComm)
            return -1;

    return ISO15693_GetMulSecurity(hComm, deviceAddress, flags, blk_add, num_blk, uid, buffer);
}

int HFReader::ISO15693_TransCOSCmd(BYTE* cmd, int cmdSize, BYTE *buffer){
    if (!hComm)
            return -1;

    return API_ISO15693TransCOSCmd(hComm, deviceAddress, cmd, cmdSize, buffer);

}

/************************* ISO 14443A API **************************************/
int HFReader::ISO1443A_REQ(BYTE inf_mode, BYTE *buffer){
    if (!hComm)
            return -1;

    return MF_Request(hComm, deviceAddress, inf_mode, buffer);
}

int HFReader::ISO1443A_Anticoll(BYTE *snr, BYTE &status){
    if (!hComm)
            return -1;

    return MF_Anticoll(hComm, deviceAddress, snr, status);
}

int HFReader::ISO1443A_Select(BYTE *snr){
    if (!hComm)
            return -1;

    return MF_Select(hComm, deviceAddress, snr);
}

int HFReader::ISO1443A_Halt(){
    if (!hComm)
            return -1;

    return MF_Halt(hComm, deviceAddress);
}

int HFReader::ISO1443A_Restore(BYTE add_blk){
    if (!hComm)
            return -1;

    return MF_Restore(hComm, deviceAddress, add_blk);
}

/******************* MIFARE High Level Functions ******************************/
int HFReader::ISO1443A_Read(BYTE mode, BYTE blk_add, BYTE num_blk, BYTE *snr, BYTE *buffer){
    if (!hComm)
            return -1;

    return API_PCDRead(hComm, deviceAddress, mode, blk_add, num_blk, snr, buffer);
}

int HFReader::ISO1443A_Write(BYTE mode, BYTE blk_add, BYTE num_blk, BYTE *snr, BYTE *buffer){
    if (!hComm)
            return -1;

    return API_PCDWrite(hComm, deviceAddress, mode, blk_add, num_blk, snr, buffer);
}

int HFReader::ISO1443A_InitVal(BYTE mode, BYTE SectNum, BYTE *snr, int *value){
    if (!hComm)
            return -1;

    return API_PCDInitVal(hComm, deviceAddress, mode, SectNum, snr, value);
}

int HFReader::ISO1443A_Dec(BYTE mode, BYTE SectNum, BYTE *snr, int *value){
    if (!hComm)
            return -1;

    return API_PCDDec(hComm, deviceAddress, mode, SectNum, snr, value);
}

int HFReader::ISO1443A_Inc(BYTE mode, BYTE SectNum, BYTE *snr, int *value){
    if (!hComm)
            return -1;

    return API_PCDInc(hComm, deviceAddress, mode, SectNum, snr, value);
}

int HFReader::ISO1443A_GET_SNR(BYTE mode, BYTE halt, BYTE *snr, BYTE *value){
    if (!hComm)
            return -1;
    return GET_SNR(hComm, deviceAddress, mode, halt, snr, value);
}

int HFReader::ISO1443A_TransCOSCmd(unsigned char *cmd, int cmdSize, unsigned char *buffer){
    if (!hComm)
            return -1;

    return API_ISO14443TypeATransCOSCmd(hComm, deviceAddress, cmd, cmdSize, buffer);
}

 /************************* ISO 14443B API **************************************/

int HFReader::ISO1443B_REQ(unsigned char *buffer){
    if (!hComm)
            return -1;

    return RequestType_B(hComm, deviceAddress, buffer);
}

int HFReader::ISO1443B_Anticoll(unsigned char *buffer){
    if (!hComm)
            return -1;

    return AntiType_B(hComm, deviceAddress, buffer);
}

int HFReader::ISO1443B_Select(unsigned char *SerialNum){
    if (!hComm)
            return -1;

    return SelectType_B(hComm, deviceAddress, SerialNum);
}

int HFReader::ISO1443B_ReqAttribute(unsigned char *buffer){
    if (!hComm)
            return -1;

    return Request_AB(hComm, deviceAddress, buffer);
}

int HFReader::ISO1443B_TransCOSCmd(unsigned char *cmd, int cmdSize, unsigned char *buffer){
    if (!hComm)
            return -1;

    return API_ISO14443TypeBTransCOSCmd(hComm, deviceAddress, cmd, cmdSize, buffer);
}

/*
int HFReader::scan(int &cardNum, BYTE *buffer)
{
    if(protocol == HF_ISO15693){
        if (!hComm)
                return -1;

        BYTE buf[256];
        BYTE num[6];
        int result = 0;
        memset(buf,0,256);
        memset(num,0,6);

        if((result = ISO15693_Inventory(hComm, deviceAddress,
                                        num,
                                        buf) != 0x00)){
            return result;
        }

        cardNum = (int)num[0];
        for(int i=0;i<10*cardNum;i++){
            buffer[i] = buf[i];
        }

        return 0;
    }
    else if(protocol == HFC_ISO1443A){
        //TODO
        /*if((result = GET_SNR(hComm, deviceAddress, 0x26, deviceAddress,
                snr,buf)) != 0x00)
                return 2;
        return 0;
    }
    else if(protocol == HFC_ISO1443B){
        //TODO
        return 0;
    }

}*/


void HFReader::buzzer()
{
        if(!hComm)
                return ;
        BYTE buf[128];
        int re = API_ControlBuzzer(hComm,deviceAddress,buzzerTime,buzzerCount,buf);

}


void HFReader::led()
{
        if(!hComm)
                return ;

        //TODO
        //BYTE buf[128];
        //int re = API_ControlBuzzer(hComm,DeviceAddress,buzzerTime,buzzerCount,buf);

}

int HFReader::getSnr(BYTE *snr)
{
        if(!hComm)
                return -1;
        BYTE buf[9];
        memset(buf,1,9);

        int re = 0;
        if((re = API_GetSerNum(hComm,deviceAddress,buf)) == 0)
        {
               memcpy(snr, buf, 9);
               return 0;
        }
        else{
               return -1;
        }
}


int HFReader::setSnr(BYTE *snr){      //set返回的buffer第一个字节是80，表示设置成功，而非新内容
    if(!hComm)
            return -1;
    BYTE buf[8];
    memset(buf,1,8);

    return API_SetSerNum(hComm,deviceAddress,snr,buf);
}


int HFReader::readUserInfo(int num_blk, int num_length, unsigned char *user_info)
{
    if (!hComm)
            return -1;

    return ReadUserInfo(hComm, deviceAddress, num_blk, num_length, user_info);
}

int HFReader::writeUserInfo(int num_blk, int num_length, unsigned char *user_info)
{
    if (!hComm)
            return -1;

    return WriteUserInfo(hComm, deviceAddress, num_blk, num_length, user_info);
}

int HFReader::getVersionNum(unsigned char *versionNum)
{
    if (!hComm)
            return -1;
    return GetVersionNum(hComm, deviceAddress, versionNum);

}


int HFReader::setDeviceID(BYTE id, BYTE &newVal){
    if(!hComm)
            return -1;

    BYTE buf[1];
    memset(buf,1,1);
    if(API_SetDeviceAddress(hComm, deviceAddress, id, buf)==0){
        newVal = buf[0];
        return 0;
    }
    else{
        return -1;
    }
}
BYTE HFReader::getDeviceID(){
    BYTE *snr = new BYTE[9];
    if(getSnr(snr)!=0){
        return -1;
    }
    BYTE re = snr[0];
    delete snr;
    return re;
}
int HFReader::getBP(){
    return bp;
}
int HFReader::getProtocol(){
    return protocol;
}
int HFReader::setBaudrate(BYTE baudrate){
    if(!hComm)
            return -1;
    BYTE buf[8];
    memset(buf,1,8);
    cout<<(int)baudrate<<endl;
    if(API_SetBandrate(hComm, deviceAddress, baudrate, buf)==0){
        switch(baudrate){
        case 0x00:
            setBP(9600);
            break;
        case 0x01:
            setBP(19200);
            break;

        case 0x02:
            setBP(38400);
            break;

        case 0x03:
            setBP(57600);
            break;

        case 0x04:
            setBP(115200);
            break;

        }
        cout<<"reopen"<<endl;
        cout<<"com="<<comPort<<endl;
        cout<<"bp="<<bp<<endl;
        /* reopen com due to baudrate change */
        if(hComm){
            cout<<CloseComm()<<endl;
        }
        OpenComm(this->comPort, this->bp);
        return 0;
    }
    else{
        return -1;
    }
}

void HFReader::setBP(int bp){
    this->bp = bp;
}
void HFReader::setProtocol(int protocol){
    this->protocol = protocol;
}


int HFReader::getBuzzerTime(){
    return buzzerTime;
}
int HFReader::getBuzzerCount(){
    return buzzerCount;
}
int HFReader::getLedTime(){
    return ledTime;
}
int HFReader::getLedCount(){
    return ledCount;
}
void HFReader::setBuzzerTime(int buzzerTime){
    this->buzzerTime = buzzerTime;
}
void HFReader::setBuzzerCount(int buzzerCount){
    this->buzzerCount = buzzerCount;
}
void HFReader::setLedTime(int ledTime){
    this->ledTime = ledTime;
}
void HFReader::setLedCount(int ledCount){
    this->ledCount = ledCount;
}
