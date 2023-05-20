#include "chatservice.h"

ChatService::ChatService(QObject *parent)
    : QObject{parent}
{
    _instance = this;
}

// the unique instance of this object
ChatService * ChatService::_instance = nullptr;
