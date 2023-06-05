
#include "dbservice.h"

#include <QSqlError>

using namespace Server::Service ;


DbService * DbService::instance() {
    if(_instance == nullptr)
        // usage of uninstanciate object
        throw std::runtime_error("Unable to get instance of DbService, please run start first.");

    // if everything is okay
    return _instance;
}

void DbService::start(const QString & host, const int port, const QString & username, const QString & password)
{
    // instanciate the service of it's not already done
    if(_instance == nullptr)
    {
        _instance = new DbService(host, port, username, password);
    }
}

// the unique instance of this object
DbService * DbService::_instance = nullptr;
