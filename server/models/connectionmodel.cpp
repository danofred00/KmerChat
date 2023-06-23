#include "connectionmodel.h"

using namespace Server::Model;

ConnectionModel::ConnectionModel(QObject *parent)
    : QObject{parent}
{ }

QList<QWebSocket *> ConnectionModel::connection(const quint64 id)
{
    if(mConnections.contains(id))
        return mConnections.value(id);

    return QList<QWebSocket*>();
}

void ConnectionModel::addConnection(QWebSocket *s, const quint64 &id)
{
    // we starting by get list of clients for this user
    auto lst = (mConnections.contains(id))
                   ? mConnections.value(id)
                   : QList<QWebSocket *>();
    lst.append(s);
    // update value of list
    mConnections[id] = lst;
}

// to update later
void ConnectionModel::removeConnection(QWebSocket *s, const quint64 &id)
{
    if(!mConnections.contains(id))
        return;

    auto lst = mConnections.value(id);
    for(int i=0; i<lst.size(); i++) {
        if(s == lst[i]) {
            lst.remove(i);
            s->deleteLater();
            break;
        }
    }
}

// To update later
void ConnectionModel::removeAll(const quint64 &id)
{
//    auto lst = mConnections.value(id);
//    for(int i=0; i<lst.size(); i++) {
//        if(s == lst[i]) {
//            lst.remove(i);
//            s->deleteLater();
//        }
    //    }
}

bool ConnectionModel::contains(const quint64 id)
{
    return mConnections.contains(id);
}
