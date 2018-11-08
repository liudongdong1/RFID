//////////////////////////////////////////////////////////////////////////
/// COPYRIGHT NOTICE
/// Copyright (c) 2011, 上海交通大学 物联网实验室  （版权声明）
/// All rights reserved.
///
/// @file    CommandLib.cpp
/// @brief   读写器库的主文件
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

#include "CommandLib.h"
#include "SerialPort.h"
#include <string>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

extern char *buffer;
//extern bool flag;
extern bool invFlag;
//extern int invGloabal;
extern int accum;
int rdLen;

CommandLib::CommandLib(const char *portName){
    mySerialPort = new CSerialPort(portName);
}

CommandLib::~CommandLib(){
    delete mySerialPort;
}

/**
*以下每条指令都需要先将flag置false
*flag = false;
*while(flag==false);
*等到port数据读完之后会在SerialPort.cpp中置true，此时buffer中有数值
*/

/**
*单条指令实现_Start
*/
//get FirmID
//@para : index长度，**firmID指向内容地址
/*<tested>*/
/*正确返回1，错误都返回0*/
int CommandLib::getFirmID(unsigned char* &firmID)
{
    unsigned char sendBytes[] = {0x10,0x03,0};
    mySerialPort->WriteData(sendBytes,3);

	unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            flush();
            delete [] a;
            a =NULL;
            return 0;
        }
	int length = a[1];
        unsigned char* rec = new unsigned char[length+1];
        firmID = new unsigned char[length-1];
        rec[0] = a[0];
        rec[1] = length;
        if(!read(2,length-2,rec)){
            delete [] a;
            a = NULL;
            delete [] rec;
            rec = NULL;
            flush();
            return 0;
        }
        strncpy((char*) firmID, (char*)(rec+2),length-2);
        firmID[length-2] = '\0';
        delete [] a;
        a =NULL;
        delete [] rec;
        rec = NULL;
	flush();
    return 1;
}

//get Hardware ID
//@para : index长度，**hardWID指向内容地址
/*<tested>*/
//int CommandLib::getHardWID(std::string &hardWID)
/*正确返回1，错误都返回0*/
int CommandLib::getHardWID(unsigned char* &hardWID)
{
//    flag = false;
    unsigned char sendBytes[] = {0x10,0x03,0x01};
    mySerialPort->WriteData(sendBytes,3);
 	unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
//            cout<<"123"<<endl;
            flush();
            delete [] a;
            a = NULL;
            return 0;
        }
	int length = a[1];
        unsigned char* rec = new unsigned char[length+1];
        hardWID = new unsigned char[length-1];
        rec[0] = a[0];
        rec[1] = length;
        if(!read(2,length-2,rec)){
            delete [] a;
            a = NULL;
            delete [] rec;
            rec = NULL;
            flush();
            return 0;
        }
        strncpy((char*) hardWID, (char*)(rec+2),length-2);
        hardWID[length-2] = '\0';
        delete [] a;
        a = NULL;
        delete [] rec;
        rec = NULL;
	flush();
    return 1;
}

//int CommandLib::setOutAntPowOff(unsigned char *retbuffer)
/*<tested>*/
/*正确返回1，错误都返回0*/
int CommandLib::setOutAntPowOff()
{
//	flag = false;
        unsigned char sendBytes[] = {0x18,0x03,0x00};
        mySerialPort->WriteData(sendBytes,3);
        unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            flush();
            delete [] a;
            a =NULL;
            return 0;
        }
        int length = a[1];
        unsigned char* rec = new unsigned char[length];
        rec[0]=a[0];
        rec[1]=length;
        if(!read(2,length-2,rec)){
           delete [] a;
           a = NULL;
           delete [] rec;
           rec = NULL;
           flush();
           return 0;
        }
        delete [] a;
        a = NULL;
        flush();
        if(rec[0]==0x19&&rec[2]==0){
            delete [] rec;
            rec = NULL;
            return 1;
        }
        delete [] rec;
        rec = NULL;
	return 0;
}

//int CommandLib::setOutAntPowOn(unsigned char *retbuffer)
	/*<tested>*/
/*正确返回1，错误都返回0*/
int CommandLib::setOutAntPowOn()
{
        unsigned char sendBytes[] = {0x18,0x03,0xff};
        mySerialPort->WriteData(sendBytes,3);
        unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            flush();
            delete [] a;
            a =NULL;
            return 0;
        }
        int length = a[1];
        unsigned char* rec = new unsigned char[length];
        rec[0]=a[0];
        rec[1]=length;
        if(!read(2,length-2,rec)){
           delete [] a;
           a = NULL;
           delete [] rec;
           rec = NULL;
           flush();
           return 0;
        }
        delete []a;
        flush();
        if(rec[0]==0x19&&rec[2]==0){
            delete [] rec;
            rec = NULL;
            return 1;
        }
        delete [] rec;
        rec = NULL;
	return 0;
}

//write the registers
	/*<tested>*/
/*return error code 仅实现正确返回1，错误都返回0*/
int CommandLib::writeReg(unsigned char regAdd, int regLen, unsigned char* &regData)
{
        unsigned char *sendBytes = new unsigned char[regLen + 3];
        sendBytes[0] = 0x1A;
        sendBytes[1] = regLen + 3;
        sendBytes[2] = regAdd;
        for(int i=0;i<regLen;i++){
            sendBytes[3 + i] = regData[i];
        }
        mySerialPort->WriteData(sendBytes, regLen + 3);
        delete [] sendBytes;
        unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            flush();
            delete [] a;
            a =NULL;
            return 0;
        }
        int length = a[1];
        unsigned char* rec = new unsigned char[length];
        rec[0]=a[0];
        rec[1]=length;
        if(!read(2,length-2,rec)){
           delete [] a;
           a = NULL;
           delete [] rec;
           rec = NULL;
           flush();
           return 0;
        }
        delete []a;
        flush();
        if(rec[2]==0){
            delete [] rec;
            rec = NULL;
            return 1;
        }
        delete [] rec;
        rec = NULL;
        return 0;
}

//这里涉及到设计问题；regLen对读没有影响，取的时候应该用到regLen.
/*<tested>*/
/*正确返回1，错误都返回0*/
int CommandLib::readReg(unsigned char regAdd, unsigned char* &regData)
{
        unsigned char sendBytes[] ={0x1C,0x03,regAdd};
        mySerialPort->WriteData(sendBytes,3);
       	unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            flush();
            delete [] a;
            a =NULL;
            return 0;
        }
        int length = a[1];
        unsigned char* rec = new unsigned char[length];
        rec[0]=a[0];
        rec[1]=length;
        if(!read(2,length-2,rec)){
           delete [] a;
           a = NULL;
           delete [] rec;
           rec = NULL;
           flush();
           return 0;
        }
        delete [] a;
        a = NULL;
        flush();
        if(rec[0]!=0x1D){
            delete [] rec;
            rec = NULL;
            return 0;
        }
        regData =rec+2;
        //TODO: delete [] rec??
        return 1;
}

