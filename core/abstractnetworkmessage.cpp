
#include "abstractnetworkmessage.h"

#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

using namespace Core;

AbstractNetworkMessage::AbstractNetworkMessage(QObject * parent)
    : QObject(parent)
{

}

AbstractNetworkMessage::AbstractNetworkMessage(const AbstractNetworkMessage &other)
{
    setContent(other.content());
    setHeaders(other.headers());
}

AbstractNetworkMessage AbstractNetworkMessage::operator=(const AbstractNetworkMessage & message)
{
    AbstractNetworkMessage msg {message};
    return msg;
}

AbstractNetworkMessage::~AbstractNetworkMessage()
{

}

void AbstractNetworkMessage::updateJsonContent()
{
    setJsonContent(QJsonDocument::fromJson(mContent.toLatin1()));
}

QString AbstractNetworkMessage::toString()
{
    QJsonObject json;
    QJsonDocument doc;
    QJsonObject headers;
    QJsonObject jsonContent;

    // make the headers
    for(const auto & k : mHeaders.keys())
        headers.insert(k, QJsonValue(mHeaders[k]));

    // make jsonContent
    updateJsonContent();
    jsonContent = mJsonContent.object();

    //qDebug() <<

    json.insert("content", jsonContent);
    json.insert("headers", headers);

    doc.setObject(json);
    return QString(doc.toJson(QJsonDocument::Compact));
}

void AbstractNetworkMessage::parse(const QString & s)
{
    QJsonParseError error;
    QJsonDocument docContent;
    QJsonDocument doc = QJsonDocument::fromJson(s.toLatin1(), &error);

    if(error.error == QJsonParseError::NoError) {

        Headers hdrs;
        const auto h = doc["headers"].toObject();

        // not working
        //        for (const auto & v : h) {
        //            auto key = QJsonObject(v).keys().first();
        //            auto val = v.toInt();
        //            hdrs.insert(key, val);
        //        }

        // get headers one by one
        hdrs.insert("type", h["type"].toInt());

        // get the content
        docContent.setObject(doc["content"].toObject());

        // update the header property
        setHeaders(hdrs);
        setContent(QString(docContent.toJson(QJsonDocument::Compact)));

    } else {
        // if there's some error
        qDebug() << "Error while parsing the JsonString : " << error.errorString();
    }

}

void AbstractNetworkMessage::addHeader(const QString & h, const int & v)
{
    if(mHeaders.contains(h))
        return;

    mHeaders.insert(h, v);
}
