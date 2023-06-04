#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

class Response : public AbstractNetworkMessage
{
public:

    enum Type {
        LoginOk,
        LoginFailed,
        RegisterOk
    };

    Response(QObject *parent = nullptr);
};

#endif // RESPONSE_H
