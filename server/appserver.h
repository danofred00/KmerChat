#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include "core/models/usermodel.h"

namespace Server {

using namespace Core::Model;

class AppServer : public QObject
{
    Q_OBJECT
public:
    explicit AppServer(QString host, int port, QObject *parent = nullptr);

    ~AppServer();

    void start();

private:

    void close();

    QString host;
    int port;

    UserModel * userModel;

};

} // namespace Server

#endif // SERVER_H
