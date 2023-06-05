#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT
public:

    User(QObject *parent = nullptr);

    User(const User & other);

    // Builder class fot the current user
    class Builder {

        public:

            Builder hasName(const QString name)
            {
                mName = name;
                return *this;
            }

            Builder hasEmail(const QString email)
            {
                mEmail = email;
                return *this;
            }

            Builder hasImage(const QString image)
            {
                mImage = image;
                return *this;
            }

            Builder hasId(const quint64 id)
            {
                mId = id;
                return *this;
            }

            Builder hasTel(const QString tel)
            {
                mTel = tel;
                return *this;
            }

            Builder hasPassword(const QString password)
            {
                mPassword = password;
                return *this;
            }

            Builder hasUsername(const QString username)
            {
                mUsername = username;
                return *this;
            }

            /**
             * @brief build
             * @return User
             */
            User build() {

                User user;
                user.setName(mName);
                user.setEmail(mEmail);
                user.setPassword(mPassword);
                user.setId(mId);
                user.setImage(mImage);
                user.setUsername(mUsername);
                user.setTel(mTel);

                return user;
            }


        private:
            quint64 mId = 0;
            QString mUsername = "";
            QString mPassword = "";
            QString mEmail = "";
            QString mName = "";
            QString mTel = "";
            QString mImage = "";
    };


    // statics mthods
    static User fromJsonString(const QString json);

    QString toString();

    // getters

    const quint64 id() const { return mId; }

    const QString username() const { return mUsername; }

    const QString password() const { return mPassword; }

    const QString email() const { return mEmail; }

    const QString name() const { return mName; }

    const QString tel() const { return mTel; }

    const QString image() const { return mImage; }

    // setters

    void setId(const quint64 & id) { mId = id; }

    void setUsername(const QString & username) { mUsername = username; }

    void setPassword(const QString & password) { mPassword = password; }

    void setEmail(const QString & email ) { mEmail = email ; }

    void setTel(const QString & tel) { mTel = tel; }

    void setName(const QString & name) { mName = name; }

    void setImage(const QString & image) { mImage = image; }

    User operator=(const User & user)
    {
        User u{user};
        return u;
    }

private:

    quint64 mId;
    QString mUsername;
    QString mPassword;
    QString mEmail;
    QString mName;
    QString mTel;
    QString mImage;

};

#endif // USER_H
