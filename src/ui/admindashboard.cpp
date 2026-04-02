#include "admindashboard.h"
#include "../database/databasemanager.h"
#include "../models/user.h"
#include "../models/vendor.h"
#include "../models/booking.h"
#include "../models/waitlistentry.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QFont>

AdminDashboard::AdminDashboard(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void AdminDashboard::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // ── Header ───────────────────────────────────────────────────────
    m_headerLabel = new QLabel("System Administrator Dashboard");
    QFont headerFont;
    headerFont.setPointSize(18);
    headerFont.setBold(true);
    m_headerLabel->setFont(headerFont);
    m_headerLabel->setStyleSheet("color: #2c3e50; margin-bottom: 10px;");

    mainLayout->addWidget(m_headerLabel);

    // ── Overview Stats ───────────────────────────────────────────────
    auto *statsGroup = new QGroupBox("System Overview");
    auto *statsLayout = new QHBoxLayout(statsGroup);
    statsLayout->setSpacing(30);

    auto makeStatLabel = [](const QString &text) -> QLabel * {
        auto *label = new QLabel(text);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet(
            "background-color: #eaf2f8; border: 1px solid #aed6f1;"
            "border-radius: 6px; padding: 14px; font-size: 13px;"
            "font-weight: bold; color: #2c3e50;"
        );
        return label;
    };

    m_totalUsersLabel = makeStatLabel("Total Users: 0");
    m_totalBookingsLabel = makeStatLabel("Total Bookings: 0");
    m_totalWaitlistLabel = makeStatLabel("Waitlist Entries: 0");

    statsLayout->addWidget(m_totalUsersLabel);
    statsLayout->addWidget(m_totalBookingsLabel);
    statsLayout->addWidget(m_totalWaitlistLabel);

    mainLayout->addWidget(statsGroup);

    // ── Users Table ──────────────────────────────────────────────────
    auto *usersGroup = new QGroupBox("All Users");
    auto *usersLayout = new QVBoxLayout(usersGroup);

    m_usersTable = new QTableWidget;
    m_usersTable->setColumnCount(4);
    m_usersTable->setHorizontalHeaderLabels({"ID", "Username", "Name", "Role"});
    m_usersTable->horizontalHeader()->setStretchLastSection(true);
    m_usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_usersTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_usersTable->verticalHeader()->setVisible(false);

    usersLayout->addWidget(m_usersTable);

    mainLayout->addWidget(usersGroup);
}

void AdminDashboard::setAdmin(User *admin)
{
    m_admin = admin;
    refreshDashboard();
}

void AdminDashboard::refreshDashboard()
{
    auto &db = DatabaseManager::instance();

    // Stats
    QList<Booking> allBookings = db.getAllBookings();
    QList<Vendor *> allVendors = db.getAllVendors();

    // We count vendors + possible operator + admin as total users
    // For a more accurate count we'd need a getAllUsers(), but we approximate
    // by counting vendors + checking for operator/admin roles
    int totalBookings = allBookings.size();

    // Collect waitlist count across all schedules
    int totalWaitlist = 0;
    QList<MarketSchedule> schedules = db.getAllSchedules();
    for (const auto &s : schedules) {
        totalWaitlist += db.getWaitlistForScheduleAndCategory(s.getId(), "Food").size();
        totalWaitlist += db.getWaitlistForScheduleAndCategory(s.getId(), "Artisan").size();
    }

    // Build user table from vendors + known non-vendor users
    // We'll populate the table using vendors and then add operator/admin
    struct UserRow {
        int id;
        QString username;
        QString name;
        QString role;
    };
    QList<UserRow> userRows;

    for (auto *v : allVendors) {
        userRows.append({v->getId(), v->getUsername(), v->getName(),
                         User::roleToString(v->getRole())});
    }
    qDeleteAll(allVendors);

    // Add admin if available
    if (m_admin) {
        userRows.append({m_admin->getId(), m_admin->getUsername(),
                         m_admin->getName(),
                         User::roleToString(m_admin->getRole())});
    }

    int totalUsers = userRows.size();

    m_totalUsersLabel->setText(QString("Total Users: %1").arg(totalUsers));
    m_totalBookingsLabel->setText(QString("Total Bookings: %1").arg(totalBookings));
    m_totalWaitlistLabel->setText(QString("Waitlist Entries: %1").arg(totalWaitlist));

    // Populate users table
    m_usersTable->setRowCount(userRows.size());
    for (int row = 0; row < userRows.size(); ++row) {
        const auto &u = userRows[row];
        m_usersTable->setItem(row, 0, new QTableWidgetItem(QString::number(u.id)));
        m_usersTable->setItem(row, 1, new QTableWidgetItem(u.username));
        m_usersTable->setItem(row, 2, new QTableWidgetItem(u.name));
        m_usersTable->setItem(row, 3, new QTableWidgetItem(u.role));
    }
}
