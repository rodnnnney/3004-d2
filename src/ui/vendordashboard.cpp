#include "vendordashboard.h"
#include "../database/databasemanager.h"
#include "../models/vendor.h"
#include "../models/booking.h"
#include "../models/marketschedule.h"
#include "../models/waitlistentry.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFont>
#include <QScrollArea>

VendorDashboard::VendorDashboard(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void VendorDashboard::setupUi()
{
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto *contentWidget = new QWidget;
    auto *mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // ── Header Section ───────────────────────────────────────────────
    auto *headerGroup = new QGroupBox("Vendor Information");
    auto *headerLayout = new QVBoxLayout(headerGroup);
    headerLayout->setSpacing(6);

    m_welcomeLabel = new QLabel;
    QFont welcomeFont;
    welcomeFont.setPointSize(16);
    welcomeFont.setBold(true);
    m_welcomeLabel->setFont(welcomeFont);
    m_welcomeLabel->setStyleSheet("color: #2c3e50;");

    m_businessLabel = new QLabel;
    m_businessLabel->setStyleSheet("color: #555; font-size: 12px;");

    m_categoryLabel = new QLabel;
    m_categoryLabel->setStyleSheet("color: #555; font-size: 12px;");

    headerLayout->addWidget(m_welcomeLabel);
    headerLayout->addWidget(m_businessLabel);
    headerLayout->addWidget(m_categoryLabel);

    // ── Notifications Section ────────────────────────────────────────
    m_notificationGroup = new QGroupBox("Notifications");
    auto *notifLayout = new QVBoxLayout(m_notificationGroup);
    m_notificationLabel = new QLabel("No new notifications.");
    m_notificationLabel->setWordWrap(true);
    m_notificationLabel->setStyleSheet(
        "background-color: #fef9e7; border: 1px solid #f9e79f;"
        "border-radius: 4px; padding: 8px; color: #7d6608;"
    );
    notifLayout->addWidget(m_notificationLabel);

    // ── My Booking Section ───────────────────────────────────────────
    m_bookingGroup = new QGroupBox("My Current Booking");
    auto *bookingLayout = new QVBoxLayout(m_bookingGroup);
    bookingLayout->setSpacing(10);

    m_bookingInfoLabel = new QLabel("No active booking.");
    m_bookingInfoLabel->setStyleSheet("font-size: 12px; padding: 4px;");

    m_cancelBookingButton = new QPushButton("Cancel Booking");
    m_cancelBookingButton->setMaximumWidth(160);
    m_cancelBookingButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #e74c3c; color: white; border: none;"
        "  border-radius: 4px; padding: 6px 14px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #c0392b; }"
        "QPushButton:disabled { background-color: #bdc3c7; }"
    );
    m_cancelBookingButton->setEnabled(false);

    bookingLayout->addWidget(m_bookingInfoLabel);
    bookingLayout->addWidget(m_cancelBookingButton);

    connect(m_cancelBookingButton, &QPushButton::clicked,
            this, &VendorDashboard::onCancelBookingClicked);

    // ── Book a Stall Section ─────────────────────────────────────────
    auto *scheduleGroup = new QGroupBox("Book a Stall");
    auto *scheduleLayout = new QVBoxLayout(scheduleGroup);

    m_scheduleTable = new QTableWidget;
    m_scheduleTable->setColumnCount(5);
    m_scheduleTable->setHorizontalHeaderLabels(
        {"Week", "Food Available", "Artisan Available", "Book", "Waitlist"});
    m_scheduleTable->horizontalHeader()->setStretchLastSection(true);
    m_scheduleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_scheduleTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_scheduleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_scheduleTable->verticalHeader()->setVisible(false);
    m_scheduleTable->setMinimumHeight(180);

    scheduleLayout->addWidget(m_scheduleTable);

    // ── My Waitlist Entries Section ──────────────────────────────────
    auto *waitlistGroup = new QGroupBox("My Waitlist Entries");
    auto *waitlistLayout = new QVBoxLayout(waitlistGroup);

    m_waitlistTable = new QTableWidget;
    m_waitlistTable->setColumnCount(4);
    m_waitlistTable->setHorizontalHeaderLabels(
        {"Market Week", "Category", "Position", "Action"});
    m_waitlistTable->horizontalHeader()->setStretchLastSection(true);
    m_waitlistTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_waitlistTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_waitlistTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_waitlistTable->verticalHeader()->setVisible(false);
    m_waitlistTable->setMinimumHeight(140);

    waitlistLayout->addWidget(m_waitlistTable);

    // ── Assemble ─────────────────────────────────────────────────────
    mainLayout->addWidget(headerGroup);
    mainLayout->addWidget(m_notificationGroup);
    mainLayout->addWidget(m_bookingGroup);
    mainLayout->addWidget(scheduleGroup);
    mainLayout->addWidget(waitlistGroup);
    mainLayout->addStretch();

    scrollArea->setWidget(contentWidget);

    auto *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(scrollArea);
}

