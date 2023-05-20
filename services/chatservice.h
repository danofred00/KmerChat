#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <QObject>

class ChatService : public QObject
{
    Q_OBJECT
public:

    // usage of singleton pattern to enable one only instance of ChatService::class
    static ChatService * instance() {
        if(_instance != nullptr)
            return _instance;

        _instance = new ChatService();
        return _instance;
    }

    ~ChatService() {
        delete _instance;
    }

signals:

private:

    ChatService(QObject *parent = nullptr);

    static ChatService * _instance;

};

#endif // CHATSERVICE_H
