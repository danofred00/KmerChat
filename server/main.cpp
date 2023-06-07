
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "appserver.h"

QString description() {
    return "The Server of KmerChatApp Client.";
}


int main(int argc, char *argv[])
{
    // main code
    QCoreApplication a{argc, argv};
    QCoreApplication::setApplicationName("KmerChat-Server");
    QCoreApplication::setApplicationVersion("0.0.1");
    QCoreApplication::setOrganizationName("cm.danofred.kmerchat.server");
    QCoreApplication::setOrganizationDomain("kmerchat.localhost");

    QCommandLineParser parser;
    parser.setApplicationDescription(description());
    parser.addHelpOption();
    parser.addVersionOption();
    // parser.addPositionalArgument("host", QCoreApplication::tr("The host address"));
    parser.addPositionalArgument("port", QCoreApplication::tr("The port of the server"));
    parser.process(QCoreApplication::arguments());

    bool ok = true;
    auto args = parser.positionalArguments();
    auto port = args.at(0).toInt(&ok);

    if((args.size() == 1) && ok){

        Server::AppServer * server = new Server::AppServer(port);
        QObject::connect(server, SIGNAL(closed(int)), &a, SLOT(quit()), Qt::QueuedConnection);
        server->start();

    } else {
        parser.showHelp();
    }

    return a.exec();
}
