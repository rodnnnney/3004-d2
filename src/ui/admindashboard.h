#pragma once

#include <QWidget>
#include <QLabel>
#include <QTableWidget>

class User;

class AdminDashboard : public QWidget {
    Q_OBJECT

public:
    explicit AdminDashboard(QWidget *parent = nullptr);

    void setAdmin(User *admin);
    void refreshDashboard();

private:
    void setupUi();

    User *m_admin = nullptr;

    QLabel *m_headerLabel = nullptr;
    QLabel *m_totalUsersLabel = nullptr;
    QLabel *m_totalBookingsLabel = nullptr;
    QLabel *m_totalWaitlistLabel = nullptr;
    QTableWidget *m_usersTable = nullptr;
};
