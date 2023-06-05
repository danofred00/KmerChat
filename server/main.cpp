
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

    QCommandLineParser parser;
    parser.setApplicationDescription(description());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("host", QCoreApplication::tr("The host address"));
    parser.addPositionalArgument("port", QCoreApplication::tr("The port of the server"));
    parser.process(QCoreApplication::arguments());

    auto args = parser.positionalArguments();

    if(args.size() == 2){

        Server::AppServer server = Server::AppServer(args.at(0), args.at(1).toInt());

        server.start();

    } else {
        parser.showHelp();
    }

    return a.exec();
}
