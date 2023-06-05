#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include "abstractnetworkmessage.h"

namespace Core {

class Request : public AbstractNetworkMessage
{
    Q_OBJECT
public:

    Request(QObject *parent = nullptr);
};

} // namespace Core

#endif // REQUEST_H
