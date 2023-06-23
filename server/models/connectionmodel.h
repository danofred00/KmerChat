#ifndef CONNECTIONMODEL_H
#define CONNECTIONMODEL_H

#include <QObject>
#include <QWebSocket>

using Connections = QMap<quint64, QList<QWebSocket*>>;

namespace Server::Model {

class ConnectionModel : public QObject
{
    Q_OBJECT
public:

    ConnectionModel(QObject *parent = nullptr);

    Connections connections() const;

    QList<QWebSocket *> connection(const quint64 id);

    void setConnections(Connections & connections) { mConnections = connections; }

    void addConnection(QWebSocket * s, const quint64 & id);

    void removeConnection(QWebSocket * s, const quint64 & id);

    void removeAll(const quint64 & id);

    bool contains(const quint64 id);

    QList<QWebSocket*> operator[](const quint64 & id)
    {
        return connection(id);
    }

signals:

    void connectionAdded(const QWebSocket * s, const quint64 & id);

    void connectionRemoved(const QWebSocket * s, const quint64 & id);

private:

    Connections mConnections;

};

} // namespace Server::Model

#endif // CONNECTIONMODEL_H
