#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

namespace Client::Gui {

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QStringList messages, QWidget * parent = nullptr);
    ~Dialog();

private:

    Ui::Dialog *ui;
};

} // namespace Client::Gui

#endif // DIALOG_H
