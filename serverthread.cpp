#include "serverthread.h"

MySocket *socket;


extern QList<QList<int>> ipList;
extern QList<int> ipListt;
serverThread::serverThread(int socketDescriptor, QObject *parent) :
    QThread(parent), socketDescriptor(socketDescriptor)
{

}

serverThread::~serverThread()
{
    socket->close();
}

void serverThread::run()
{
    socket = new MySocket(socketDescriptor, 0);

    if (!socket->setSocketDescriptor(socketDescriptor))
        return ;
    //qDebug()<<socket->peerAddress().toString();
    QList<int> a;
    a.append(socketDescriptor);
    a.append(socket->peerAddress().toIPv4Address());
    ipList.append(a);
    ipListt.append(socket->peerAddress().toIPv4Address());
    for (int i = 0; i < ipListt.count(); i++)
    {
        for (int k = i + 1; k <  ipListt.count(); k++)
        {
            if ( ipListt.at(i) ==  ipListt.at(k))
            {
                ipListt.removeAt(k);
                k--;
            }
        }
    }
    connect(socket, &MySocket::disconnected, this, &serverThread::disconnectToHost);
    connect(socket, SIGNAL(revData(QString, QByteArray)), this, SLOT(recvData(QString, QByteArray)));
    connect(this, SIGNAL(sendDat(QByteArray, int)), socket, SLOT(sendMsg(QByteArray, int)));

    exec();
}

void serverThread::sendData(QByteArray data, int id)
{
    if (data == "")
        return ;

    emit sendDat(data, id);
}

void serverThread::recvData(QString peerAddr, QByteArray data)
{
    emit revData(peerAddr, data);
}

void serverThread::disconnectToHost()
{
    emit disconnectTCP(socketDescriptor);
    socket->disconnectFromHost();
}
