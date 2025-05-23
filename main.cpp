#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include "mainwindow.h"
#include "authdialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    while (true) {
        AuthDialog authDialog;
        if (authDialog.exec() != QDialog::Accepted)
            return 0;

        QString login = authDialog.getLogin();
        QString password = authDialog.getPassword();

        MainWindow window(login, password);
        window.setWindowTitle("PassGuard — Менеджер паролей");
        window.resize(600, 400);
        window.show();

        int result = app.exec();
        if (result != 42) // если не выход из аккаунта
            break;
    }

    return 0;
}

