/*
* File:         SRAPIV2.H
* Created:      03/09/2001
* Description:  The Function Prototype Header File for SRAPIV2.DLL
*					 (The API for Serial Reader (SR100, SR120, SR950 .. etc)
*
*****************************************************************************
*
* Version | Date     | Description
* --------+----------+------------------------------------------------
* V1.02   | 03/12/01 | Date type of the parameter Value in the MIFARE

*********************************************************************************/

/******************* System Functions ******************************/


//1. Set communciation baudrate.
 HANDLE (WINAPI * API_OpenComm)(char *com,int Baudrate);

//2. To close communication port.
 int (WINAPI * API_CloseComm)(HANDLE commHandle);

 //设置通讯地址
 int (WINAPI *  API_SetDeviceAddress)(                                    HANDLE commHandle,
                                                                          int DeviceAddress,
                                                                          BYTE newAddr,
                                                                          BYTE *buffer);
 //计算机的比特率
 int (WINAPI * API_SetBandrate)(                                    HANDLE commHandle,
                                                                    int DeviceAddress,
                                                                    BYTE newBaud,
                                                                    BYTE *buffer);
 //设置灯的工作状态，包括，灯亮的周期以及循环的次数
 int (WINAPI *  API_ControlLED)( HANDLE commHandle,
                                                                   int DeviceAddress,
                                                                   BYTE freq,
                                                                   BYTE duration,
                                                                   BYTE *buffer);
 //设置蜂鸣器的工作状态，包括，蜂鸣器的周期以及循环的次数
 int (WINAPI *  API_ControlBuzzer)( HANDLE commHandle,
                                                                   int DeviceAddress,
                                                                   BYTE freq,
                                                                   BYTE duration,
                                                                   BYTE *buffer);

 //读取由厂家预设的1个字节的读卡器地址和8个字节序列号
 int (WINAPI *  API_GetSerNum)(
                                                           HANDLE commHandle,
                                                           int DeviceAddress,
                                                           BYTE *buffer);
 //设置由厂家预设的1个字节的读卡器地址和8个字节序列号
 int (WINAPI *  API_SetSerNum)(
                                                           HANDLE commHandle,
                                                           int DeviceAddress,
                                                           BYTE *newValue,
                                                           BYTE *buffer);

int (WINAPI *  ReadUserInfo)(HANDLE commHandle,
                                    int DeviceAddress,
                                    int num_blk,
                                    int num_length,
                                    unsigned char *user_info);

int (WINAPI *  WriteUserInfo)(HANDLE    commHandle,
                    int           DeviceAddress,
                    int           num_blk,
                    int           num_length,
                    unsigned char          *user_info);



 /* invalid, need to check */
 int (WINAPI *  GetVersionNum)(HANDLE          commHandle,
                                                           int DeviceAddress,
                                                           unsigned char *VersionNum);





 /***********************  ISO 15693 API ************************************/

  //ISO15693寻卡
  /*int (WINAPI * ISO15693_Inventory)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE *snr,
                                                          BYTE *buffer);*/

  int (WINAPI * ISO15693_Inventory)(HANDLE commHandle, int DeviceAddress,
                                                           BYTE *nrOfCard,
                                                           BYTE *buffer);

  //ISO15693读取数据
  int (WINAPI * API_ISO15693Read)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          int blk_add,
                                                          int num_blk,
                                                          BYTE *uid,
                                                          BYTE *buffer);

  //ISO15693写数据
  int (WINAPI * API_ISO15693Write)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          int blk_add,
                                                          int num_blk,
                                                          BYTE *uid,
                                                          BYTE *data);



  int (WINAPI * API_ISO15693Lock)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          int num_blk,
                                                          BYTE *uid,
                                                          BYTE *buffer);


  int (WINAPI * ISO15693StayQuiet)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          BYTE *uid,
                                                          BYTE *buffer);

  int (WINAPI * ISO15693Select)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          BYTE *uid,
                                                          BYTE *buffer);

  int (WINAPI * ResetToReady)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          BYTE *uid,
                                                          BYTE *buffer);


  int (WINAPI * WriteAFI)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          BYTE afi,
                                                          BYTE *uid,
                                                          BYTE *buffer);

  int (WINAPI * LockAFI)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          BYTE *uid,
                                                          BYTE *buffer);

  int (WINAPI * WriteDSFID)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          BYTE DSFID,
                                                          BYTE *uid,
                                                          BYTE *buffer);

  int (WINAPI * LockDSFID)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          BYTE *uid,
                                                          BYTE *buffer);

  int (WINAPI * ISO15693_GetSysInfo)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          BYTE *uid,
                                                          BYTE *buffer);


  int (WINAPI * ISO15693_GetMulSecurity)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE flags,
                                                          int blk_add,
                                                          int num_blk,
                                                          BYTE *uid,
                                                          BYTE *buffer);

 int (WINAPI * API_ISO15693TransCOSCmd)(HANDLE commHandle, int DeviceAddress,
                                                          BYTE* cmd,
                                                          int cmdSize,
                                                          BYTE *buffer);



