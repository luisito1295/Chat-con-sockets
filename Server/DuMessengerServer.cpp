#include "DuMessengerServer.h"
#include "DuMessengerSocket.h"
#include <QTextStream>
#include <QDebug>

DuMessengerServer::DuMessengerServer(QObject *parent)
    :QTcpServer(parent)
{
}
bool DuMessengerServer::startServer(quint16 port)
{
   return listen(QHostAddress::Any, port );
}
void DuMessengerServer::incomingConnection(qintptr handle)
{
    auto socket=new DuMessengerSocket(handle, this);
    mSockets << socket;

    for(auto i:mSockets){
        QTextStream T(i);
        T << "Server: Se ha conectado:" << handle;
        i->flush();
    }

    connect(socket, &DuMessengerSocket::DuReadyRead, [&](DuMessengerSocket *S)
    {
        qDebug() << "Leido";
        QTextStream T(S);
        auto text = T.readAll();

        for(auto i : mSockets){
            QTextStream K(i);
            K << text;
            i->flush();
        }
    });

    connect(socket, &DuMessengerSocket::DuStateChanged,
            [&](DuMessengerSocket *S, int ST){
        qDebug() << "DuStateChanged";
        if(ST == QTcpSocket::UnconnectedState){
            qDebug() << "Desconectado";
            mSockets.removeOne(S);
            for(auto i:mSockets){
                QTextStream K(i);
                K << "Server: El cliente"
                  << S->socketDescriptor()
                  << "Se ha desconectado...";
                i->flush();
            }
        }
    });
}
