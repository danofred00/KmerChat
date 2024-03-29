#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <QObject>
#include "core/services/abstractservice.h"

namespace Server::Service  {

using namespace Core;
using namespace Core::Service;

/**
 * @brief The ChatService class
 * Receive user's messages and tranfert messages to their destinations
 */
class ChatService : public AbstractService
{
    Q_OBJECT
public:

    // usage of singleton pattern to enable one only instance of ChatService::class


    static ChatService * instance();

    static void start();

    ~ChatService() {}

    static void stop() {
        qDebug() << "Closing the Service : " << serviceName();
        delete _instance;
    }

    static QString serviceName() {
        return QString("ChatService");
    }

public slots:

    void sendMessage(QString * msg);

    void readMessage();

signals:

    void messageReceive(QString * msg);

    void messageSend(QString * msg);

private:

    ChatService(QObject * parent = nullptr);
    static ChatService * _instance;

};

} // namespace Server

#endif // CHATSERVICE_H