/**
*使用Vector变长数组，行数不固定，列数固定
*注意要delete
*/
/**
*这里是用变长二维数组实现*/
	/*<tested>*/
/*正确返回1，错误都返回0*/
int CommandLib::getInventory(unsigned char** &retbuffer, int &tagNum, int* &length){
        unsigned char sendBytes[] ={0x31,0x03,0x01};
        mySerialPort->WriteData(sendBytes,3);
        unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            flush();
            delete [] a;
            a =NULL;
            return 0;
        }
        int len = a[1];
        unsigned char* rec = new unsigned char[len];
        rec[0]=a[0];
        rec[1]=len;
        if(!read(2,len-2,rec)){
           delete [] a;
           a = NULL;
           delete [] rec;
           rec = NULL;
           flush();
           return 0;
        }
        tagNum = rec[2];
        //char* tagNumCopy = new char ();
        //strcpy(tagNumCopy,(char *)buffer+2);
		//*tagNumCopy	= *(buffer+2);
		//tagNum = *tagNumCopy;
		//if(tagNum ==2)
        //cout<<"TagNUM: "<<tagNum<<endl;
	if(tagNum==0)
	{
		printf("%s","没读到！");
                delete []a;
                a = NULL;
                delete []rec;
                rec = NULL;
                flush();
		return 0;
	}
    retbuffer = new unsigned char*[tagNum];
	//length returns the true EPC length
    length = new int[tagNum];
    unsigned char EPCLen = rec[3]-2;
    length[0] =EPCLen;
    retbuffer[0] = new unsigned char[EPCLen];
    for(int j=0;j<EPCLen;j++)
        retbuffer[0][j] = rec[6+j];
    add(len);
    delete []rec;
    rec = NULL;
    for(int i=1;i<tagNum;i++){
        if(!read(0,2,a)){
            flush();
            delete [] a;
            a =NULL;
            return 0;
        }
     len = a[1];
     unsigned char* rec = new unsigned char[len];
     rec[0]=a[0];
     rec[1]=len;
     if(!read(2,len-2,rec)){
        delete [] a;
        a = NULL;
        delete [] rec;
        rec = NULL;
        flush();
        return 0;
     }
        EPCLen = rec[3]-2;
        length[i] = EPCLen;
        retbuffer[i] = new unsigned char[EPCLen];
		//for(int j=0;j<buffer[3]-2;j++)
		//	(EPC)[i].push_back(buffer[j+6]);
		//EPCline = EPC[i];
		/*for(int j=0;j<EPCLen;j++){*/
        for(int j=0;j<EPCLen;j++){
            retbuffer[i][j]=rec[j+6];
        }
        add(len);
        delete []rec;
        rec = NULL;
    }
    delete [] a;
    a = NULL;
    delete []rec;
    rec = NULL;
    flush();
    return 1;
	//}
}

/**
*	new之后记得delete
*/
	/*<tested>*/
	/*PS: 调用时如输入数组注意十六进制与十进制区别*/
    /*return error code 正确返回1，错误都返回0*/
int CommandLib::selectTag(unsigned char* EPC, unsigned char length){
        unsigned char *sendBytes = new unsigned char[length+3];
        sendBytes[0] = 0x33;
        sendBytes[1] = length+3;
        sendBytes[2] = length;
	for(int j=0;j<length;j++)
	{
                sendBytes[3+j] = *(EPC+j);
	}
        mySerialPort->WriteData(sendBytes,length+3);
        delete []sendBytes;
        unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            flush();
            delete [] a;
            a =NULL;
            return 0;
        }
        int len = a[1];
        unsigned char* rec = new unsigned char[len];
        rec[0]=a[0];
        rec[1]=len;
        if(!read(2,len-2,rec)){
           delete [] a;
           a = NULL;
           delete [] rec;
           rec = NULL;
           flush();
           return 0;
        }
        delete [] a;
        flush();
	/*cout<<(int)(buffer[2])<<endl;*/
	if(!(rec[2])){
            delete []rec;
            rec = NULL;
            //cout<<"Sel_sccess"<<endl;
            return 1;
        }
        else
             return 0;
}

	/*<tested>*/
/*return error code 函数过程错误返回-1*/
int CommandLib::writeTag(unsigned char mem, unsigned char begAdd, unsigned char* &psw, unsigned char* &data, unsigned char dataLen)
{
        unsigned char *sendBytes = new unsigned char[dataLen+9];
        sendBytes[0] = 0x35;
        sendBytes[1] = dataLen+9;
        sendBytes[2] = mem;
        sendBytes[3] = begAdd;
        sendBytes[4] = psw[0];
        sendBytes[5] = psw[1];
        sendBytes[6] = psw[2];
        sendBytes[7] = psw[3];
        sendBytes[8] = dataLen/2;
	for(int j=0;j<dataLen;j++)
                sendBytes[9+j] = *(data+j);
        mySerialPort->WriteData(sendBytes,dataLen+9);
                delete []sendBytes;

        unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            delete [] a;
            a = NULL;
            flush();
            return -1;
        }
        int len = a[1];
        unsigned char* rec = new unsigned char[len];
        rec[0]=a[0];
        rec[1]=len;
        if(!read(2,len-2,rec)){
            delete [] a;
            a = NULL;
            delete [] rec;
            rec = NULL;
            flush();
            return -1;
        }
        delete [] a;
        a = NULL;
        flush();
        int retCode = rec[2];
        delete [] rec;
        rec = NULL;
        return retCode;
}

	/*tested*/
int CommandLib::setEPC(unsigned char *&psw, unsigned char* &data, unsigned char dataLen){
	int i = CommandLib::writeTag(0x01, 0x02, psw, data, dataLen);
	return i;
}

/*mark=0表示KillPassword， mark=1表示AccessPassword*/
/*tested*/
int CommandLib::setPsw(unsigned char* psw, unsigned char* newPsw,bool mark){
        unsigned char dataLen = 0x04;
	if(mark){
		int i = CommandLib::writeTag(0x00, 0x02,psw,newPsw,dataLen);
			return i;
	}
	else{
		int i = CommandLib::writeTag(0x00, 0x00,psw,newPsw,dataLen);
			return i;
	}
}

/*参数依次为起始地址，密码，修改数据*/
	/*tested*/
int CommandLib::setUser(unsigned char begAdd, unsigned char* psw, unsigned char* &data)
{
        unsigned char dataLen = 0x02;
        unsigned char begAddWord = begAdd/2;
	int i = CommandLib::writeTag(0x03, begAddWord, psw, data, dataLen);
        return i;
}


/**
*返回格式为 reportID，指令长度，错误码，读取长度（单位：字），data
*/
	/*<tested>*/
