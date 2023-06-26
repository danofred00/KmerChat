#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

namespace Core {

class Message : public QObject
{
    Q_OBJECT
public:

    enum MessageType {
        TEXT, IMAGE, AUDIO, VIDEO, REQUEST, UNKNOWTYPE
    };

    enum MessageState { SEND, PENDING, NOTSEND, UNKNOWSTATE };

    Message(const Message & other);

    Message(QString content = QString(""), MessageType type = TEXT, MessageState state = MessageState::NOTSEND, QObject * parent = nullptr);

    // getters
    const QString content() const { return mContent; }

    const quint64 chatId() const { return mChatId; }

    const quint64 userId() const { return mUserId; }

    const quint64 id() const { return mId;}

    MessageType type() const { return mType; }

    MessageState state() const { return mState; }

    // setters

    void setContent(const QString & content) { mContent = content; }

    void setState(MessageState state) { mState = state; }

    void setType(MessageType type) { mType = type; }

    void setChatId(const quint64 id) { mChatId = id; }

    void setUserId(const quint64 id) { mUserId = id; }

    void setId(const quint64 id) { mId = id; }

    // utils

    static Message * fromJsonString(const QString & json);

    QString toString();

    static QString msgTypeToStr(MessageType type);

    static QString msgStateToStr(MessageState state);

    static MessageType strToMessageType(const QString & type);

    static MessageState strToMessageState(const QString & state);

    Message operator=(const Message & message) {
        Message msg{message};
        return msg;
    }


private:

    QString mContent;
    quint64 mChatId;
    quint64 mUserId;
    quint64 mId;
    MessageState mState;
    MessageType mType;
};

} // namespace Core

#endif // MESSAGE_H
