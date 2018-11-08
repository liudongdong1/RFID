// UHFReader.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "lib\uhf\CommandLib.h"

using namespace std;


CommandLib *uhf;


char* ByteArrayToQString(unsigned char *array, int length) {   //长度12
	char *result = new char[3 * length];   //
	for (int i = 0; i < length - 1; i++) {
		sprintf(result + 2 * i, "%02X", array[i]);
		result[3 * i + 2] = ' ';
	}
	sprintf(result + (3 * length - 3), "%02X", array[length - 1]);
	result[3 * length - 1] = '\0';
	return result;
}

/**
*mem 所要写的内存标签区
*begAdd 写入内存区的起始地址    paw 需要提供的密码地址   data 代写入数据的起始地址   dataLen 写入数据的长度  /unsigned char* &psw, unsigned char* &data
**/
void writeBank(unsigned char *EPC, unsigned char len, unsigned char mem, unsigned char begAdd, unsigned char* psw, unsigned char* data, unsigned char dataLen) {
	cout << "write bank:" << endl;
	int n = 5;
	int flag;
	while (--n) {
		flag = uhf->selectTag(EPC, len);
		if (!flag) {
			Sleep(5);
			continue;
		}
		else {
			cout << "select successfully!" << endl;
			break;
		}
	}

	if (uhf->writeTag(mem, begAdd, psw, data, dataLen) == 0) {
		cout << "write successfully!" << endl;
	}


}
//mark =0，killpassword   mark=1  acesspassword,  密码为四个字节，EPC为12字节
int changePassword(unsigned char *EPC, unsigned char len, unsigned char* psw, unsigned char* newPsw, bool mark) {
	cout << "change password:" << endl;
	int n = 5;
	int flag;
	while (--n) {
		flag = uhf->selectTag(EPC, len);
		if (!flag) {
			Sleep(5);
			continue;
		}
		else {
			cout << "select successfully!" << endl;
			break;
		}
	}

	return uhf->setPsw(psw, newPsw, mark);
}
void readBank(unsigned char *EPC, unsigned char len) {
	cout << "read bank:" << endl;
	int n = 5;
	int flag;
	while (--n) {
		flag = uhf->selectTag(EPC, len);
		if (!flag) {
			Sleep(5);
			continue;
		}
		else {
			cout << "select successfully!" << endl;
			break;
		}
	}
	//扫描卡
	Sleep(1000);
	cout << "开始读取 EPC内容 ： " << endl;
	unsigned char l = 0;
	unsigned char *contents;    //标签中数据类型
	int re = uhf->readCompleteEPC(l, contents);   //读取EPC信息
	cout << "Bank EPC: ";
	char *c;
	for (int i = 0; i < l; i++) {
		c = new char[3];
		sprintf(c, "%02X", contents[i]);
		c[2] = '\0';
		cout << c << " ";
		delete c;
	}
	cout << endl;
	cout << "开始读取 TID内容 ： " << endl;
	l = 0;
	Sleep(1000);
	uhf->readCompleteTID(l, contents);   //读取TID信息并输出
	cout << "Bank TID: ";
	for (int i = 0; i < l; i++) {
		c = new char[3];
		sprintf(c, "%02X", contents[i]);
		c[2] = '\0';
		cout << c << " ";
		delete c;
	}
	cout << endl;
	Sleep(1000);
	cout << "开始读取  User内容 ： " << endl;
	l = 0;
	uhf->readUser(l, contents);    //读取用户data信息并输出
	cout << "Bank User: ";
	for (int i = 0; i < l; i++) {
		c = new char[3];
		sprintf(c, "%02X", contents[i]);
		c[2] = '\0';
		cout << c << " ";
		delete c;
	}
	cout << endl;
	l = 0;
	re = uhf->readCompleteEPC(l, contents);   //读取EPC信息
	cout << "读出EPC数据：" << l << endl;
	for (int i = 0; i < l; i++) {
		c = new char[3];
		sprintf(c, "%02X", contents[i]);
		c[2] = '\0';
		cout << c << " ";
		delete c;
	}
	unsigned char * d = new unsigned char[l];
	for (int i = l - 1, j = 0; i >= 0; i--, j++)
		d[j] = contents[i];
	cout << "颠倒后的EPC数据: " << endl;
	for (int i = 0; i < l; i++) {
		c = new char[3];
		sprintf(c, "%02X", d[i]);
		c[2] = '\0';
		cout << c << " ";
		delete c;
	}
	cout << "开始写入数据" << endl;
	unsigned char psw[] = { 0x00,0x00,0x00,0x00 };
	unsigned char*pw = psw;
	int p = 0;
	//p=uhf->setUser(0x00,psw,d);
	unsigned char* ff = (unsigned char*)"123456";
	p = uhf->writeTag(0x03, 0x01, pw,ff, l);
	delete d;
	//char p[]="liudongdong"; 
	unsigned char* t = (unsigned char*)"liudongdong";
	p = uhf->writeTag(0x03, 0x20, pw, t, 11);
	/*	int p=1;
		while(p<5)
		{

			Sleep(1000);
			p++;
		}*/
	if (p)
		cout << "写入数据成功 " << endl;
	cout << "再次开始读取判断是否写入" << endl;
	unsigned char *content;
	uhf->readUser(l, content);    //读取用户data信息并输出
	cout << "Bank User: ";
	for (int i = 0; i < l; i++) {
		c = new char[3];
		sprintf(c, "%02X", content[i]);
		c[2] = '\0';
		cout << c << " ";
		delete c;
	}
	cout << endl;

}

