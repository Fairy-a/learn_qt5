#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>

namespace Ui {
class MyDialog;
}

class MyDialog : public QDialog
{
    Q_OBJECT
signals:
    void dlgReturn(int);
public:
    explicit MyDialog(QWidget *parent = nullptr);
    ~MyDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MyDialog *ui;
};

#endif // MYDIALOG_H