int CommandLib::readTag(unsigned char mem, unsigned char begAdd, unsigned char &readLen, unsigned char* &tagInfo)
{
        flush();
        unsigned char *sendBytes = new unsigned char[5];
        sendBytes[0] = 0x37;
        sendBytes[1] = 0x05;
        sendBytes[2] = mem;
        sendBytes[3] = begAdd;
        sendBytes[4] = readLen/2;//字长非字节长
        bool re = mySerialPort->WriteData(sendBytes,5);
        delete [] sendBytes;

        unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            delete [] a;
            a = NULL;
            flush();
           return -1;
        }
        int len = a[1];
        unsigned char* rec = new unsigned char[len];
        rec[0]=a[0];
        rec[1]=len;
        if(!read(2,len-2,rec)){
            delete [] a;
            a = NULL;
            delete [] rec;
            rec = NULL;
            flush();
            return -1;
        }
        delete [] a;
        a = NULL;
        flush();
//        flush();
//	if(rec[0] == 0x38 && rec[2] == 0)
//        if(rec[0] == 0x38 && (rec[2]==0x83||!(rec[2])||rec[2]==0x84) )      //TODO:  && (rec[2] == 0||rec[2] == 0x83)
//        {
//            cout<<"rec[3]: "<<(int)rec[3]<<endl;
//            cout<<"rec[4]: "<<(int)rec[4]<<endl;

        int retCode = rec[2];
        if(rec[1]<=4){
            tagInfo = NULL;
            readLen = 0;
        }
        else{
            //int readLen1 = rec[3]*2;//buffer[3] represents the dataLength in words
            int readLen1 = len-4;
            //TODO: rec[3]*2 有时比 len-4大，这里改为len-4
            readLen = readLen1;
            tagInfo = new unsigned char[len-4];
            for(int i=0;i<len-4;i++)
                tagInfo[i] = rec[i+4];
        }

        delete [] rec;
        rec = NULL;
        return retCode;
//            return 0;
//        }
//        int retCode = rec[2];
//        delete [] rec;//留给最后使用的人delete这个指针
//        return retCode;
}

int CommandLib::readTagOnPsw(unsigned char mem, unsigned char begAdd, unsigned char* psw, unsigned char &readLen, unsigned char* &tagInfo){
    flush();
    unsigned char *sendBytes = new unsigned char[5];
    sendBytes[0] = 0x39;
    sendBytes[1] = 0x09;
    sendBytes[2] = mem;
    sendBytes[3] = begAdd;
    sendBytes[4] = psw[0];
    sendBytes[5] = psw[1];
    sendBytes[6] = psw[2];
    sendBytes[7] = psw[3];
    sendBytes[8] = readLen/2;//字长非字节长
    bool re = mySerialPort->WriteData(sendBytes,9);
    delete [] sendBytes;

    unsigned char* a = new unsigned char[2];
    if(!read(0,2,a)){
        delete [] a;
        a = NULL;
        flush();
       return -1;
    }
    int len = a[1];
    unsigned char* rec = new unsigned char[len];
    rec[0]=a[0];
    rec[1]=len;
    if(!read(2,len-2,rec)){
        delete [] a;
        a = NULL;
        delete [] rec;
        rec = NULL;
        flush();
        return -1;
    }
    delete [] a;
    a = NULL;
    flush();
//        flush();
//	if(rec[0] == 0x38 && rec[2] == 0)
//        if(rec[0] == 0x38 && (rec[2]==0x83||!(rec[2])||rec[2]==0x84) )      //TODO:  && (rec[2] == 0||rec[2] == 0x83)
//        {
//            cout<<"rec[3]: "<<(int)rec[3]<<endl;
//            cout<<"rec[4]: "<<(int)rec[4]<<endl;

    int retCode = rec[2];
    if(rec[1]<=4){
        tagInfo = NULL;
        readLen = 0;
    }
    else{
        //int readLen1 = rec[3]*2;//buffer[3] represents the dataLength in words
        int readLen1 = len-4;
        //TODO: rec[3]*2 有时比 len-4大，这里改为len-4
        readLen = readLen1;
        tagInfo = new unsigned char[len-4];
        for(int i=0;i<len-4;i++)
            tagInfo[i] = rec[i+4];
    }

    delete [] rec;
    rec = NULL;
    return retCode;
}

/*Read Only EPC*/
	/*tested*/
int CommandLib::readEPC(unsigned char &readLen, unsigned char* &tagInfo){
	readLen = 0x00;
	int i = CommandLib::readTag(0x01, 0x02, readLen, tagInfo);
	return i;
	//return 1;
}

/*read mem EPC*/
	/*tested*/
int CommandLib::readCompleteEPC(unsigned char &readLen, unsigned char* &tagInfo)
{
	readLen =0x00;
	int i = CommandLib::readTag(0x01, 0x00, readLen, tagInfo);
	return i;
	//readLen = sizeof(tagInfo);
	//return 1;
}

/*readLength Known*/
	/*tested*/
int CommandLib::readTID(unsigned char &readLen, unsigned char* &tagInfo){
	int i = CommandLib::readTag(0x02, 0x00, readLen, tagInfo);
	return i;
	//return 1;
}

/*readLength Unknown*/
	/*tested*/
int CommandLib::readCompleteTID(unsigned char &readLen, unsigned char* &tagInfo)
{
	readLen =0x00;
	int i = CommandLib::readTID(readLen,tagInfo);
	return i;
	//readLen = sizeof(tagInfo);
	//return 1;
}

	/*tested*/
int* CommandLib::readReserved(unsigned char* &killPwd, unsigned char* &accessPwd)
{
        unsigned char readLen = 0x04;
        int *i = new int[2];
        i[0] = CommandLib::readTag(0x00, 0x00, readLen, killPwd);
        i[1] = CommandLib::readTag(0x00, 0x02, readLen, accessPwd);

		//return 0;
        return i;
}

int* CommandLib::readReservedOnPsw(unsigned char* psw, unsigned char* &killPwd, unsigned char* &accessPwd)
{
        unsigned char readLen = 0x04;
        int *i = new int[2];
        i[0] = CommandLib::readTagOnPsw(0x00, 0x00, psw, readLen, killPwd);
        i[1] = CommandLib::readTagOnPsw(0x00, 0x02, psw, readLen, accessPwd);
                //return 0;
        return i;
}


int CommandLib::readUser(unsigned char &readLen, unsigned char* &tagInfo, unsigned char begAdd)
{
	//readLen = 0x40;
	readLen = 0x00;
        int i = CommandLib::readTag(0x03, begAdd, readLen, tagInfo);
		//return 0;
	return i;
}

//op操作数：解锁0x00，锁0x01，永久锁0x02，锁且永久锁0x03；mem内存区
//mem内存区： 0x00Kill PSW, 0x01Acess PSW, 0x02EPC, 0x03TID, 0x04User
int CommandLib::lockUnlock(unsigned char op, unsigned char mem, unsigned char* &psw)
{
	//flag = false;
        unsigned char *sendBytes = new unsigned char[8];
        sendBytes[0] = 0x3b;
        sendBytes[1] = 0x08;
        sendBytes[2] = op;
        sendBytes[3] = mem;
        for(int i=0;i<4;i++)
            sendBytes[i+4]=psw[i];
        mySerialPort->WriteData(sendBytes,8);
        delete [] sendBytes;
        sendBytes = NULL;
        unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            delete [] a;
            a = NULL;
            flush();
           return -1;
        }
        int len = a[1];
        unsigned char* rec = new unsigned char[len];
        rec[0]=a[0];
        rec[1]=len;
        if(!read(2,len-2,rec)){
            delete [] a;
            a = NULL;
            delete [] rec;
            rec = NULL;
            flush();
            return -1;
        }
        delete []a;
        a = NULL;
        int ret = rec [2];
        delete [] rec;
        rec = NULL;
        flush();
        return ret;
}

