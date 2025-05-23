# PassGuard — Менеджер паролей

PassGuard — это настольное приложение для безопасного хранения и управления паролями пользователей. Написано на C++ с использованием Qt 6.

![Qt logo](https://img.shields.io/badge/Qt-6.9-green?style=flat\&logo=qt)

---

## 🚀 Возможности

* 👤 Поддержка нескольких пользователей (регистрация/вход/удаление)
* 🔒 Хранение паролей с шифрованием (AES-256)
* ➕ Добавление, редактирование и удаление записей
* 📦 Импорт и экспорт файлов с паролями
* 📷 Генерация QR-кода для одной записи
* ⚙ Меню настроек (выйти из аккаунта / удалить аккаунт / выйти из приложения)

<details>
<summary><strong>📁 Структура проекта</strong></summary>

```
PassGuard/
├── main.cpp
├── mainwindow.{h,cpp}
├── authdialog.{h,cpp}
├── passwordmanager.{h,cpp}
├── passwordentry.h
├── cryptomanager.{h,cpp}
├── qrdialog.{h,cpp}
├── qrcodegen.{hpp,cpp}
└── CMakeLists.txt
```

</details>

---

## 🔐 Безопасность

* Все данные пользователя шифруются с использованием пароля и алгоритма **AES-256 (OpenSSL)**
* Хэширование мастер-пароля при регистрации
* Данные разделены по логинам

---

## 💠 Сборка проекта

> Требуется установленный **Qt 6** и **CMake**.

```bash
git clone https://github.com/CatherineFlower/PassGuard.git
cd PassGuard
mkdir build && cd build
cmake ..
make
./PassGuard
```

---

## 👩‍💻 Автор

**Екатерина Петренко**
Курсовая работа по дисциплине: *Технологии индустриального программирования*
МИРЭА — Российский технологический университет, 2025

---

## 📌 Лицензия

Этот проект создан в образовательных целях. Коммерческое использование запрещено без согласия автора.
