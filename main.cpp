#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include "mainwindow.h"
#include "authdialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const QString correctMasterPassword = "passguard2025";
    const QString dataFile = "data.dat";
    int attempts = 0;
    const int maxAttempts = 3;
    QString userPassword;

    if (QFile::exists(dataFile)) {
        // Запросить мастер-пароль
        while (attempts < maxAttempts) {
            AuthDialog dialog;
            if (dialog.exec() == QDialog::Accepted) {
                if (dialog.getPassword() == correctMasterPassword) {
                    userPassword = dialog.getPassword();
                    break;
                } else {
                    QMessageBox::warning(nullptr, "Ошибка", "Неверный мастер-пароль.");
                    attempts++;
                }
            } else {
                return 0;
            }
        }

        if (attempts >= maxAttempts) {
            QMessageBox::critical(nullptr, "Доступ запрещён", "Превышено количество попыток. Программа будет закрыта.");
            return 0;
        }
    } else {
        // Первый запуск — сразу пропускаем ввод
        userPassword = correctMasterPassword;
    }

    MainWindow w(userPassword);
    w.setWindowTitle("PassGuard — Менеджер паролей");
    w.resize(600, 400);
    w.show();
    return app.exec();
}