int CommandLib::killTag(unsigned char* &psw)
{
	//flag = false;
        unsigned char *sendBytes = new unsigned char[7];
        sendBytes[0] = 0x3d;
        sendBytes[1] = 0x07;
        for(int i=0;i<4;i++)
            sendBytes[i+2]=(unsigned char)(psw[i]);
        sendBytes[6] = 0x00;
        mySerialPort->WriteData(sendBytes,7);
                delete []sendBytes;
        //while(!flag){
        //    Sleep(10);
        //}
        unsigned char* kTag = new unsigned char[10];
        read(0,10,kTag);
        add(10);
        unsigned char* a = new unsigned char[2];
        if(!read(0,2,a)){
            delete [] a;
            a = NULL;
            flush();
            return -1;
        }
        int len = a[1];
        unsigned char* rec = new unsigned char[len];
        rec[0]=a[0];
        rec[1]=len;
        if(!read(2,len-2,rec)){
            delete []a;
            a = NULL;
            delete []rec;
            rec = NULL;
            flush();
            return -1;
        }
        delete []a;
        a = NULL;
        flush();
        int retCode = rec[2];
        delete [] rec;
        rec =NULL;
//        if(!rec[2])
//            return 1;
        return retCode;
}

	/*<tested>*/
int CommandLib::setChangerFreq(unsigned char mask, unsigned char* &freq, char RSSI, unsigned char activePro)
{
	//flag = false;
        unsigned char *sendBytes = new unsigned char[8];
        sendBytes[0] = 0x41;
        sendBytes[1] = 0x08;
        sendBytes[2] = mask;
        sendBytes[3] = freq[2];
        sendBytes[4] = freq[1];
        sendBytes[5] = freq[0];
        sendBytes[6] = RSSI;
        sendBytes[7] = activePro;
        mySerialPort->WriteData(sendBytes,8);
        //while(!flag){
        //    Sleep(5);
        //}
	//cout<<"buffer[2] :"<<buffer[2]<<endl;
		unsigned char* a = new unsigned char[2];
		read(0,2,a);
		int len = a[1];
		unsigned char* rec = new unsigned char[len];
		rec[0]=a[0];
		rec[1]=len;
		read(2,len-2,rec);
		delete a;
		flush();
        unsigned char i = rec[2];
        unsigned char j = rec[3];
//	if((i==-2||i==-4)&&j==-1)
        if((i==0xfe||i==0xfc)&&j==0xff)
		return 1;
	else return 0;
}

	/*<tested>*/
int CommandLib::setLBT(UINT intLis, UINT intSend, UINT intIdle)
{
	//flag = false;
	
        unsigned char lis[2];
        unsigned char send[2];
        unsigned char idle[2];
        lis[1] = (unsigned char)intLis;
        lis[0] = (unsigned char)(intLis>>8);
        send[1] = (unsigned char)intSend;
        send[0] = (unsigned char)(intSend>>8);
	//cout<<"高字节: "<<(int)(send[0])<<"低字节: "<<(int)(send[1])<<endl;
        idle[1] = (unsigned char)intIdle;
        idle[0] = (unsigned char)(intIdle>>8);
        unsigned char *sendBytes = new unsigned char[9];
        sendBytes[0] = 0x41;
        sendBytes[1] = 0x09;
        sendBytes[2] = 0x10;
        sendBytes[3] = lis[1];
        sendBytes[4] = lis[0];
        sendBytes[5] = send[1];
        sendBytes[6] = send[0];
        sendBytes[7] = idle[1];
        sendBytes[8] = idle[0];
        mySerialPort->WriteData(sendBytes,9);
		delete sendBytes;
        //while(!flag){
        //    Sleep(10);
        //}
		unsigned char* a = new unsigned char[2];
		read(0,2,a);
		int len = a[1];
		unsigned char* rec = new unsigned char[len];
		rec[0]=a[0];
		rec[1]=len;
		read(2,len-2,rec);
		delete a;
		flush();
        if((rec[2])==0xfe&&(rec[3])==0xff)
		return 1;
	return 0;
}

/**
*retbuffer指向一个数组，该数组从头开始分别代表
*ActiveProfile、监听时间低字节、监听时间高字节、最大发送时间低字节、最大发送时间高字节、空闲时间低字节、
*空闲时间高字节、最小频率低字节、最小频率中字节、最小频率高字节、最大频率低字节、最大频率中字节、最大频率高字节、
*跳频数、RSSI阈值、当前频率
*/
	/*<tested>*/
//int CommandLib::getFreqPara(unsigned char* &retbuffer)
int CommandLib::getFreqPara(char* &retbuffer){
//    flag = false;
//    unsigned char *sendBytes = new unsigned char[3];
//    sendBytes[0] = 0x41;
//    sendBytes[1] = 0x03;
//    sendBytes[2] = 0x11;
    unsigned char sendBytes[] = {(char)0x41, (char)0x03, (char)0x11};
    mySerialPort->WriteData(sendBytes,3);
//	delete sendBytes;
    //while(!flag){
    //    Sleep(10);
    //}
	unsigned char* a = new unsigned char[2];
	read(0,2,a);
	int len = a[1];
	unsigned char* rec = new unsigned char[len];
	rec[0]=a[0];
	rec[1]=len;
	read(2,len-2,rec);
        delete []a;
	flush();
    if((rec[2])==0xFE && (rec[3])==0xFF)
    {
            retbuffer = (char *)rec+4;
            delete [] rec;
            return 1;
    }
    delete [] rec;
    return 0;
}

