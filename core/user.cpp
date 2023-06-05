#include "user.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <exception>
User::User(QObject *parent)
    : QObject{parent}
{

}

User::User(const User & other)
{
    setName(other.name());
    setId(other.id());
    setUsername(other.username());
    setEmail(other.email());
    setTel(other.tel());
    setPassword(other.password());
    setImage(other.image());
}

User User::fromJsonString(const QString json)
{
    User user;
    QJsonParseError * error = nullptr;
    auto jsonDoc = QJsonDocument::fromJson(json.toLatin1(), error);

    if(error != nullptr)
        throw std::runtime_error(error->errorString().toStdString());

    // if there's no error
    user.setName(jsonDoc["name"].toString());
    user.setId(jsonDoc["id"].toInteger());
    user.setUsername(jsonDoc["username"].toString());
    user.setEmail(jsonDoc["email"].toString());
    user.setTel(jsonDoc["tel"].toString());
    user.setPassword(jsonDoc["password"].toString());
    user.setImage(jsonDoc["image"].toString());

    return user;
}

QString User::toString()
{
    QJsonDocument json;
    QJsonObject obj;

    obj.insert("id", QJsonValue((qint64) id()));
    obj.insert("username", QJsonValue(username()));
    obj.insert("name", QJsonValue(name()));
    obj.insert("password", QJsonValue(password()));
    obj.insert("email", QJsonValue(email()));
    obj.insert("tel", QJsonValue(tel()));
    obj.insert("image", QJsonValue(image()));

    json.setObject(obj);

    return QString(json.toJson(QJsonDocument::Compact));
}
