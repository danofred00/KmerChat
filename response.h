#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

class Response : public AbstractNetworkMessage
{
public:

    enum Code {
        SUCCESS = 200, FAILED = 400
    };

    Response(QObject *parent = nullptr);
};

#endif // RESPONSE_H
