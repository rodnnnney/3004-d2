#include "loginwindow.h"
#include "../database/databasemanager.h"
#include "../models/user.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>
#include <QSpacerItem>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void LoginWindow::setupUi()
{
    setMinimumSize(400, 300);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(60, 40, 60, 40);
    mainLayout->setSpacing(20);

    // ── Title ────────────────────────────────────────────────────────
    m_titleLabel = new QLabel("HintonMarket");
    QFont titleFont;
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("color: #2c3e50; margin-bottom: 10px;");

    auto *subtitleLabel = new QLabel("Vendor Stall Management System");
    QFont subFont;
    subFont.setPointSize(11);
    subtitleLabel->setFont(subFont);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("color: #7f8c8d; margin-bottom: 20px;");

    // ── Form ─────────────────────────────────────────────────────────
    auto *formLayout = new QFormLayout;
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight);

    m_usernameEdit = new QLineEdit;
    m_usernameEdit->setPlaceholderText("Enter your username");
    m_usernameEdit->setMinimumHeight(32);

    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setPlaceholderText("Enter your password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setMinimumHeight(32);

    QFont labelFont;
    labelFont.setPointSize(10);

    auto *userLabel = new QLabel("Username:");
    userLabel->setFont(labelFont);
    auto *passLabel = new QLabel("Password:");
    passLabel->setFont(labelFont);

    formLayout->addRow(userLabel, m_usernameEdit);
    formLayout->addRow(passLabel, m_passwordEdit);

    // ── Login Button ─────────────────────────────────────────────────
    m_loginButton = new QPushButton("Login");
    m_loginButton->setMinimumHeight(38);
    m_loginButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #3498db;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 4px;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #2471a3;"
        "}"
    );

    // ── Assemble ─────────────────────────────────────────────────────
    mainLayout->addStretch(1);
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_loginButton);
    mainLayout->addStretch(2);

    // ── Connections ──────────────────────────────────────────────────
    connect(m_loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed,
            this, &LoginWindow::onLoginClicked);
    connect(m_usernameEdit, &QLineEdit::returnPressed,
            this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed",
                             "Please enter both username and password.");
        return;
    }

    User *user = DatabaseManager::instance().authenticateUser(username, password);
    if (user) {
        m_usernameEdit->clear();
        m_passwordEdit->clear();
        emit loginSuccess(user);
    } else {
        QMessageBox::warning(this, "Login Failed",
                             "Invalid username or password. Please try again.");
        m_passwordEdit->clear();
        m_passwordEdit->setFocus();
    }
}
