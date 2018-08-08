#ifndef CATEGORY_ADD_H
#define CATEGORY_ADD_H

#include <QWidget>

namespace Ui {
class category_add;
}

class category_add : public QWidget
{
    Q_OBJECT

public:
    explicit category_add(QWidget *parent = 0);
    ~category_add();
    void set_data(QString name, QString command);

signals:
    void category_saved(QString category);

private slots:
    void on_save_clicked();

private:
    Ui::category_add *ui;
};

#endif // CATEGORY_ADD_H
