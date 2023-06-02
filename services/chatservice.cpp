#include "chatservice.h"
#include "dbservice.h"

ChatService::ChatService(DbService * db)
    : db(db), AbstractService(nullptr)
{
    _instance = this;
}

// the unique instance of this object
ChatService * ChatService::_instance = nullptr;
