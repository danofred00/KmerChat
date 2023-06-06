
#include <QApplication>

#include <QDebug>

#include "appclient.h"

using namespace Client;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AppClient client;

    client.start("localhost", 1234);

    return app.exec();
}
