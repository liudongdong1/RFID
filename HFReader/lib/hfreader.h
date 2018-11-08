#ifndef HFREADER_H
#define HFREADER_H
#include <windows.h>

class HFReader
{
private:
    int bp;
    int protocol;
    int buzzerTime;
    int buzzerCount;
    int ledTime;
    int ledCount;
    char comPort[10];
    BYTE deviceAddress;
    HANDLE    hComm;
    HINSTANCE m_hInstMaster;
    int LoadDll();
    int CloseDll();
    int OpenComm(char *com, int bp);
    int CloseComm();
public:
    HFReader(char *com, int bp);
    HFReader(char *com, int bp, int protocol);
    ~HFReader();
    void buzzer();
    void led();
    int getBP();
    void setBP(int bp);
    int getProtocol();
    void setProtocol(int protocol);
    int setBaudrate(BYTE baudrate);
    int getBuzzerTime();
    int getBuzzerCount();
    int getLedTime();
    int getLedCount();
    void setBuzzerTime(int buzzerTime);
    void setBuzzerCount(int buzzerCount);
    void setLedTime(int ledTime);
    void setLedCount(int ledCount);
    int setDeviceID(BYTE id, BYTE &newVal);
    BYTE getDeviceID();
    int setSnr(BYTE *snr);
    int getSnr(BYTE *snr);
    int readUserInfo(int num_blk, int num_length, unsigned char *user_info);
    int writeUserInfo(int num_blk, int num_length, unsigned char *user_info);
    int getVersionNum(unsigned char *versionNum);



    int ISO15693_Scan(int &cardNum, BYTE **buffer);
    int ISO15693_Read(unsigned char flags, int blk_add, int num_blk,
                                                   unsigned char *uid,
                                                   unsigned char *buffer);
    int ISO15693_Write(unsigned char flags, int blk_add, int num_blk,
                                                   unsigned char *uid,
                                                   unsigned char *data);


    int ISO15693_Lock(BYTE flags, int num_blk, BYTE *uid, BYTE *buffer);


    int ISO15693_StayQuiet(BYTE *uid, BYTE *buffer);

    int ISO15693_Select(BYTE *uid, BYTE *buffer);

    int ISO15693_ResetToReady(BYTE flags, BYTE *uid, BYTE *buffer);


    int ISO15693_WriteAFI(BYTE flags, BYTE afi, BYTE *uid, BYTE *buffer);

    int ISO15693_LockAFI(BYTE flags, BYTE *uid, BYTE *buffer);

    int ISO15693_WriteDSFID(BYTE flags, BYTE DSFID, BYTE *uid, BYTE *buffer);

    int ISO15693_LockDSFID(BYTE flags, BYTE *uid, BYTE *buffer);

    int ISO15693_GetSystemInfo(BYTE flags, BYTE *uid, BYTE *buffer);


    int ISO15693_GetMultiSecurity(BYTE flags, BYTE blk_add, BYTE num_blk, BYTE *uid, BYTE *buffer);

    int ISO15693_TransCOSCmd(BYTE* cmd, int cmdSize, BYTE *buffer);


    /************************* ISO 14443A API **************************************/
      // REQA发送寻卡指令.
    int ISO1443A_REQ(BYTE inf_mode, BYTE *buffer);

      //Anticoll 防冲突
     int ISO1443A_Anticoll(BYTE *snr, BYTE &status);

     //  Select 选择卡
     int ISO1443A_Select(BYTE *snr);

     // Halt 中断卡  选择卡，使卡进入被中断的状态…
     int ISO1443A_Halt();

     int ISO1443A_Restore(BYTE add_blk);

    /******************* MIFARE High Level Functions ******************************/
     int ISO1443A_Read(BYTE mode, BYTE blk_add, BYTE num_blk, BYTE *snr, BYTE *buffer);

     int ISO1443A_Write(BYTE mode, BYTE blk_add, BYTE num_blk, BYTE *snr, BYTE *buffer);

     int ISO1443A_InitVal(BYTE mode, BYTE SectNum, BYTE *snr, int *value);

     int ISO1443A_Dec(BYTE mode, BYTE SectNum, BYTE *snr, int *value);

     int ISO1443A_Inc(BYTE mode, BYTE SectNum, BYTE *snr, int *value);

     int ISO1443A_GET_SNR(BYTE mode, BYTE halt, BYTE *snr, BYTE *value);

     int ISO1443A_TransCOSCmd(unsigned char *cmd, int cmdSize, unsigned char *buffer);

     /************************* ISO 14443B API **************************************/

     int ISO1443B_REQ(unsigned char *buffer);

     int ISO1443B_Anticoll(unsigned char *buffer);
     int ISO1443B_Select(unsigned char *SerialNum);

     int ISO1443B_ReqAttribute(unsigned char *buffer);

     int ISO1443B_TransCOSCmd(unsigned char *cmd, int cmdSize, unsigned char *buffer);

};

#endif // HFREADER_H