int CommandLib::getInventoryRSSI(unsigned char** &EPC, int &tagNum, int* &EPCLen, int* &intRSSI,int* &RSSI_I, int* &RSSI_Q, unsigned char** &freq){
    unsigned char sendBytes []= {(char)0x43, (char)0x03,(char)0x01};
    mySerialPort->WriteData(sendBytes,3);

    unsigned char* a = new unsigned char[2];
    if(!read(0,2,a)){
        delete [] a;
        a = NULL;
        flush();
        return 0;
    }
    int len = a[1];
    unsigned char* rec = new unsigned char[len];
    rec[0]=a[0];
    rec[1]=len;
    if(!read(2,len-2,rec)){
        delete []a;
        a = NULL;
        delete []rec;
        rec = NULL;
        flush();
        return 0;
    }
    tagNum = rec[2];
    if(tagNum == 0){
       delete []a;
       a = NULL;
       delete []rec;
       rec = NULL;
       flush();
       return 0;
    }
    //length returns the true EPC length
    //commandLen returns the Command length
    //unsigned char commandLen = rec[1];
    unsigned char EPCLength = rec[7]-2;
    EPC = new unsigned char *[tagNum];
//    unsigned char EPC = EPC_Copy;
//    delete [] EPC_Copy;
    EPCLen = new int [tagNum];
//    EPCLen = EPCLen_Copy;
    unsigned char *RSSI  = new unsigned char [tagNum];
//    intRSSI = new int [tagNum];
    intRSSI = new int [tagNum];
//    freq = new unsigned char *[tagNum];
    freq = new unsigned char *[tagNum];
//    RSSI_I = new int [tagNum];
    RSSI_I = new int [tagNum];
//    RSSI_Q = new int [tagNum];
    RSSI_Q = new int [tagNum];
    EPC[0] = new unsigned char[EPCLength];
    for(int j=0;j<EPCLength;j++)
        EPC[0][j] = rec[10+j];
//    EPC[0][j] = rec[10+j];
//    EPCLen[0] = EPCLength;
    EPCLen[0] = EPCLength;
    RSSI[0] = rec[3];
    RSSI_I[0] = (INT)(((RSSI[0] & (0x0F)))*2);
    RSSI_Q[0] = (INT)((((RSSI[0] & (0xF0)) >>4))*2);
    intRSSI[0] = ((max(RSSI_I[0],RSSI_Q[0]))*100/30);
    freq[0] = new unsigned char[3];
    for(int m=0;m<3;m++)
        freq[0][m] = rec[4+m];
    add(len);
    delete []rec;
    rec = NULL;
    for(int i=1;i<tagNum;i++){
        if(!read(0,2,a)){
            delete [] a;
            a = NULL;
            flush();
            return 0;
        }
        int len = a[1];
        unsigned char* rec = new unsigned char[len];
        rec[0]=a[0];
        rec[1]=len;
        if(!read(2,len-2,rec)){
            delete []a;
            a = NULL;
            delete []rec;
            rec = NULL;
            flush();
            return 0;
        }
        RSSI[i] = rec[3];
//          intRSSI[i] = (int)RSSI[i];
                        //cout<<"RSSI[i]: "<<RSSI[i]<<endl;
        RSSI_I[i] = (INT)(((RSSI[i] & (0x0F)))*2);
        RSSI_Q[i] = (INT)((((RSSI[i] & (0xF0)) >>4))*2);
                        //cout<<"max"<<max(RSSI_I[i],RSSI_Q[i])<<endl;
        intRSSI[i] = ((max(RSSI_I[i],RSSI_Q[i]))*100/30);
        EPCLength = rec[7]-2;
        EPCLen[i] = EPCLength;
        EPC[i] = new unsigned char[EPCLength];
        freq[i] = new unsigned char[3];
        for(int j=0;j<(int)EPCLength;j++)
            EPC[i][j]=rec[j+10];
        for(int m=0;m<3;m++)
            freq[i][m] = rec[4+m];
            //commandLen += buffer[1+commandLen];
        add(len);
        delete []rec;
        rec = NULL;
    }
//    for(int i=1;i<tagNum;i++){
//        delete []EPC_Array[i];
//        delete []freq_Array[i];
//    }
    delete []RSSI;
    RSSI = NULL;
    delete []a;
    a = NULL;
    flush();
    return 1;
}

/**
*linkFreq:  连接频率：0=40kHz，3=80 kHz，6=160 kHz，8=213 kHz,9=256 kHz,12=320 kHz
*sendBytes[5]:  Miller:0=FM0,1=Miller2,2= Miller4,3= Miller8
*sendBytes[7]:  Session:0=S0, 1=S1, 2=S2, 3=S3,4=SL
*sendBytes[9]:  Trext:1使用，0不使用
*sendBytes[10]: Q：slot=0到2^q的随机数
*/
/*tested*/
//int CommandLib::setGen2Settings(int linkFreqInt, unsigned char miller, unsigned char session, unsigned char trext, unsigned char Q, int sens,
//                                int &_linkFreq, unsigned char &_miller, unsigned char &_session, unsigned char &_trext, unsigned char &_Q, int &_sens)
int CommandLib::setGen2Settings(int linkFreqInt, unsigned char miller, unsigned char session, unsigned char trext, unsigned char Q,
                                int &_linkFreq, unsigned char &_miller, unsigned char &_session, unsigned char &_trext, unsigned char &_Q){
        /*flag=false;*/
        unsigned char linkFreq;
        switch(linkFreqInt)
        {
        case 40:
                linkFreq = 0x00;
                break;
        case 80:
                linkFreq = 0x03;
                break;
        case 160:
                linkFreq = 0x06;
                break;
        case 213:
                linkFreq = 0x08;
                break;
        case 256:
                linkFreq = 0x09;
                break;
        case 320:
                linkFreq = 0x12;
                break;
        case 640:
                linkFreq = 0x15;
                break;
        default:
                return 0;
                break;
        }

        unsigned char *sendBytes = new unsigned char[12];
        sendBytes[0] = 0x59;
        sendBytes[1] = 0x0C;
        sendBytes[2] = 0x01;
        sendBytes[3] = linkFreq;
        sendBytes[4] = 0x01;
        sendBytes[5] = miller;
        sendBytes[6] = 0x01;
        sendBytes[7] = session;
        sendBytes[8] = 0x01;
        sendBytes[9] = trext;
        sendBytes[10] = 0x01;
        sendBytes[11] = Q;
		//sendBytes[12] = 0x01;
		//sendBytes[13] = sens;
        mySerialPort->WriteData(sendBytes,12);
		delete sendBytes;
        //while(!flag){
        //    Sleep(10);
        //}
		unsigned char* a = new unsigned char[2];
		read(0,2,a);
		int len = a[1];
		unsigned char* rec = new unsigned char[len];
		rec[0]=a[0];
		rec[1]=len;
		read(2,len-2,rec);
        if(rec[0] != 0x5A || rec[2] != 0x00 || rec[4]!=0x00 || rec[6]!=0x00
                || rec[8]!=0x00 || rec[10]!=0x00|| rec[12]!=0x00)
                return 0;

        switch(rec[3])
        {
        case 0x00:
                _linkFreq = 40;
                break;
        case 0x03:
                _linkFreq = 80;
                break;
        case 0x06:
                _linkFreq = 160;
                break;
        case 0x08:
                _linkFreq = 213;
                break;
        case 0x09:
                _linkFreq = 256;
                break;
        case 0x12:
                _linkFreq = 320;
                break;
        case 0x15:
                _linkFreq = 640;
                break;
        default:
                return 0;
        }
        _miller = rec[5];
        _session = rec[7];
        _trext = rec[9];
        _Q = rec[11];
		delete a;
		flush();
		//_sens = buffer[13];
        return 1;
}

