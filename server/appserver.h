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
    explicit AppServer(int port, QObject *parent = nullptr);

    ~AppServer();

    void start();

signals:
    void closed(int code);

private:

    void close();

    int port;
    UserModel * userModel;

};

} // namespace Server

#endif // SERVER_H
