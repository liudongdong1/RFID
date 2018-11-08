//////////////////////////////////////////////////////////////////////////
/// COPYRIGHT NOTICE
/// Copyright (c) 2011, 上海交通大学 物联网实验室  （版权声明）
/// All rights reserved.
/// 
/// @file    CommandLib.h  
/// @brief   读写器库的头文件
///
/// 
/// @version 1.0   
/// @author  陈杰 
/// @E-mail：chjie@sjtu.edu.cn
/// @date    2011/04/19
/// 
///
///  修订说明：
//////////////////////////////////////////////////////////////////////////

#ifndef COMMANDLIB_CL_
#define COMMANDLIB_CL_
#include <string>
#include "SerialPort.h"
#include <vector>
#include <Windows.h>

#define FM0     0x00
#define Miller2 0x01
#define Miller4 0x02
#define Miller8 0x03

#define S0      0x00
#define S1      0x01
#define S2      0x02
#define S3      0x03
#define SL      0x04

using namespace std;
class CommandLib{
private:
    CSerialPort *mySerialPort;

public:
CommandLib(const char *portName);
~CommandLib();

//get the Reader FIRMID Version
int getFirmID(unsigned char* &firmID);

//get the Reader HARDWID Version
int getHardWID(unsigned char* &hardWID);

//PA Contorl
//int setOutAntPowOff(char *retbuffer);
int setOutAntPowOff();
//int setOutAntPowOn(char *retbuffer);
int setOutAntPowOn();

//changer the output level
int changeOutputLevel(int outLev, unsigned char &v);

int getOutputLevel(int &value);

int getOutputLevel(int &value, unsigned char &v);

//write the regs
int writeReg(unsigned char regAdd, int regLen, unsigned char* &regDate);

//read the regs
int readReg(unsigned char regAdd, unsigned char* &regData);

//inquire the inventory
//int getInventory(vector<vector<char>  > *retbuffer);
int getInventory(unsigned char** &retbuffer, int &tagNum, int* &length);
//int getInventory(char ***retbuffer, int &tagNum);

//select the very tag
/*tested*/
int selectTag(unsigned char* EPC, unsigned char length);

//write the tag
//格式为密码后是写入数据长度，以字为单位
/*tested*/
int writeTag(unsigned char mem, unsigned char begAdd, unsigned char* &psw, unsigned char* &data, unsigned char dataLen);

/*tested*/
int setEPC(unsigned char* &psw, unsigned char* &data, unsigned char dataLen);

int setPsw(unsigned char* psw, unsigned char* newPsw, bool mark);

int setUser(unsigned char begAdd, unsigned char* psw, unsigned char* &data);
//read the tag
int readTag(unsigned char mem, unsigned char begAdd, unsigned char &readLen, unsigned char* &tagInfo);

int readTagOnPsw(unsigned char mem, unsigned char begAdd, unsigned char* psw, unsigned char &readLen, unsigned char* &tagInfo);

int readEPC(unsigned char &readLen, unsigned char* &tagInfo);

int readCompleteEPC(unsigned char &readLen, unsigned char* &tagInfo);

int readTID(unsigned char &readLen, unsigned char* &tagInfo);

int readCompleteTID(unsigned char &readLen, unsigned char* &tagInfo);

int* readReserved(unsigned char* &killPwd, unsigned char* &accessPwd);

int* readReservedOnPsw(unsigned char* psw, unsigned char* &killPwd, unsigned char* &accessPwd);

int readUser(unsigned char &reeadLen, unsigned char* &tagInfo, unsigned char begAdd = 0x00);
//lock & unlock the tag
int lockUnlock(unsigned char op, unsigned char mem, unsigned char* &psw);

//kill tag
int killTag(unsigned char* &psw);


//get Inventory with RSSI
//三维指针指向二维数组，EPC[tagNum][EPCLen],freq[tagNum][3]
//freq的3列分别表示low，mid，high
int getInventoryRSSI(unsigned char** &EPC, int &tagNum, int* &EPCLen, int* &intRSSI,int* &RSSI_I, int* &RSSI_Q, unsigned char** &freq);

//get Regs Complete
int getRegsComplete(unsigned char* &retbuffer);

//set & get GEN2 Settings
//int setGen2Settings(int linkFreqInt, char miller, char session, char trext, char Q, int sens,
//                                int &_linkFreq, char &_miller, char &_session, char &_trext, char &_Q, int &_sens);
int setGen2Settings(int linkFreqInt, unsigned char miller, unsigned char session, unsigned char trext, unsigned char Q,
                                int &_linkFreq, unsigned char &_miller, unsigned char &_session, unsigned char &_trext, unsigned char &_Q);
int setSensSetting(int sens, int &_sens);
int getSensSetting(int &_sens);
int getGen2Settings(int &linkFreq, unsigned char &miller, unsigned char &session, unsigned char &trext, unsigned char &Q, int &sens);

int getFreqParam(unsigned char &activePro,UINT &lis,UINT &maxAlloc,UINT &idle, UINT &startFreq, UINT &endFreq, double &inc, int &rssi);

int setUserFreq(double startFreq, double endFreq, double inc, int RSSI, UINT idle, UINT lis, UINT maxAlloc, unsigned char activeProfile = 0x00);
//set USAFreq Profile
//begins from 902.750 to 927.250
//RSSI threshhold -40
//listen time 1ms,Idle Time 0,Increment 0.5,Maximum Allocation 400
int setEurFreq();
int setJapFreq();
int setUSAFreq();
int setCHN920Freq();
int setCHN840Freq();
int setKorFreq();

int getEurFreq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc);
int getJapFreq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc);
int getUSAFreq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc);
int getCHN920Freq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc);
int getCHN840Freq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc);
int getKorFreq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc);


void hexConversion(UINT dec, unsigned char hex[]);

//Decimal To Complement
//void decToComp(int dec, char hexComp);
void decToComplement(int dec, unsigned char &hexComp);

//get SerialNum
//int getSerialPortsReg(char (*pListStr)[80]);
//int getSerialPortsDevM(char **pListStr);

//set Reader Frequency
int setChangerFreq(unsigned char mask, unsigned char* &freq, char RSSI, unsigned char activePro);

//mask = 0x10 设置LBT参数
int setLBT(UINT intLis, UINT intSend, UINT intIdle);

private:


void decToBin(UINT dec, int bin[]);

int binToHex(int bin[], char hexStr[]);

void hexstrToHex(char hexStr[], unsigned char hex[]);

//mask = 0x11 返回频率设置相关信息
//int getFreqPara(unsigned char* &retbuffer);
int getFreqPara(char* &retbuffer);

//int getSerialPortsReg(char (*pListStr)[80]);
bool read(int beg, int len, unsigned char* &arr);
void flush();
void add(int len);
};
#endif //COMMANDLIB_CL_
