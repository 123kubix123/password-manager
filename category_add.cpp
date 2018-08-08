#include "category_add.h"
#include "ui_category_add.h"

category_add::category_add(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::category_add)
{
    ui->setupUi(this);
}

category_add::~category_add()
{
    delete ui;
}

void category_add::set_data(QString name, QString command)
{
    ui->name->setText(name);
    ui->command->setPlainText(command);
}

void category_add::on_save_clicked()
{
    emit category_saved("command");
}
