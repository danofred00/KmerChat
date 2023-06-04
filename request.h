#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QMap>

using Headers = QMap<QString, int>;

class Request : public QObject
{
    Q_OBJECT
public:

    enum RequestType {
        Simple,
        Login,
        Logout,
        Register,
        Unknow
    };

    Request(QObject *parent = nullptr);

    Headers headers() const { return mHeaders; }

    QString content() const { return mContent; }

    QString toJsonString();

    void parse(const QString & request);

    void setHeaders(const Headers &newHeaders) { mHeaders = newHeaders; }

    void setContent(const QString &newContent) { mContent = newContent; }

    void addHeader(const QString & h, const int & v);


private:
    Headers mHeaders;
    QString mContent;
};

#endif // REQUEST_H