int CommandLib::setSensSetting(int sens, int &_sens){
	//flag=false;
        unsigned char *sendBytes = new unsigned char[14];
	sendBytes[0] = 0x59;
        sendBytes[1] = 0x0E;
	for(int i=2;i<12;i++){
		sendBytes[i] = 0x00;
	}
	sendBytes[12] = 0x01;
	sendBytes[13] = sens;
	mySerialPort->WriteData(sendBytes,14);
	delete sendBytes;
	unsigned char* a = new unsigned char[2];
	read(0,2,a);
	int len = a[1];
	unsigned char* rec = new unsigned char[len];
	rec[0]=a[0];
	rec[1]=len;
	read(2,len-2,rec);
	delete a;
	flush();
	if(rec[0] != 0x5A||rec[12]!=0x00)
		return 0;
        _sens = (char) rec[13];
        return 1;
}
int CommandLib::getSensSetting(int &_sens){
    unsigned char *sendBytes = new unsigned char[14];
    sendBytes[0] = 0x59;
    sendBytes[1] = 0x0E;
    for(int i=2;i<14;i++){
            sendBytes[i] = 0x00;
    }
    mySerialPort->WriteData(sendBytes,14);
    delete sendBytes;
        unsigned char* a = new unsigned char[2];
    read(0,2,a);
    int len = a[1];
    unsigned char* rec = new unsigned char[len];
    rec[0]=a[0];
    rec[1]=len;
    read(2,len-2,rec);
    delete a;
    flush();
    if(rec[0] != 0x5A||rec[12]!=0x00)
            return 0;
    _sens = (char) rec[13];

    return 1;
}


/*tested
说明: sens取值只有-50至-68，increment为3
*/
int CommandLib::getGen2Settings(int &linkFreq, unsigned char &miller, unsigned char &session, unsigned char &trext, unsigned char &Q, int &sens){
	//flag=false;
        unsigned char *sendBytes = new unsigned char[14];
        sendBytes[0] = 0x59;
        sendBytes[1] = 0x0E;
        for(int i=2;i<14;i++)
            sendBytes[i] = 0x00;
        mySerialPort->WriteData(sendBytes,14);
        delete []sendBytes;
        //while(!flag){
        //    Sleep(10);
        //}
	unsigned char* a = new unsigned char[2];
	read(0,2,a);
	int len = a[1];
	unsigned char* rec = new unsigned char[len];
	rec[0]=a[0];
	rec[1]=len;
	read(2,len-2,rec);
	switch(rec[3]){
	case 0x00: 
		linkFreq = 40;
		break;
	case 0x03:
		linkFreq = 80;
		break;
        case 0x06:
		linkFreq = 160;
		break;
        case 0x08:
		linkFreq = 213;
		break;
        case 0x09:
		linkFreq = 256;
		break;
        case 0x12:
		linkFreq = 320;
		break;
        case 0x15:
            linkFreq = 640;
            break;
	default:
        return 0;
	}
        delete []a;
	flush();
	//这边有待测试，不能直接用buffer[]跟0xXX比较
        if( (0x5A)!=  rec[0]  || (rec[2]) != 0x00 || rec[4]!=0x00 || rec[6]!=0x00
                || rec[8]!=0x00 || rec[10]!=0x00)
                return 0;

	miller = rec[5];
	session = rec[7];
	trext = rec[9];
        Q = rec[11];
        sens = rec[13];
        //0xBC表示-68
		//switch(buffer[13]){
		//case (0xBC):
		//	sens = -68;
		//	break;
		//case (0xBF):
		//	sens = -65;
		//	break;
		//case (0xC2):
		//	sens = -62;
		//	break;
		//case (0xC5):
		//	sens = -59;
		//	break;
		//case (0xC8):
		//	sens = -56;
		//	break;
		//case (0xCB):
		//	sens = -53;
		//	break;
		//case (0xCE):
		//	sens = -50;
		//	break;
		//}
        delete [] rec;
        return 1;
}
/**
*单条指令实现_End
*/

int CommandLib::getOutputLevel(int &value, unsigned char &v){
    unsigned char *regData;
    if(!readReg(0x15, regData)){
        return 0;
    }
    unsigned char outLev = regData[0];
    v = outLev;
    if((outLev & 0x10) != 0x00){
        value = (int)(0 - (outLev & 0x07) - 12);
        return 1;
    }
    else if((outLev & 0x08) != 0x00){
        value = (int)(0 - (outLev & 0x07) - 6);
        return 1;
    }
    else if((outLev & 0x78) == 0x00){
        value = (int)(0 - outLev);
        return 1;
    }

    return 0;
}


int CommandLib::getOutputLevel(int &value){
    unsigned char *regData;
    if(!readReg(0x15, regData)){
        return 0;
    }
    unsigned char outLev = regData[0];
        if((outLev & 0x10) != 0x00){
        value = (int)(0 - (outLev & 0x07) - 12);
        return 1;
    }
    else if((outLev & 0x08) != 0x00){
        value = (int)(0 - (outLev & 0x07) - 6);
        return 1;
    }
    else if((outLev & 0x78) == 0x00){
        value = (int)(0 - outLev);
        return 1;
    }

    return 0;
}
/**
*复合指令实现_Start
*/
//changer the output level
//从-19到0，分别改变15#寄存器的34567位
//第4位置1表示-6，第3位置1表示-12，34位为粗调，567位表示游标微调
	/*<tested>*/
int CommandLib::changeOutputLevel(int outLev, unsigned char &v)
{
    unsigned char *regData;
    if(!readReg(0x15, regData)){
        return 0;
    }
    unsigned char b = regData[0];
    unsigned char temp = b & 0xE0;
    if(outLev==0){
        regData[0]=temp;
    }
    else if(outLev==-6){
        regData[0]= 0x08 | temp;
    }
    else if(outLev==-12){
        regData[0]= 0x10 | temp;
    }
    else if(outLev<0&&outLev>-6){
        regData[0] = temp | (unsigned char)(-outLev);
    }
    else if(outLev<-6&&outLev>-12){
        regData[0] = (0x08 | temp) | (-outLev-6);
    }
    else if(outLev<-12){
        regData[0] = (0x10 | temp) | (-outLev-12);
    }
    else{
        return 0;
    }
    v = regData[0];
    if(!writeReg(0x15, 0x03, regData)){
        return 0;
    }
    return 1;
}

	/*<tested>*/
int CommandLib::getFreqParam(unsigned char &activePro,UINT &lis,UINT &maxAlloc,UINT &idle, UINT &startFreq, UINT &endFreq, double &inc, int &rssi)
{
//        unsigned char *retbuffer = NULL;
        char *retbuffer = NULL;
        getFreqPara(retbuffer);
		activePro = retbuffer[0];
        lis = retbuffer[2]*256+(unsigned char)(retbuffer[1]);
        maxAlloc = retbuffer[4]*256+(unsigned char)(retbuffer[3]);
        idle = retbuffer[6]*256+(unsigned char)(retbuffer[5]);

        startFreq = (unsigned char)retbuffer[9]*65536+(unsigned char)retbuffer[8]*256+(unsigned char)retbuffer[7];
        endFreq = ((unsigned char)retbuffer[12]*65536)+((unsigned char)retbuffer[11]*256)+(unsigned char)retbuffer[10];

        //cout<<"retbuffer10"<<(int)(unsigned char)retbuffer[10]<<endl;
        int hopNum = retbuffer[13];
        rssi = retbuffer[14];
        inc = ((double)endFreq / 1000.0 - (double)startFreq / 1000.0) / ((double)hopNum - 1.0);

	return 1;
}

