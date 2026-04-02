#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include <QAction>

class User;
class LoginWindow;
class VendorDashboard;
class OperatorDashboard;
class AdminDashboard;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onLoginSuccess(User *user);
    void onLogout();

private:
    void setupUi();
    void setupMenuBar();

    QStackedWidget *m_stack = nullptr;
    LoginWindow *m_loginWindow = nullptr;
    VendorDashboard *m_vendorDashboard = nullptr;
    OperatorDashboard *m_operatorDashboard = nullptr;
    AdminDashboard *m_adminDashboard = nullptr;

    User *m_currentUser = nullptr;

    QAction *m_logoutAction = nullptr;
    QAction *m_exitAction = nullptr;
};
