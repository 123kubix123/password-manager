#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>
#include <QDialog>
#include <QCloseEvent>
#include <QTimer>
#include <QMessageBox>
#include <QProcess>
#include <QDir>
#include <QInputDialog>
#include <QDateTime>
#include <QScrollBar>


#include "pw_check.h"
#include "category_add.h"

namespace Ui {
class gui;
}

class gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit gui(QWidget *parent = 0);
    ~gui();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void closing();

private slots:
    void on_actionFullscreen_triggered();

    void on_actionStandard_triggered();

    void initialize();

    void load_categories();

    void load_entries(QString category);

    void on_show_pw_clicked();

    void decrypt();

    void password_check(QString pw);

    void shutdown();

    void on_actionZamknij_triggered();

    void on_actionWyloguj_triggered();

    void on_change_pw_clicked();

    void on_entry_delete_clicked();

    void on_category_delete_clicked();

    void verify_category(QString name, QString category);

    void on_actionDodaj_Kategori_triggered();

    QString execute_command(QString command);

    void update_log(QString text);

    void show_add_category_window(QString name, QString command);

private:
    Ui::gui *ui;
    bool close_requested = false;
    QString password = NULL;
};

#endif // GUI_H
