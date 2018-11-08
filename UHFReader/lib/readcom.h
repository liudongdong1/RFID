#ifndef READCOM_H
#define READCOM_H


class ReadCom
{
public:
    ReadCom();
    ~ReadCom();
private:
    HKEY hKey;
    LPCWSTR subkey;

    wchar_t keyname[256]; //��������
    char keyvalue[256];  //��ֵ����
    DWORD keysize,type,valuesize;

public:
    int getComm(QList<QString> *comports, int index);

};

#endif // READCOM_H
