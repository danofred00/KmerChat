#include "test.h"

Test::Test(QObject *parent)
    : QObject(parent)
{
    QObject::connect(&client, SIGNAL(connected()), this, SLOT(connected()));

    QObject::connect(&client, &QWebSocket::textMessageReceived, this, [&](QString message) { messageReceived(message.toLatin1()); });

    QObject::connect(&client, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(messageReceived(QByteArray)));

    client.open(QUrl("ws://localhost:1234"));
}

void Test::messageReceived(const QByteArray &message) {

    qDebug() << "Message received : " << message;
    qDebug() << "---------------------";
}

void Test::connected() {
    qDebug() << "Connected to the server : " << client.localAddress() << ":" << client.localPort();

    login();
}

void Test::login() {

    client.sendBinaryMessage("{\"content\":{\"password\":\"password\",\"username\":\"admin\"},"
                             "\"headers\":{\"type\":1}}");
}

void Test::signout() {
    client.sendBinaryMessage("{\"content\":{\"password\":\"password\",\"username\":\"john\",\"name\":\"Donald\",\"email\":\"doe@localhost\",\"id\":9},"
                             "\"headers\":{\"type\":4}}");
}

void Test::signin() {
    client.sendBinaryMessage("{\"content\":{\"password\":\"password\",\"username\":\"john\",\"name\":\"Donald\",\"email\":\"doe@localhost\"},"
                             "\"headers\":{\"type\":3}}");
}

void Test::logout() {

    client.sendBinaryMessage("{\"content\":{\"password\":\"password\",\"username\":\"john\",\"id\":8},"
                             "\"headers\":{\"type\":2}}");

}
