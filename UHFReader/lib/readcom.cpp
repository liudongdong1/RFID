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

    QString strkey="HARDWARE\\DEVICEMAP\\SERIALCOMM";//子键路径
    subkey = strkey.utf16();//类型转换

    if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_READ,&hKey)!=0)
    {
         QMessageBox::information(NULL, "Error", "can not access regedit or has no com port!");//无法打开注册表时返回error
         return -1;
    }

    for(int i=0;i<list.size();i++){
        commresult = "";

        keysize=sizeof(keyname);
        valuesize=sizeof(keyvalue);
        if(::RegEnumValue(hKey, i, keyname, &keysize, 0, &type, (BYTE*)keyvalue, &valuesize)==0)//列举键名和值
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
             QMessageBox::information(NULL, "Error",  "can not access regedit");//无法打开注册表时返回error
             ::RegCloseKey(hKey);//关闭注册表
             return -1;
        }

    }

    ::RegCloseKey(hKey);//关闭注册表


    return re;
}
