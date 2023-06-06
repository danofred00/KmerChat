#include "dialog.h"
#include "ui_dialog.h"

#include <QListView>
#include <QAbstractListModel>
#include <QStringListModel>

using namespace Client::Gui;

Dialog::Dialog(QStringList messages, QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QListView * list = ui->messages;

    QAbstractListModel * model = new QStringListModel(messages);
    list->setModel(model);
}

Dialog::~Dialog()
{
    delete ui;
}
