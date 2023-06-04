#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include "abstractnetworkmessage.h"

class Request : public AbstractNetworkMessage
{
    Q_OBJECT
public:

    enum Type {
        Simple,
        Login,
        Logout,
        Register,
        Unknow
    };

    Request(QObject *parent = nullptr);
};

#endif // REQUEST_H
