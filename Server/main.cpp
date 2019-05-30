#include <QCoreApplication>
#include "DuMessengerServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DuMessengerServer Server;
    if(!Server.startServer(3333)){
        qDebug() << "Error:" << Server.errorString();
        return 1;
    }
    qDebug() << "Servidor en linea";
    return a.exec();
}
