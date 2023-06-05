
#include <QJsonDocument>
#include <QJsonObject>

#include <exception>
#include "message.h"

using namespace Core;

/**
 * @brief Message::Message
 * @param type
 * @param content
 * @param state
 * @param parent
 */

Message::Message(MessageType type,
                 QString content,
                 MessageState state,
                 QObject *parent) :
    mType(type), mContent(content), mState(state), QObject(parent)
{
    setUserFrom(0);
    setId(0);
    setUserTo(0);
}

/**
 * @brief Message::Message the Copy constructor
 * @param other
 */
Message::Message(const Message & other)
{
    setContent(other.content());
    setState(other.state());
    setType(other.type());
    setUserFrom(other.from());
    setUserTo(other.to());
    setId(other.id());
}

/**
 * @return QString a message object in JsonString form
*/
QString Message::toString()
{
    QJsonDocument json;
    QJsonObject obj;

    //
    obj.insert(QStringLiteral("type"), QJsonValue(msgTypeToStr(type())));
    obj.insert(QStringLiteral("state"), QJsonValue(msgStateToStr(state())));
    obj.insert(QStringLiteral("from"), QJsonValue((qint64) from()));
    obj.insert(QStringLiteral("to"), QJsonValue((qint64) to()));
    obj.insert(QStringLiteral("content"), QJsonValue(content()));
    obj.insert(QStringLiteral("id"), QJsonValue((qint64) id()));

    //
    json.setObject(obj);
    return QString(json.toJson(QJsonDocument::Compact));
}

/**
 * @brief Message::fromJsonString
 * @param json
 * @return
 *///////////
Message Message::fromJsonString(const QString &json)
{
    Message msg;
    QJsonDocument jsonDoc;
    QJsonParseError * error = nullptr;

    //
    jsonDoc = QJsonDocument::fromJson(json.toLatin1(), error);
    // if error
    if(error != nullptr)
        throw std::runtime_error("Error while parsing the Json String");
    // else
    msg.setType(Message::strToMessageType(jsonDoc["type"].toString()));
    msg.setState(Message::strToMessageState(jsonDoc["state"].toString()));
    msg.setUserFrom(jsonDoc["from"].toInteger());
    msg.setUserTo(jsonDoc["to"].toInteger());
    msg.setId(jsonDoc["id"].toInteger());
    msg.setContent(jsonDoc["content"].toString());
    //
    return msg;
}

/**
 * @brief Message::msgStateToStr
 * @param state
 * @return
 */
QString Message::msgStateToStr(MessageState state)
{
    switch (state) {
    case SEND:
        return QStringLiteral("SEND");
    case NOTSEND:
        return QStringLiteral("NOTSEND");
    case PENDING:
        return QStringLiteral("PENDING");
    default:
        return QStringLiteral("UNKNOW");
    }
}

/**
 * @brief Message::msgTypeToStr
 * @param type
 * @return
 */
QString Message::msgTypeToStr(MessageType type)
{
    switch (type) {
    case TEXT:
        return QStringLiteral("TEXT");
    case AUDIO:
        return QStringLiteral("AUDIO");
    case VIDEO:
        return QStringLiteral("VIDEO");
    case IMAGE:
        return QStringLiteral("IMAGE");
    case REQUEST:
        return QStringLiteral("REQUEST");
    default:
        return QStringLiteral("UNKNOW");
    }
}

/**
 * @brief Message::strToMessageType
 * @param type
 * @return MessageType if a string is reallu a messageType, but -1 if not
 */
Message::MessageType Message::strToMessageType(const QString &type)
{
    if (type == "TEXT")
        return TEXT;
    else if (type == "AUDIO")
        return AUDIO;
    else if (type == "VIDEO")
        return VIDEO;
    else if (type == "IMAGE")
        return IMAGE;
    else if (type == "REQUEST")
        return REQUEST;
    else
        return UNKNOWTYPE;
}

/**
 * @brief Message::strToMessageState
 * @param state
 * @return -1 if error occured while parsing the state
 */
Message::MessageState Message::strToMessageState(const QString &state)
{
    if (state == "SEND")
        return SEND;
    else if (state == "NOTSEND")
        return NOTSEND;
    else if (state == "PENDING")
        return PENDING;
    else
        return UNKNOWSTATE;
}
////
