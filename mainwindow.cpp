#include "mainwindow.h"
#include "qrdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>

#include <QToolButton>
#include <QMenu>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QDebug>

#include <QApplication>

MainWindow::MainWindow(const QString &login, const QString &password, QWidget *parent)
    : QMainWindow(parent), currentLogin(login), masterPassword(password) {
    setupUI();
    QDir().mkpath("users");  // создаёт папку, если её нет

    bool success = manager.loadFromFile("users/" + currentLogin + ".dat", masterPassword);
    if (!success) {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить пароли. Возможно, неверный пароль или повреждён файл.");
    }

    refreshTable();

}


MainWindow::~MainWindow() {
    manager.saveToFile("users/" + currentLogin + ".dat", masterPassword);
}


void MainWindow::setupUI() {
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    table = new QTableWidget(0, 4, this);
    table->setHorizontalHeaderLabels({"Сервис", "Логин", "Пароль", "Заметка"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QPushButton *addBtn = new QPushButton("Добавить");
    QPushButton *editBtn = new QPushButton("Редактировать");
    QPushButton *removeBtn = new QPushButton("Удалить");

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addPassword);
    connect(editBtn, &QPushButton::clicked, this, &MainWindow::editPassword);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::removePassword);

    layout->addWidget(table);
    layout->addWidget(addBtn);
    layout->addWidget(editBtn);
    layout->addWidget(removeBtn);

    setCentralWidget(central);

    QPushButton *importBtn = new QPushButton("Импорт");
    QPushButton *exportBtn = new QPushButton("Экспорт");

    connect(importBtn, &QPushButton::clicked, this, &MainWindow::importPasswords);
    connect(exportBtn, &QPushButton::clicked, this, &MainWindow::exportPasswords);

    layout->addWidget(importBtn);
    layout->addWidget(exportBtn);

    layout->addSpacing(10); // между группами

    QPushButton *qrBtn = new QPushButton("QR-код");
    connect(qrBtn, &QPushButton::clicked, this, &MainWindow::showQrCode);
    layout->addWidget(qrBtn);

    createSettingsMenu();
}

// void MainWindow::setMasterPassword(const QString &password) {
//     masterPassword = password;
//     manager.loadFromFile("data.dat", masterPassword);
//     refreshTable();
// }


void MainWindow::refreshTable() {
    table->setRowCount(0);
    const auto &entries = manager.getEntries();

    qDebug() << "Загружено записей:" << entries.size();

    for (int i = 0; i < entries.size(); ++i) {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(entries[i].service));
        table->setItem(i, 1, new QTableWidgetItem(entries[i].username));
        table->setItem(i, 2, new QTableWidgetItem(entries[i].password));
        table->setItem(i, 3, new QTableWidgetItem(entries[i].note));
    }
}

void MainWindow::addPassword() {
    PasswordEntry entry;
    entry.service = QInputDialog::getText(this, "Сервис", "Введите название сервиса:");
    entry.username = QInputDialog::getText(this, "Логин", "Введите логин:");
    entry.password = QInputDialog::getText(this, "Пароль", "Введите пароль:", QLineEdit::Password);
    entry.note = QInputDialog::getText(this, "Заметка", "Дополнительная информация:");

    if (!entry.service.isEmpty() && !entry.username.isEmpty()) {
        manager.addEntry(entry);
        manager.saveToFile("users/" + currentLogin + ".dat", masterPassword);
        refreshTable();
    }
}


void MainWindow::editPassword() {
    int row = table->currentRow();
    if (row < 0) return;

    PasswordEntry entry = manager.getEntries()[row];

    entry.service = QInputDialog::getText(this, "Сервис", "Изменить сервис:", QLineEdit::Normal, entry.service);
    entry.username = QInputDialog::getText(this, "Логин", "Изменить логин:", QLineEdit::Normal, entry.username);
    entry.password = QInputDialog::getText(this, "Пароль", "Изменить пароль:", QLineEdit::Password, entry.password);
    entry.note = QInputDialog::getText(this, "Заметка", "Изменить заметку:", QLineEdit::Normal, entry.note);

    if (!entry.service.isEmpty() && !entry.username.isEmpty()) {
        manager.updateEntry(row, entry);
        manager.saveToFile("users/" + currentLogin + ".dat", masterPassword);
        refreshTable();
    }
}

