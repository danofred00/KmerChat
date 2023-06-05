
#include "chatservice.h"

using namespace Server::Service ;

ChatService::ChatService(QObject * parent)
    : AbstractService{parent}
{ }

// the unique instance of this object
ChatService * ChatService::_instance = nullptr;

ChatService * ChatService::instance()
{
    if(_instance == nullptr)
        // usage of uninstanciate object
        throw std::runtime_error("Unable to get instance of ChatService, please run start first.");

    // if everything is okay
    return _instance;
}

void ChatService::start()
{
    if(_instance == nullptr)
        _instance = new ChatService();
}


void ChatService::readMessage()
{

}

void ChatService::sendMessage(QString *msg)
{

}