//自定义设置范围为840.000至960.000
//分别对应freqs设置的start freq，end freq，increment，rssi threshold，idle time， listen time，max allocation
	/*<tested>*/
int CommandLib::setUserFreq(double startFreq, double endFreq,double inc,int RSSI, UINT idle, UINT lis, UINT maxAlloc, unsigned char activeProfile)
{
	UINT start = (UINT)(startFreq*1000);
	UINT end = (UINT)(endFreq*1000); 
	UINT m = start;
	UINT n = m + inc*1000;
	//这里hex[6]为最小大小，否则越界,原因未知
        unsigned char hex[6];
        unsigned char hexP[6];
        unsigned char *_hex = NULL;
        unsigned char *_hexP = NULL;
        unsigned char rssiCom;
    //unsigned char ActiveProfile = 0x00;
        //unsigned char hexMirr[3];
	hexConversion(m,hex);
	hexConversion(n,hexP);
	//hexMirr[0] = hex[2];
	//hexMirr[1] = hex[1];
	//hexMirr[2] = hex[0];
	_hexP = hexP;
	_hex = hex;
	decToComplement(RSSI, rssiCom);
        setChangerFreq(0x08, _hex, rssiCom, activeProfile);
	//int i = 0;
        //unsigned char hex1[3];
	while(n<=end+1){
            
		hexConversion(n,hexP);
		//hexMirr[0] = hex1[2];
		//hexMirr[1] = hex1[1];
		//hexMirr[2] = hex1[0];
		//hexP = hexMirr;
		_hexP = hexP;
		decToComplement(RSSI, rssiCom);
		//int i;
        if(!setChangerFreq(0x04, _hexP, rssiCom, activeProfile))
		{
                    return 0;
                }
		n+=(UINT)(inc*1000);
		//i++;
	}
	setLBT(lis,maxAlloc,idle);
	//cout<<"次数： "<<i<<endl;
        return 1;
}

int CommandLib::getUSAFreq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc){
    startFreq = 902.750;
    endFreq = 927.250;
    inc = 0.500;
    RSSI = -40;
    idle = 0;
    lis = 1;
    maxAlloc = 400;
    return 1;
}

/*<tested>*/
int CommandLib::setUSAFreq()
{
        if(setUserFreq(902.750, 927.250, 0.500, -40, 0, 1, 400, 0x03)){
            return 1;
        }
        else{
            return 0;
        }
}



int CommandLib::getEurFreq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc){
    startFreq = 865.700;
    endFreq = 867.500;
    inc = 0.600;
    RSSI = -40;
    idle = 0;
    lis = 1;
    maxAlloc = 10000;
    return 1;
}

/*<tested>*/
int CommandLib::setEurFreq()
{
        if(setUserFreq(865.700, 867.500, 0.600, -40, 0, 1, 10000, 0x01)){
            return 1;
        }
        else{
            return 0;
        }
}


int CommandLib::getJapFreq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc){
    startFreq = 952.4000;
    endFreq = 953.6000;
    inc = 1.200;
    RSSI = -87;
    idle = 100;
    lis = 10;
    maxAlloc = 4000;
    return 1;
}

/*<tested>*/
int CommandLib::setJapFreq()
{
    if(setUserFreq(952.4000, 953.6000, 1.200, -87, 100, 10, 4000, 0x02)){
        return 1;
    }
    else{
        return 0;
    }
}

int CommandLib::getCHN920Freq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc){
    startFreq = 920.625;
    endFreq = 924.375;
    inc = 0.750;
    RSSI = -40;
    idle = 0;
    lis = 1;
    maxAlloc = 10000;
    return 1;
}

/*<tested>*/
int CommandLib::setCHN920Freq()
{
    if(setUserFreq(920.625, 924.375, 0.750, -40, 0, 1, 10000, 0x04)){
        return 1;
    }
    else{
        return 0;
    }
}


int CommandLib::getCHN840Freq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc){
    startFreq = 840.125;
    endFreq = 844.875;
    inc = 0.250;
    RSSI = -40;
    idle = 0;
    lis = 1;
    maxAlloc = 10000;
    return 1;
}

/*<tested>*/
int CommandLib::setCHN840Freq()
{
    if(setUserFreq(840.125, 844.875, 0.250, -40, 0, 1, 10000, 0x05)){
        return 1;
    }
    else{
        return 0;
    }
}

int CommandLib::getKorFreq(double &startFreq, double &endFreq, double &inc, int &RSSI, UINT &idle, UINT &lis, UINT &maxAlloc){
    startFreq = 917.300;
    endFreq = 920.300;
    inc = 0.600;
    RSSI = -40;
    idle = 0;
    lis = 1;
    maxAlloc = 10000;
    return 1;
}

/*<tested>*/
int CommandLib::setKorFreq()
{
    if(setUserFreq(917.300,920.300,0.600,-40,0,1,10000, 0x06)){
        return 1;
    }
    else{
        return 0;
    }
}
//十进制转二进制
/*<tested>*/
void CommandLib::decToBin(UINT dec, int *bin)
{
 //int nNum = 0;
 int nSwap = 999999;   
 int nPos = 0;        //此变量做记录转换后的二进制数的个数
 //cin >> nNum;

 for(int i = 0; i < 24; i++)
 {
  nSwap = dec / 2 ;
  if(dec % 2 == 1)     //数组的0位置存放二进制的低位（二进制数从低位到高位的顺序存放到数组中）
  {
   bin[i] = 1;
  }
  else
  {
   bin[i] = 0;
  }
  nPos = i;
  if(nSwap == 0)
   break;
  else
   dec = nSwap ;
 }

}

//二进制转16进制
        /*<tested>*/
int CommandLib::binToHex(int *bin, char *hexStr)
{
	int i;
	int j=0;
	//int m;
	for(int m=0;m<6;m++){
	i = bin[0+j]+bin[1+j]*10+bin[2+j]*100+bin[3+j]*1000;
	j+=4;
	switch(i) 
 {
 case 0:
  hexStr[5-m] = '0';
  break;
 case 1:
  hexStr[5-m] = '1';
  break;
 case 10:
 hexStr[5-m] = '2';
  break;
 case 11:
 hexStr[5-m] = '3';
  break;
 case 100:
 hexStr[5-m] = '4';
  break;
 case 101:
 hexStr[5-m] = '5';
  break;
 case 110:
 hexStr[5-m] = '6';
  break;
 case 111:
 hexStr[5-m] = '7';
  break;
 case 1000:
 hexStr[5-m] = '8';
  break;
 case 1001:
 hexStr[5-m] = '9';
  break;
 case 1010:
 hexStr[5-m] = 'A';
  break;
 case 1011:
 hexStr[5-m] = 'B';
  break;
 case 1100:
 hexStr[5-m] = 'C';
  break;
 case 1101:
 hexStr[5-m] = 'D';
  break;
 case 1110:
 hexStr[5-m] = 'E';
  break;
 case 1111:
 hexStr[5-m] = 'F';
  break;
 default:
	 return -1;
 }
	}
	return 1;
}

