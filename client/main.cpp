
#include <QCoreApplication>
#include "test.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	
	Test test = Test();

    QObject::connect(&test, &Test::finish, &a, &QCoreApplication::quit);

    return a.exec();
}
