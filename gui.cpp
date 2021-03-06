#include "gui.h"
#include "ui_gui.h"
#include "pw_check.h"


gui::gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gui)
{
    ui->setupUi(this);
    initialize();

}

gui::~gui()
{
    delete ui;
}

void gui::initialize()
{
    this->setEnabled(false); // disable ui until password verification succeded
    //on_actionFullscreen_triggered();
    //this->setWindowFlag(Qt::WindowStaysOnTopHint);
    //on_actionFullscreen_triggered();
    decrypt(); // access encrypted file
}

void gui::decrypt()
{/*
    pw_check *check = new pw_check();
    connect(check, SIGNAL(pw_entered(QString)), this, SLOT(password_check(QString)));
    connect(check, SIGNAL(force_close()), this, SLOT(shutdown()));
    check->setWindowFlags(Qt::WindowStaysOnTopHint);
    check->show();
    //QTimer::singleShot(500,check, SLOT(raise())); // qtimers to delay code execution and ensure proper loading (pw window over main window)
    QTimer::singleShot(0,check, SLOT(focus_to_edit()));
    */
    QInputDialog *inp = new QInputDialog(this);
    inp->setLabelText("Weryfikacja hasła");
    inp->setWindowTitle("Hasło");
    inp->setTextEchoMode(QLineEdit::Password);
    inp->setWindowFlags(Qt::Dialog|Qt::WindowTitleHint|Qt::WindowSystemMenuHint|Qt::WindowCloseButtonHint);
    inp->adjustSize();
    inp->move(QApplication::desktop()->screen()->rect().center() - inp->rect().center());
    if(inp->exec() == QDialog::Accepted && !inp->textValue().isEmpty())
    {
        password_check(inp->textValue());
    }
    else
    {
        QTimer::singleShot(0,this, &gui::on_actionWyloguj_triggered);
    }
    inp->deleteLater();
}

void gui::password_check(QString pw)
{
    // use script to verify pw
    pw = "novell";
    QString result = execute_command("../../haslo.sh " + pw + " vp");
    if(result == "OK")
    {
        password = pw;
        this->setEnabled(true); // activate ui
        load_categories();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Nieprawidłowe hasło!!!");
        decrypt(); // restart pw verification
    }
}


QString gui::execute_command(QString command)
{
    QProcess bash;
    bash.start("/bin/bash");
    command = command + " && exit || exit\n";
    qDebug()<<command;
    bash.write(command.toStdString().c_str());
    bash.waitForFinished();
    QString result = bash.readAllStandardOutput();
    bash.close();
    result.chop(1);
    return result;
}


void gui::load_categories()
{
    bool loaded = false;
    ui->categories->clear();
    // use script to load categories
    QString categories = execute_command("../../haslo.sh " + password + " sc ");
    QStringList categories_splitted = categories.split("\n");
    categories_splitted.removeDuplicates();
    for(int i =0; i < categories_splitted.count(); i++)
    {
        if(categories_splitted.at(i).isEmpty() || categories_splitted.at(i) == " ")
        {
            continue;
        }
        ui->categories->addItem(categories_splitted.at(i));
        loaded = true;
    }

    if(loaded)
    {
        connect(ui->categories, SIGNAL(currentIndexChanged(QString)), this, SLOT(load_entries(QString)));
        load_entries(ui->categories->currentText());
    }
    else
    {
        disconnect(ui->categories, SIGNAL(currentIndexChanged(QString)), this, SLOT(load_entries(QString)));
        QMessageBox::warning(this, "Error", "Nie załadowano kategorii!");
    }
}

void gui::load_entries(QString category)
{
    ui->entries->clear();
    // use script to load entries
    QString entries = execute_command("../../haslo.sh " + password + " se "+category);
    QList<QString> entries_splitted = entries.split("\n");
    for(int i =0; i < entries_splitted.count(); i++)
    {
        if(entries_splitted.at(i).isEmpty() || entries_splitted.at(i) == " ")
        {
            continue;
        }
        QList<QString> entry_splitted = entries_splitted.at(i).split("<trelemorele>");
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(0,entry_splitted.at(2) + " " + entry_splitted.at(3) + ":" + entry_splitted.at(4));
        item->setData(3,entry_splitted.at(1));
        ui->entries->addItem(item);
    }
}

void gui::on_actionFullscreen_triggered()
{
    this->showFullScreen();
}

void gui::on_actionStandard_triggered()
{
    this->showNormal();
}