void VendorDashboard::setVendor(Vendor *vendor)
{
    m_vendor = vendor;
    refreshDashboard();
}

void VendorDashboard::refreshDashboard()
{
    if (!m_vendor) return;

    m_welcomeLabel->setText("Welcome, " + m_vendor->getName());
    m_businessLabel->setText("Business: " + m_vendor->getBusinessName());
    m_categoryLabel->setText("Category: " + m_vendor->getCategory());

    refreshNotifications();
    refreshMyBooking();
    refreshScheduleTable();
    refreshWaitlistTable();
}

void VendorDashboard::refreshNotifications()
{
    if (!m_vendor) return;

    QStringList notifs =
        DatabaseManager::instance().getNotificationsForVendor(m_vendor->getId());

    if (notifs.isEmpty()) {
        m_notificationLabel->setText("No new notifications.");
        m_notificationLabel->setStyleSheet(
            "background-color: #eafaf1; border: 1px solid #a9dfbf;"
            "border-radius: 4px; padding: 8px; color: #1e8449;"
        );
    } else {
        m_notificationLabel->setText(notifs.join("\n"));
        m_notificationLabel->setStyleSheet(
            "background-color: #fef9e7; border: 1px solid #f9e79f;"
            "border-radius: 4px; padding: 8px; color: #7d6608;"
        );
    }
}

void VendorDashboard::refreshMyBooking()
{
    if (!m_vendor) return;

    auto &db = DatabaseManager::instance();
    bool hasBooking = db.hasActiveBooking(m_vendor->getId());

    if (hasBooking) {
        Booking booking = db.getActiveBookingForVendor(m_vendor->getId());
        MarketSchedule sched = db.getScheduleById(booking.getScheduleId());
        m_bookingInfoLabel->setText(
            QString("Booked: %1  |  Category: %2  |  Booked on: %3")
                .arg(sched.getWeekLabel(),
                     booking.getVendorCategory(),
                     booking.getBookingDate().toString("yyyy-MM-dd hh:mm")));
        m_cancelBookingButton->setEnabled(true);
    } else {
        m_bookingInfoLabel->setText("No active booking.");
        m_cancelBookingButton->setEnabled(false);
    }
}

void VendorDashboard::refreshScheduleTable()
{
    if (!m_vendor) return;

    auto &db = DatabaseManager::instance();
    QList<MarketSchedule> schedules = db.getAllSchedules();
    bool hasBooking = db.hasActiveBooking(m_vendor->getId());
    QString category = m_vendor->getCategory();

    m_scheduleTable->setRowCount(schedules.size());

    for (int row = 0; row < schedules.size(); ++row) {
        const MarketSchedule &sched = schedules[row];

        int foodAvail = sched.getMaxFoodStalls() - sched.getBookedFoodStalls();
        int artisanAvail = sched.getMaxArtisanStalls() - sched.getBookedArtisanStalls();

        m_scheduleTable->setItem(row, 0,
            new QTableWidgetItem(sched.getWeekLabel()));
        m_scheduleTable->setItem(row, 1,
            new QTableWidgetItem(QString::number(foodAvail) + " / " +
                                 QString::number(sched.getMaxFoodStalls())));
        m_scheduleTable->setItem(row, 2,
            new QTableWidgetItem(QString::number(artisanAvail) + " / " +
                                 QString::number(sched.getMaxArtisanStalls())));

        // Book button
        bool canBook = !hasBooking &&
            ((category == "Food" && sched.hasFoodAvailability()) ||
             (category == "Artisan" && sched.hasArtisanAvailability()));

        auto *bookBtn = new QPushButton("Book");
        bookBtn->setEnabled(canBook);
        bookBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #27ae60; color: white; border: none;"
            "  border-radius: 3px; padding: 4px 10px; font-weight: bold;"
            "}"
            "QPushButton:hover { background-color: #219a52; }"
            "QPushButton:disabled { background-color: #bdc3c7; color: #7f8c8d; }"
        );
        int schedId = sched.getId();
        connect(bookBtn, &QPushButton::clicked, this, [this, schedId]() {
            onBookClicked(schedId);
        });
        m_scheduleTable->setCellWidget(row, 3, bookBtn);

        // Waitlist button
        bool weekFull = (category == "Food" && !sched.hasFoodAvailability()) ||
                        (category == "Artisan" && !sched.hasArtisanAvailability());
        bool alreadyOnWaitlist = db.isVendorOnWaitlist(m_vendor->getId(), sched.getId());

        auto *waitBtn = new QPushButton(alreadyOnWaitlist ? "On Waitlist" : "Join Waitlist");
        waitBtn->setEnabled(weekFull && !alreadyOnWaitlist && !hasBooking);
        waitBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #f39c12; color: white; border: none;"
            "  border-radius: 3px; padding: 4px 10px; font-weight: bold;"
            "}"
            "QPushButton:hover { background-color: #e67e22; }"
            "QPushButton:disabled { background-color: #bdc3c7; color: #7f8c8d; }"
        );
        connect(waitBtn, &QPushButton::clicked, this, [this, schedId]() {
            onJoinWaitlistClicked(schedId);
        });
        m_scheduleTable->setCellWidget(row, 4, waitBtn);
    }
}

