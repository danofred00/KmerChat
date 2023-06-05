#ifndef TEST_H
#define TEST_H

#include <QtWebSockets>
#include <QThread>
#include <QObject>
#include <QDebug>

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject * parent = nullptr);

private slots:

    void connected();

    void messageReceived(const QByteArray & message);

signals:
    void finish();

private:

    void login();

    void logout();

    void signout();

    void signin();

    QWebSocket client;
};

#endif // TEST_H