void gui::closeEvent(QCloseEvent *event)
{
    close_requested = true;
    if(close_requested)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void gui::on_actionZamknij_triggered()
{
    shutdown();
}

void gui::shutdown()
{
    close_requested = true;
    // bash systemctl poweroff
    QProcess poweroff;
   // poweroff.startDetached("/bin/bash", QStringList()<< "-c" << "systemctl poweroff;\n");
    emit closing();
    this->close();
}

void gui::on_actionWyloguj_triggered()
{
    close_requested = true;
    QProcess poweroff;
    //poweroff.startDetached("/bin/bash", QStringList()<< "-c" << "kill -9 -1;\n");
    emit closing();
    this->close();
   // QApplication::exit(0);
}

void gui::on_show_pw_clicked()
{
    // use script to decrypt pw
    if(ui->entries->currentItem() != NULL)
    {
        QString entries = execute_command("../../haslo.sh " + password + " se "+ui->categories->currentText());
        QList<QString> entries_splitted = entries.split("\n");
        for(int i =0; i < entries_splitted.count(); i++)
        {
            if(entries_splitted.at(i).isEmpty() || entries_splitted.at(i) == " ")
            {
                continue;
            }
            QList<QString> entry_splitted = entries_splitted.at(i).split("<trelemorele>");
            if(entry_splitted.at(1) == ui->entries->currentItem()->data(3))
            {
                QMessageBox::information(this, ui->entries->currentItem()->text(), entry_splitted.at(5));
            }
        }
    }
    else
    {
       QMessageBox::warning(this, "Error", "Nie wybrano wpisu!");
    }
}

void gui::on_change_pw_clicked()
{
    // use script to  change pw
    if(ui->entries->currentItem() != NULL)
    {
        bool user_ok;
        QString username = QInputDialog::getText(this, tr("Podaj użytkownika któremu chcesz zmienić hasło: "),
                                             tr("Nazwa użytkownika na ")+ui->entries->currentItem()->text(), QLineEdit::Normal,
                                             "Username", &user_ok);
        if (user_ok && !username.isEmpty())
        {
            bool pw_ok;
            QString new_pw = QInputDialog::getText(this, tr("Podaj nowe hasło do ")+ui->entries->currentItem()->text(),
                                                 tr("Nowe hasło: "), QLineEdit::Password,
                                                 "Password", &pw_ok);
            if (pw_ok && !new_pw.isEmpty())
            {
                QString result = execute_command("../../haslo.sh " + password + " cp "+ui->entries->currentItem()->data(3).toString()+" "+username+" "+new_pw);
                update_log(result);
            }
            else
            {
                QMessageBox::information(this, "Anulowano", "Hasło nie zostało zmienione!");
            }
        }
        else
        {
            QMessageBox::information(this, "Anulowano", "Hasło nie zostało zmienione!");
        }
    }
    else
    {
       QMessageBox::warning(this, "Error", "Nie wybrano wpisu!");
    }
}

void gui::on_entry_delete_clicked()
{
    // use script to delete entry
    if(ui->entries->currentItem() != NULL)
    {
        QString result = execute_command("../../haslo.sh " + password + " de "+ui->entries->currentItem()->data(3).toString());
        if(result == "OK")
        {
           QMessageBox::information(this, "Usunięto", "Usunięto wpis.");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Coś poszło nie tak!!!\n"+result);
        }
        load_categories();
    }
    else
    {
       QMessageBox::warning(this, "Error", "Nie wybrano wpisu!");
    }
}

void gui::on_category_delete_clicked()
{
    // use script to delete category
    if(ui->categories->currentText() != "")
    {
        QString result = execute_command("../../haslo.sh " + password + " dc "+ui->categories->currentText());
        if(result == "OK")
        {
           QMessageBox::information(this, "Usunięto", "Usunięto kategorię.");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Coś poszło nie tak!!!\n"+result);
        }
        load_categories();
    }
    else
    {
       QMessageBox::warning(this, "Error", "Nie wybrano kategorii!");
    }
}

void gui::verify_category(QString name, QString category)
{
    // use script to verify category
    QString result = execute_command("../../haslo.sh " + password + " vc "+name);
    qDebug()<<result;
    if(result == "OK")
    {
        // use script to add category
        QString result = execute_command("../../haslo.sh " + password + " ac "+name + " " + category);
        if(result != "OK")
        {
            QMessageBox::warning(this, "Error", "Coś poszło nie tak!!!\n"+result);
        }
        load_categories();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Kategoria już istnieje!");
        show_add_category_window(name,category);
    }
}

void gui::on_actionDodaj_Kategori_triggered()
{
    show_add_category_window("","");
}

void gui::show_add_category_window(QString name, QString command)
{
    category_add *new_category = new category_add();
    new_category->set_data(name,command);
    new_category->setWindowFlags(Qt::WindowStaysOnTopHint);
    connect(new_category, SIGNAL(category_saved(QString,QString)), this, SLOT(verify_category(QString,QString)));
    connect(new_category, SIGNAL(category_saved()), new_category, SLOT(close()));
    connect(new_category, SIGNAL(category_saved()), new_category, SLOT(deleteLater()));  // may cause problems on non supported qt version eg. 5.9.4
    connect(this, SIGNAL(closing()), new_category, SLOT(close()));
    new_category->show();
}

void gui::update_log(QString text)
{
    text = text+"\n";
    ui->log->append(text);
    QScrollBar *down = ui->log->verticalScrollBar();
    down->setValue(down->maximum());
}

void gui::on_actionZmie_has_o_g_wne_triggered()
{
    QInputDialog *inp = new QInputDialog(this);
    inp->setLabelText("Nowe Hasło:");
    inp->setWindowTitle("Hasło");
    inp->setTextEchoMode(QLineEdit::Password);
    inp->setWindowFlags(Qt::Dialog|Qt::WindowTitleHint|Qt::WindowSystemMenuHint|Qt::WindowCloseButtonHint);
    inp->adjustSize();
    inp->move(QApplication::desktop()->screen()->rect().center() - inp->rect().center());
    if(inp->exec() == QDialog::Accepted && !inp->textValue().isEmpty())
    {
        QString result = execute_command("../../haslo.sh " + password + " cmp "+inp->textValue());
        if(result == "OK")
        {
            password = inp->textValue();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Hasło nie zostało zmienione!");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Hasło nie zostało zmienione!");
    }
    inp->deleteLater();
}

void gui::on_actionDodaj_komputer_triggered()
{
 // not implemented yet
}
