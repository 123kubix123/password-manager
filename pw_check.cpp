#include "pw_check.h"
#include "ui_pw_check.h"

pw_check::pw_check(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pw_check)
{
    ui->setupUi(this);
}

pw_check::~pw_check()
{
    delete ui;
}

void pw_check::on_ok_clicked()
{
    QString pw = ui->pw->text();
    has_pw = true;
    emit pw_entered(pw);
    this->close();
}



void pw_check::closeEvent(QCloseEvent *event)
{
    if(has_pw)
    {
        event->accept();
        this->deleteLater();
    }
    else
    {
        emit force_close();
        this->deleteLater();
    }

}

void pw_check::on_pw_returnPressed()
{
    on_ok_clicked();
}

void pw_check::focus_to_edit()
{
    this->raise();
    this->activateWindow();
    this->setWindowState(Qt::WindowActive);
    ui->pw->setFocus();
    //this->setFocus();
    //QTimer::singleShot(0, ui->pw, SLOT(setFocus()));

}
