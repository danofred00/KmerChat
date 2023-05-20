#include <QtTest>

// add necessary includes here

class UserTest : public QObject
{
    Q_OBJECT

public:
    UserTest() {

    }

    ~UserTest() {

    }

private slots:
    void test_create() {

        int i=0;

        QVERIFY(i == 0);
    }

};

QTEST_MAIN(UserTest)
#include "test_message.moc";