/************************* ISO 14443A API **************************************/
  // REQA发送寻卡指令.
  int (WINAPI * MF_Request)(HANDLE commHandle,
          int DeviceAddress,
          BYTE inf_mode,
          BYTE *buffer);

  //Anticoll 防冲突
  int (WINAPI *  MF_Anticoll)(HANDLE commHandle,
                                                    int DeviceAddress,
                                                    BYTE *snr,
                                                    BYTE &status) ;

 //  Select 选择卡
 int (WINAPI *  MF_Select)(HANDLE commHandle,
                                                   int DeviceAddress,
                                                   BYTE *snr);

 // Halt 中断卡  选择卡，使卡进入被中断的状态…
 int (WINAPI *  MF_Halt)(HANDLE commHandle,
                                                 int DeviceAddress);

 int (WINAPI *  MF_Restore)(HANDLE commHandle, int DeviceAddress,
                            BYTE add_blk);



/******************* MIFARE High Level Functions ******************************/
 int (WINAPI * API_PCDRead)(HANDLE commHandle, int DeviceAddress, BYTE mode,
                                                        BYTE blk_add, BYTE num_blk,
                                                        BYTE *snr, BYTE *buffer);

 int (WINAPI * API_PCDWrite)(HANDLE commHandle, int DeviceAddress, BYTE mode,
                                                        BYTE blk_add, BYTE num_blk,
                                                        BYTE *snr, BYTE *buffer);

 int (WINAPI * API_PCDInitVal)(HANDLE commHandle, int DeviceAddress,BYTE mode,
                                                         BYTE SectNum,
                                                         BYTE *snr, int *value);

 int (WINAPI * API_PCDDec)(HANDLE commHandle, int DeviceAddress, BYTE mode,
                                                   BYTE SectNum,
                                                   BYTE *snr, int *value);

 int (WINAPI * API_PCDInc)(HANDLE commHandle, int DeviceAddress, BYTE mode,
                                                   BYTE SectNum,
                                                   BYTE *snr, int *value);


 int (WINAPI *  GET_SNR)(HANDLE  commHandle,
                                                 int DeviceAddress,
                                                 BYTE mode,
                                                 BYTE halt,
                                                 BYTE *snr,
                                                 BYTE *value);

 int  (WINAPI *  API_ISO14443TypeATransCOSCmd)(HANDLE commHandle,
                                         int  DeviceAddress,
                                         unsigned char *cmd,
                                         int cmdSize,
                                         unsigned char *buffer);

 /************************* ISO 14443B API **************************************/

 int  (WINAPI *  RequestType_B)(HANDLE commHandle,
                                         int  DeviceAddress,
                                         unsigned char   *buffer);

 int  (WINAPI *  AntiType_B)(HANDLE commHandle,
                                         int  DeviceAddress,
                                         unsigned char   *buffer);
 int  (WINAPI *  SelectType_B)(HANDLE commHandle,
                                         int  DeviceAddress,
                                         unsigned char   *SerialNum);

 int  (WINAPI *  Request_AB)(HANDLE commHandle,
                                         int  DeviceAddress,
                                         unsigned char   *buffer);


 int  (WINAPI *  API_ISO14443TypeBTransCOSCmd)(HANDLE commHandle,
                                         int  DeviceAddress,
                                         unsigned char *cmd,
                                         int cmdSize,
                                         unsigned char *buffer);
/*


 //自动读取卡Ultralight的卡号
 int (WINAPI * UL_Request)(HANDLE commHandle, int DeviceAddress,BYTE mode,
                                                         BYTE *snr);


 //读取卡中指定扇区的数据
 int (WINAPI * UL_HLRead)(HANDLE commHandle, int DeviceAddress,BYTE mode,
                                                         BYTE blk_Addr,
                                                         BYTE *snr,
                                                         BYTE *buffer);

 //写取卡中指定扇区的数据
 int (WINAPI * UL_HLWrite)(HANDLE commHandle, int DeviceAddress,BYTE mode,
                                                         BYTE blk_Addr,
                                                         BYTE *snr,
                                                         BYTE *buffer);



  int (WINAPI * RDM_GetSnr)(HANDLE commHandle, int deviceAddress,
                                 BYTE *pCardNo);



 */
