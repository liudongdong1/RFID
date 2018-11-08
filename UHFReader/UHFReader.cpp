// UHFReader.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include "lib\uhf\CommandLib.h"

using namespace std;


CommandLib *uhf;


char* ByteArrayToQString(unsigned char *array, int length) {   //����12
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
*mem ��Ҫд���ڴ��ǩ��
*begAdd д���ڴ�������ʼ��ַ    paw ��Ҫ�ṩ�������ַ   data ��д�����ݵ���ʼ��ַ   dataLen д�����ݵĳ���  /unsigned char* &psw, unsigned char* &data
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
//mark =0��killpassword   mark=1  acesspassword,  ����Ϊ�ĸ��ֽڣ�EPCΪ12�ֽ�
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
	//ɨ�迨
	Sleep(1000);
	cout << "��ʼ��ȡ EPC���� �� " << endl;
	unsigned char l = 0;
	unsigned char *contents;    //��ǩ����������
	int re = uhf->readCompleteEPC(l, contents);   //��ȡEPC��Ϣ
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
	cout << "��ʼ��ȡ TID���� �� " << endl;
	l = 0;
	Sleep(1000);
	uhf->readCompleteTID(l, contents);   //��ȡTID��Ϣ�����
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
	cout << "��ʼ��ȡ  User���� �� " << endl;
	l = 0;
	uhf->readUser(l, contents);    //��ȡ�û�data��Ϣ�����
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
	re = uhf->readCompleteEPC(l, contents);   //��ȡEPC��Ϣ
	cout << "����EPC���ݣ�" << l << endl;
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
	cout << "�ߵ����EPC����: " << endl;
	for (int i = 0; i < l; i++) {
		c = new char[3];
		sprintf(c, "%02X", d[i]);
		c[2] = '\0';
		cout << c << " ";
		delete c;
	}
	cout << "��ʼд������" << endl;
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
		cout << "д�����ݳɹ� " << endl;
	cout << "�ٴο�ʼ��ȡ�ж��Ƿ�д��" << endl;
	unsigned char *content;
	uhf->readUser(l, content);    //��ȡ�û�data��Ϣ�����
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

//opΪ���Ĳ������� 0x00 ����  0X01����     ,0X02������    ,0X03������������
//mem ������Ӧ�ڴ�����ַ
//psw �����׵�ַ
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

	if (uhf->getInventoryRSSI(EPC, tagNum, EPCLen, intRSSI, RSSI_I, RSSI_Q, freq)) {//EPC LEN 12,tagname ��ʾɨ��ı�ǩ��
		cout << endl << "ɨ�赽" << tagNum << "����ǩ,�������� ��" << endl;
		for (int i = 0; i < tagNum; i++) {
			cout << "ɨ�赽�ĵ�  " << i + 1 << "  ����ǩ,�������� ��" << endl;
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
	cout << "***                ��ӭ����RFID��дʵ��                         ***" << endl;
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