void MainWindow::removePassword() {
    int row = table->currentRow();
    if (row < 0) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Удаление", "Удалить выбранную запись?", QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        manager.removeEntry(row);
        manager.saveToFile("users/" + currentLogin + ".dat", masterPassword);
        refreshTable();
    }
}

void MainWindow::exportPasswords() {
    QString filePath = QFileDialog::getSaveFileName(this, "Экспорт паролей", "", "Файл данных (*.dat)");
    if (filePath.isEmpty()) return;

    if (manager.saveToFile(filePath, masterPassword)) {
        QMessageBox::information(this, "Успех", "Пароли экспортированы.");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось экспортировать данные.");
    }
}

void MainWindow::importPasswords() {
    QString filePath = QFileDialog::getOpenFileName(this, "Импорт паролей", "", "Файл данных (*.dat)");
    if (filePath.isEmpty()) return;

    if (manager.loadFromFile(filePath, masterPassword)) {
        QMessageBox::information(this, "Успех", "Данные успешно импортированы.");
        refreshTable();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось расшифровать или прочитать файл.");
    }
}

void MainWindow::showQrCode() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите строку в таблице.");
        return;
    }

    const PasswordEntry &entry = manager.getEntries()[row];
    QString data = QString("Сервис: %1\nЛогин: %2\nПароль: %3")
                       .arg(entry.service)
                       .arg(entry.username)
                       .arg(entry.password);

    QRDialog *dialog = new QRDialog(data, this);
    dialog->exec();
}


void MainWindow::createSettingsMenu() {
    auto *settingsButton = new QToolButton(this);
    settingsButton->setText("⚙ Настройки");
    settingsButton->setPopupMode(QToolButton::InstantPopup);

    QMenu *menu = new QMenu(this);
    menu->addAction("Выйти из аккаунта", this, &MainWindow::logout);
    menu->addAction("Удалить аккаунт", this, &MainWindow::deleteAccount);
    menu->addAction("Выйти из приложения", this, &MainWindow::quitApp);

    settingsButton->setMenu(menu);

    auto *layout = static_cast<QVBoxLayout *>(centralWidget()->layout());
    layout->addSpacing(10);
    layout->addWidget(settingsButton);
}

void MainWindow::logout() {
    manager.saveToFile("users/" + currentLogin + ".dat", masterPassword);
    QMessageBox::information(this, "Выход", "Выход из аккаунта.");
    qApp->exit(42);
}

void MainWindow::deleteAccount() {
    manager.saveToFile("users/" + currentLogin + ".dat", masterPassword);

    if (QMessageBox::question(this, "Удалить аккаунт", "Удалить аккаунт " + currentLogin + "?") == QMessageBox::Yes) {
        // Очистка паролей перед удалением файла
        manager = PasswordManager(); // сбросить менеджер
        manager.saveToFile("users/" + currentLogin + ".dat", masterPassword);
        QFile::remove("users/" + currentLogin + ".dat");

        QFile file("users.json");
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            QJsonObject obj = doc.object();
            obj.remove(currentLogin);
            file.close();

            file.open(QIODevice::WriteOnly);
            file.write(QJsonDocument(obj).toJson());
            file.close();
        }

        QMessageBox::information(this, "Удалено", "Аккаунт успешно удалён.");
        qApp->exit(42);
    }
}

void MainWindow::quitApp() {
    manager.saveToFile("users/" + currentLogin + ".dat", masterPassword);
    qApp->quit();
}