void VendorDashboard::refreshWaitlistTable()
{
    if (!m_vendor) return;

    QList<WaitlistEntry> entries =
        DatabaseManager::instance().getWaitlistForVendor(m_vendor->getId());

    m_waitlistTable->setRowCount(entries.size());

    for (int row = 0; row < entries.size(); ++row) {
        const WaitlistEntry &entry = entries[row];
        MarketSchedule sched =
            DatabaseManager::instance().getScheduleById(entry.getScheduleId());

        m_waitlistTable->setItem(row, 0,
            new QTableWidgetItem(sched.getWeekLabel()));
        m_waitlistTable->setItem(row, 1,
            new QTableWidgetItem(entry.getCategory()));
        m_waitlistTable->setItem(row, 2,
            new QTableWidgetItem(QString::number(entry.getPosition())));

        auto *removeBtn = new QPushButton("Remove");
        removeBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #e74c3c; color: white; border: none;"
            "  border-radius: 3px; padding: 4px 10px; font-weight: bold;"
            "}"
            "QPushButton:hover { background-color: #c0392b; }"
        );
        int entryId = entry.getId();
        connect(removeBtn, &QPushButton::clicked, this, [this, entryId]() {
            onRemoveWaitlistClicked(entryId);
        });
        m_waitlistTable->setCellWidget(row, 3, removeBtn);
    }
}

void VendorDashboard::onCancelBookingClicked()
{
    if (!m_vendor) return;

    auto reply = QMessageBox::question(this, "Cancel Booking",
        "Are you sure you want to cancel your current booking?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    Booking booking =
        DatabaseManager::instance().getActiveBookingForVendor(m_vendor->getId());

    if (DatabaseManager::instance().cancelBooking(booking.getId())) {
        QMessageBox::information(this, "Success", "Booking cancelled successfully.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to cancel booking.");
    }
    refreshDashboard();
}

void VendorDashboard::onBookClicked(int scheduleId)
{
    if (!m_vendor) return;

    auto &db = DatabaseManager::instance();

    if (db.hasActiveBooking(m_vendor->getId())) {
        QMessageBox::warning(this, "Cannot Book",
            "You already have an active booking. Cancel it first.");
        return;
    }

    MarketSchedule sched = db.getScheduleById(scheduleId);
    QString category = m_vendor->getCategory();

    auto reply = QMessageBox::question(this, "Confirm Booking",
        QString("Book a %1 stall for %2?").arg(category, sched.getWeekLabel()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    if (db.createBooking(m_vendor->getId(), scheduleId, category)) {
        QMessageBox::information(this, "Success", "Stall booked successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to book stall. It may no longer be available.");
    }
    refreshDashboard();
}

void VendorDashboard::onJoinWaitlistClicked(int scheduleId)
{
    if (!m_vendor) return;

    auto &db = DatabaseManager::instance();
    MarketSchedule sched = db.getScheduleById(scheduleId);
    QString category = m_vendor->getCategory();

    QList<WaitlistEntry> current =
        db.getWaitlistForScheduleAndCategory(scheduleId, category);

    auto reply = QMessageBox::question(this, "Join Waitlist",
        QString("Join the waitlist for %1 (%2 stall)?\n"
                "Current queue size: %3")
            .arg(sched.getWeekLabel(), category)
            .arg(current.size()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    if (db.addToWaitlist(m_vendor->getId(), scheduleId, category)) {
        QMessageBox::information(this, "Success", "Added to waitlist successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to join waitlist.");
    }
    refreshDashboard();
}

void VendorDashboard::onRemoveWaitlistClicked(int entryId)
{
    auto reply = QMessageBox::question(this, "Remove from Waitlist",
        "Remove yourself from this waitlist?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    if (DatabaseManager::instance().removeFromWaitlist(entryId)) {
        QMessageBox::information(this, "Success", "Removed from waitlist.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to remove from waitlist.");
    }
    refreshDashboard();
}
