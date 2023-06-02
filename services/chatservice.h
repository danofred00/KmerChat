#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <QObject>
#include "abstractservice.h"

class DbService;

/**
 * @brief The ChatService class
 * Receive user's messages and tranfert messages to their destinations
 */
class ChatService : public AbstractService
{
    Q_OBJECT
public:

    // usage of singleton pattern to enable one only instance of ChatService::class
    static ChatService * instance() {
        if(_instance == nullptr)
            // usage of uninstanciate object
            throw std::runtime_error("Unable to get instance of ChatService, please run start first.");

        // if everything is okay
        return _instance;
    }

    ~ChatService() {}

    static void stop() {
        delete _instance;
    }

signals:

private:

    ChatService(DbService * db);

    static ChatService * _instance;
    DbService * db;

};

#endif // CHATSERVICE_H
