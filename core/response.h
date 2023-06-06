#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

namespace Core {

class Response : public AbstractNetworkMessage
{
    Q_OBJECT
public:

    enum Code {
        SUCCESS = 200, FAILED = 400
    };

    using AbstractNetworkMessage::AbstractNetworkMessage;

    // Response(QObject *parent = nullptr);

    void parse(const QString & response);
};

} // namespace Core

#endif // RESPONSE_H
