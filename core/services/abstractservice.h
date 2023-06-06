#ifndef ABSTRACTSERVICE_H
#define ABSTRACTSERVICE_H

#include <QObject>
#include <QDebug>

#define DEFAULT_SERVICE_NAME "AbstractService"

namespace Core::Service {

/**
 * @brief The AbstractService class
 * A service can be started, and stoped
 */
class AbstractService : public QObject
{
    Q_OBJECT
protected:

    AbstractService(QObject * parent = nullptr) : QObject(parent)
    { }

public:

    virtual ~AbstractService() {
        // virtual destructor
    }

    static void start() {
        // method to start the service
        // this method should be overloaded
    }

    static void stop() {
        // method to stop the service
        // this method should be overloaded
    }

    static QString serviceName() {
        return QString(DEFAULT_SERVICE_NAME);
    }
};

} // namespace Core::Service

#endif // ABSTRACTSERVICE_H
