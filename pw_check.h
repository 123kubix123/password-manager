#ifndef PW_CHECK_H
#define PW_CHECK_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class pw_check;
}

class pw_check : public QDialog
{
    Q_OBJECT

public:
    explicit pw_check(QWidget *parent = 0);
    ~pw_check();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void pw_entered(QString pw);
    void force_close();

private slots:
    void on_ok_clicked();

private:
    Ui::pw_check *ui;
    bool has_pw = false;
};

#endif // PW_CHECK_H
