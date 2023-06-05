#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <QObject>
#include "core/services/abstractservice.h"


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
        delete _instance;
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

#endif // CHATSERVICE_H
