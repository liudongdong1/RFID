#include "readcom.h"
#include "qt_windows.h"

#include <Windows.h>
#include <QSettings>
#include <QMessageBox>
#include <qdebug.h>


#pragma comment( lib, "Advapi32.lib" )

ReadCom::ReadCom()
{

}


ReadCom::~ReadCom()
{

}

int ReadCom::getComm(QList<QString> *comports, int index)
{
    QString commresult="";
    int _index = 0;
    int re = -1;

    QSettings *reg = new QSettings("HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM\\", QSettings::NativeFormat);
    QStringList list = reg->allKeys();

    QString strkey="HARDWARE\\DEVICEMAP\\SERIALCOMM";//�Ӽ�·��
    subkey = strkey.utf16();//����ת��

    if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_READ,&hKey)!=0)
    {
         QMessageBox::information(NULL, "Error", "can not access regedit or has no com port!");//�޷���ע���ʱ����error
         return -1;
    }

    for(int i=0;i<list.size();i++){
        commresult = "";

        keysize=sizeof(keyname);
        valuesize=sizeof(keyvalue);
        if(::RegEnumValue(hKey, i, keyname, &keysize, 0, &type, (BYTE*)keyvalue, &valuesize)==0)//�оټ�����ֵ
        {
//            qDebug()<<"valuesize"<<valuesize;
//            qDebug()<<"keyname"<<keyname;
//             qDebug()<<QString::fromWCharArray(keyname).indexOf("VCP");
              if(QString::fromWCharArray(keyname).indexOf("VCP") != -1 || QString::fromWCharArray(keyname).indexOf("Serial") != -1){
                  for(int j=0; j<valuesize; j++)
                  {
                        if(keyvalue[j]!=0x00)
                        {
                            commresult.append(keyvalue[j]);
//                            qDebug()<<"keyvalue"<<keyvalue[j];
                        }
                  }
//                  qDebug()<<"commresult"<<commresult;
                  comports->append(commresult);
                  if(QString::fromWCharArray(keyname).indexOf("VCP") != -1){
                     if(_index == index){
                        re = comports->size()-1;
                        _index++;
                     }
                     else{
                        _index++;
                     }
                  }
              }
        }
        else
        {
             QMessageBox::information(NULL, "Error",  "can not access regedit");//�޷���ע���ʱ����error
             ::RegCloseKey(hKey);//�ر�ע���
             return -1;
        }

    }

    ::RegCloseKey(hKey);//�ر�ע���


    return re;
}
