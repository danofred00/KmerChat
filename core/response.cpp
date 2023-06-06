
#include "response.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

using namespace Core;

//Response::Response(QObject *parent)
//    : AbstractNetworkMessage{parent}
//{

//}

void Response::parse(const QString & s)
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
        hdrs.insert("code", h["code"].toInt());

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
