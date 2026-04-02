#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class User;

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccess(User *user);

private slots:
    void onLoginClicked();

private:
    void setupUi();

    QLineEdit *m_usernameEdit = nullptr;
    QLineEdit *m_passwordEdit = nullptr;
    QPushButton *m_loginButton = nullptr;
    QLabel *m_titleLabel = nullptr;
};
