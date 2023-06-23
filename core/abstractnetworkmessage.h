#ifndef ABSTRACTNETWORKMESSAGE_H
#define ABSTRACTNETWORKMESSAGE_H

#include <QObject>
#include <QJsonDocument>

namespace Core {

using Headers = QMap<QString, int>;

class AbstractNetworkMessage : public QObject
{
    Q_OBJECT
public:

    enum Type {
        Simple,
        Login,
        Logout,
        Register,
        UnRegister,
    };

    AbstractNetworkMessage(QObject * parent = nullptr);

    AbstractNetworkMessage(const AbstractNetworkMessage & other);

    virtual ~AbstractNetworkMessage();

    Headers headers() const { return mHeaders; }

    QString content() const { return mContent; }

    virtual QString toString();

    virtual void parse(const QString & request);

    void setHeaders(const Headers &newHeaders) { mHeaders = newHeaders; }

    void setContent(const QString &newContent) { mContent = newContent; }
    // to update the value of the JsonContent when the content changed, call this method in setContent
    //setJsonContent(QJsonDocument::fromJson(newContent.toLatin1()));

    void addHeader(const QString & h, const int & v);

    AbstractNetworkMessage operator =(const AbstractNetworkMessage & message);

protected:
    QJsonDocument getJsonContent() const { return mJsonContent; }
    void setJsonContent(const QJsonDocument &newContent) { mJsonContent = newContent; }
    void updateJsonContent();

private:
    Headers mHeaders;
    QString mContent;
    QJsonDocument mJsonContent;
};

} // namespace Core

#endif // ABSTRACTNETWORKMESSAGE_H
