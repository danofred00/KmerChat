#include "user.h"
#include "utils.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <exception>

using namespace Core;

User::User(QObject *parent)
    : QObject{parent}
{

}

User::User(QMap<QString, QVariant> user, QObject * parent)
    : QObject(parent)
{

    mId = user.value("id").toInt();
    mName = user.value("name").toString();
    mUsername = user.value("username").toString();
    mEmail = user.value("email").toString();
    mTel = user.value("tel").toString();
    // hash the password
    mPassword = Utils::hash(user.value("password").toByteArray());
    mImage = user.value("image").toString();
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

    auto _id = id();
    auto _username = username();
    auto _name = name();
    auto _password = password();
    auto _email = email();
    auto _tel = tel();
    auto _image = image();

    if(_id != 0)
        obj.insert("id", QJsonValue((qint64) _id));
    if(!_username.isEmpty())
        obj.insert("username", QJsonValue(_username));
    if(!_name.isEmpty())
        obj.insert("name", QJsonValue(_name));
    if(!_password.isEmpty())
        obj.insert("password", QJsonValue(_password));
    if(!_email.isEmpty())
        obj.insert("email", QJsonValue(_email));
    if(!_tel.isEmpty())
        obj.insert("tel", QJsonValue(_tel));
    if(!_image.isEmpty())
        obj.insert("image", QJsonValue(_image));

    json.setObject(obj);

    return QString(json.toJson(QJsonDocument::Compact));
}
