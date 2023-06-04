#ifndef ABSTRACTNETWORKMESSAGE_H
#define ABSTRACTNETWORKMESSAGE_H

#include <QObject>
#include <QJsonDocument>

using Headers = QMap<QString, int>;

class AbstractNetworkMessage : public QObject
{
    Q_OBJECT
public:
    AbstractNetworkMessage(QObject * parent = nullptr);

    Headers headers() const { return mHeaders; }

    QString content() const { return mContent; }

    QJsonDocument getJsonContent() const { return mJsonContent; }

    QString toJsonString();

    void parse(const QString & request);

    void setHeaders(const Headers &newHeaders) { mHeaders = newHeaders; }

    void setContent(const QString &newContent) { mContent = newContent; }
    // to update the value of the JsonContent when the content changed, call this method in setContent
    //setJsonContent(QJsonDocument::fromJson(newContent.toLatin1()));

    void setJsonContent(const QJsonDocument &newContent) { mJsonContent = newContent; }

    void addHeader(const QString & h, const int & v);

    void updateJsonContent();


private:
    Headers mHeaders;
    QString mContent;
    QJsonDocument mJsonContent;
};

#endif // ABSTRACTNETWORKMESSAGE_H
