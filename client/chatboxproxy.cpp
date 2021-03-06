﻿#include "chatboxproxy.h"
void ChatBoxProxy::sendChatMessage(QString str){
    /*QChar* Iterator=str.data();
        QString strr[3];
        for(int i=0;i<3;i++){
            while(*Iterator!=' '&&!Iterator->isNull()){
                strr[i].append(*Iterator);
                Iterator++;
            }
            Iterator++;
        }
        emit sendMessage(strr[0],strr[1],strr[2]);*/
    emit sendMessage("toServer", "sendChatMessage", str);
}

void ChatBoxProxy::connectSignal(){
    QObject::connect(this->item,SIGNAL(sendMessage(QString)),this,SLOT(sendChatMessage(QString)));
    QObject::connect(this,SIGNAL(sendMessage(QString,QString,QString,QString,QString)),player,SLOT(receiveMessage(QString,QString,QString,QString,QString)));
    QObject::connect(player,SIGNAL(sendMessage(QString,QString,QString,QString,QString)),this,SLOT(receiveMessage(QString,QString,QString,QString,QString)));
    sendBox=this->item->findChild<QObject*>("sendBox");
}

void ChatBoxProxy::receiveMessage(QString str1, QString str2, QString str3, QString str4, QString str5){
    if(str1=="showChatMessage"){
        showChatMessage(str2);
    }else if(str1=="canChat") canChat();
    else if(str1=="cancelChat") cancelChat();
}

void ChatBoxProxy::showChatMessage(QString str){
    QMetaObject::invokeMethod(this->item, "showMessage", Q_ARG(QVariant, QVariant(str)));
}

void ChatBoxProxy::canChat(){
    if(!chat)
        sendBox->setProperty("canChat", QVariant(true));
    chat=1;
}

void ChatBoxProxy::cancelChat(){
    if(chat)
        sendBox->setProperty("canChat",QVariant(false));
    chat=0;
}
