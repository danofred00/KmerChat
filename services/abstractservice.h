#ifndef ABSTRACTSERVICE_H
#define ABSTRACTSERVICE_H

#include <QObject>

/**
 * @brief The AbstractService class
 * A service can be started, and stoped
 */
class AbstractService : public QObject
{
    Q_OBJECT
public:

    AbstractService(QObject * parent = nullptr) : QObject(parent)
    { }

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
};

#endif // ABSTRACTSERVICE_H