//op为锁的操作类型 0x00 解锁  0X01加锁     ,0X02永久锁    ,0X03锁并且永久锁
//mem 操作相应内存区地址
//psw 密码首地址
int lock(unsigned char *EPC, unsigned char len, unsigned char op, unsigned char mem, unsigned char* &psw) {
	cout << "lock & unlock:" << endl;
	int n = 5;
	int flag;
	while (--n) {
		flag = uhf->selectTag(EPC, len);
		if (!flag) {
			Sleep(5);
			continue;
		}
		else {
			cout << "select successfully!" << endl;
			break;
		}
	}

	return uhf->lockUnlock(op, mem, psw);
}


int inventory() {
	cout << "inventory:" << endl;
	unsigned char** EPC;
	int tagNum;
	int* EPCLen;
	int* intRSSI;
	int* RSSI_I;
	int* RSSI_Q;
	unsigned char** freq;
	char* epc;

	if (uhf->getInventoryRSSI(EPC, tagNum, EPCLen, intRSSI, RSSI_I, RSSI_Q, freq)) {//EPC LEN 12,tagname 表示扫描的标签数
		cout << endl << "扫描到" << tagNum << "个标签,具体如下 ：" << endl;
		for (int i = 0; i < tagNum; i++) {
			cout << "扫描到的第  " << i + 1 << "  个标签,具体如下 ：" << endl;
			cout << "EPC: " << ByteArrayToQString(EPC[i], EPCLen[i]) << endl;
			readBank(EPC[i], EPCLen[i]);
			Sleep(1000);


		}

		cout << endl;
		return 0;
	}
	else {
		cout << "du qu shibai" << endl;
		return -1;
	}
}
void welcome() {
	cout << endl;
	cout << "*******************************************************************" << endl;
	cout << "***                                                             ***" << endl;
	cout << "***                欢迎来到RFID读写实验                         ***" << endl;
	cout << "***                                                             ***" << endl;
	cout << "*******************************************************************" << endl;
	cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	uhf = new CommandLib("COM7");
	//uhf = new CommandLib("\\\\.\\COM13");
	welcome();
	int a = 1;
	while (a) {
		inventory();
		cout << "press 0 to stop,or continue" << endl;
		cin >> a;
	}
	system("pause");

	return 0;
}