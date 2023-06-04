#include "request.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

Request::Request(QObject *parent)
    : QObject{parent}
{

}

QString Request::toJsonString()
{
    QJsonObject json;
    QJsonDocument doc;
    QJsonObject headers;


    for(const auto & k : mHeaders.keys())
        headers.insert(k, QJsonValue(mHeaders[k]));

    json.insert("headers", headers);
    json.insert("content", QJsonValue(mContent));

    doc.setObject(json);
    return QString(doc.toJson(QJsonDocument::Compact));
}

void Request::parse(const QString & request)
{
    QJsonParseError * error = nullptr;
    QJsonDocument doc;

    doc = QJsonDocument::fromJson(request.toLatin1(), error);
    if(error == nullptr) {

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

        // update the header property
        setHeaders(hdrs);
        setContent(doc["content"].toString());

    } else {
        // if there's some error
    }

}

void Request::addHeader(const QString & h, const int & v)
{
    if(mHeaders.contains(h))
        return;

    mHeaders.insert(h, v);
}