//十六进制字符串转十六进制数
        /*<tested>*/
void CommandLib::hexstrToHex(char *hexStr, unsigned char *hex)
{
	//string str = hexStr[5]+hexStr[4]+hexStr[3]+hexStr[2]+hexStr[1]+hexStr[0];
	for(int i=0;i<3;i++)
	sscanf_s(hexStr+i*2,"%2x",(hex+i));
}

//十进制转十六进制数，返回在hex[3]中,其中hex[0]为高位，hex[2]为低位，但是指令写格式为低位在前，高位在后
        /*<tested>*/
void CommandLib::hexConversion(UINT dec, unsigned char *hex)
{
        char hexStr[6];
	int bin[24] = {0};
	decToBin(dec, bin);
	binToHex(bin, hexStr);
	hexstrToHex(hexStr, hex);
}

//负十进制int数转补码
        /*<tested>*/
void CommandLib::decToComplement(int dec, unsigned char &hexComp)
{
	//if(dec<0){
                //(unsigned char)dec&(0x)
                unsigned char ch = (((~(unsigned char(-dec)))&(0x7f))+1)|(0x80);
        //unsigned char temp0 = ~(unsigned char(-dec));
        //unsigned char temp1 = temp0 & 0x7f;
        //unsigned char temp2 = temp1+1;
        //unsigned char ch = temp2|0x80;
	hexComp = ch;
	//}
	//else return;
}

//获取串口号(regTable)
/* 函数名 : GetSerialPortsReg
*  说  明 : 通过注册表枚举系统串口数量，并列出所有串口的系统名
*
*  参  数 : pListStr, 串口名称列表
*  返回值 : int, 系统串口个数
*//*
int CommandLib::getSerialPortsReg(unsigned char (*pListStr)[80])
{
    HKEY hKey;
    LPCWSTR data_Set = L"HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
    long ret0 = (::RegOpenKeyExW(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey));
    if(ret0 != ERROR_SUCCESS)
    {
        return -1;
    }
    int i = 0;
    LPWSTR Name;
    //TCHAR Name[25];//Name = 0x0012fd5c "\Device\Serial0"
    UCHAR szPortName[80];//C O M 1
    LONG Status;
    DWORD dwIndex = 0;
    DWORD dwName;
    DWORD dwSizeofPortName;
    DWORD Type;
    dwName = sizeof(Name);
    dwSizeofPortName = sizeof(szPortName);
    do
    {
        Status = RegEnumValue(hKey, dwIndex++, Name, &dwName, NULL, &Type,
            szPortName, &dwSizeofPortName);
       if((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA))
        {
            if (pListStr != NULL)
            {
               memcpy(*(pListStr + i), (LPCSTR)szPortName, 80);
           }
            i++;
        }
    } while((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA));
    RegCloseKey(hKey);
    return i;
}*/

//获取串口号(设备管理器)(Uncomplete)
/**
int CommandLib::getSerialPortsDevM(unsigned char **pListStr){
	HKEY hKey;
	DWORD dwCount=0;//保存检索项的结果，即COM端口总数
	DWORD dwBufLen=_MAX_PATH;//缓存大小
	DWORD dwBufLen1[_MAX_PATH];//缓存大小
	::FillMemory((LPVOID)dwBufLen1,_MAX_PATH*2,0xFF);
	DWORD dwBufLen2[_MAX_PATH];//缓存大小
	::FillMemory((LPVOID)dwBufLen2,_MAX_PATH*2,0xFF);
    

    DWORD dwTp=REG_DWORD;//待检索项的数据类型

	TCHAR subKey[]=_T("SYSTEM\\CurrentControlSet\\Services\\serenum\\Enum");


	if(ERROR_SUCCESS!=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKey,
		NULL,KEY_QUERY_VALUE,&hKey))
	return;//打开注册表失败，退出程序


	//注册表打开成功，进一步检索设备管理器的设备树显示的端口节点里COM口的个数
	if(ERROR_SUCCESS!=::RegQueryValueEx(hKey, _T("Count"), NULL, &dwTp,
		(LPBYTE)&dwCount, &dwBufLen)){
		::RegCloseKey( hKey );
		return;//检索失败，退出
		}

	HKEY hKey1;
	TCHAR *subKey1=_T("SYSTEM\\CurrentControlSet\\Enum\\");//的COM设备名字检索路径     
	wstring strReg;
	wstring strACPI(('\0'),_MAX_PATH);
	LPSTR lpstrACPI=strACPI.GetBuffer(_MAX_PATH);

	wstring strACPI1(_T('\0'),_MAX_PATH);
	LPSTR lpstrACPI1=strACPI1.GetBuffer(_MAX_PATH);

     
for(unsigned int i=0;i<dwCount;i++)//COM端口数检索成功，进一步检索在设备管理器里出现的文字
{
  
   strReg.Format("%d",i);
  
   if(::RegQueryValueEx(hKey,(LPCTSTR)strReg,NULL, 
    &dwTp,(LPBYTE)lpstrACPI,&dwBufLen1[i])
    !=ERROR_SUCCESS) 
    break;//检索失败，退出
  
  
   subKey1=subKey1+lpstrACPI;

   if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKey1,
    NULL,KEY_QUERY_VALUE,&hKey1)!=ERROR_SUCCESS)   
    break;//打开注册表失败，退出程序
  
        
   DWORD dwTp1=REG_SZ;//此处即为COM的说明文字项数据类型
   if(ERROR_SUCCESS!=::RegQueryValueEx(hKey1, _T("FriendlyName"), NULL, &dwTp1,
    (LPBYTE)lpstrACPI1, &dwBufLen2[i]))//检索COM口具体说明文字，即FriendlyName的值
     break;//检索失败，退出

   m_ctlComm.AddString(lpstrACPI1);//添加检索的值添加到组合列表框
  
   strReg.Empty();//重新对变量赋值，避免前后冲突
   subKey1.Empty();
   subKey1=_T("SYSTEM\\CurrentControlSet\\Enum\\");
   strACPI.Empty();
   strACPI1.Empty();
   strACPI=wstring(_T('\0'),_MAX_PATH);
   strACPI1=wstring(_T('\0'),_MAX_PATH);      

}

    
strACPI.ReleaseBuffer();
strACPI1.ReleaseBuffer();
::RegCloseKey( hKey1 );
::RegCloseKey( hKey );
m_ctlComm.SetCurSel(0);
}*/
bool CommandLib::read(int beg, int len, unsigned char* &arr){
        int n = 200;
        while(n--){
                if(rdLen+beg+len>accum+1){
                        Sleep(5);
                        continue;
		}
		else
                    break;
	}
        if(n<=0){
            return false;
        }
        else{
	for(int i=beg;i<beg+len;i++)
		arr[i] = buffer[i+rdLen];
        return true;}
}

void CommandLib::flush(){
	accum=-1;
	rdLen = 0;
}

void CommandLib::add(int len){
	rdLen+=len;
}
