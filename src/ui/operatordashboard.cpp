#include "operatordashboard.h"
#include "../database/databasemanager.h"
#include "../models/user.h"
#include "../models/vendor.h"
#include "../models/booking.h"
#include "../models/marketschedule.h"
#include "../models/waitlistentry.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFont>
#include <QGroupBox>

OperatorDashboard::OperatorDashboard(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void OperatorDashboard::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // ── Header ───────────────────────────────────────────────────────
    m_headerLabel = new QLabel("Market Operator Dashboard");
    QFont headerFont;
    headerFont.setPointSize(18);
    headerFont.setBold(true);
    m_headerLabel->setFont(headerFont);
    m_headerLabel->setStyleSheet("color: #2c3e50; margin-bottom: 10px;");

    mainLayout->addWidget(m_headerLabel);

    // ── Tab Widget ───────────────────────────────────────────────────
    m_tabWidget = new QTabWidget;

    auto *bookTab = new QWidget;
    setupBookTab(bookTab);
    m_tabWidget->addTab(bookTab, "Book Stall for Vendor");

    auto *cancelTab = new QWidget;
    setupCancelTab(cancelTab);
    m_tabWidget->addTab(cancelTab, "Cancel Booking");

    auto *waitlistTab = new QWidget;
    setupWaitlistTab(waitlistTab);
    m_tabWidget->addTab(waitlistTab, "Remove from Waitlist");

    mainLayout->addWidget(m_tabWidget);

    connect(m_tabWidget, &QTabWidget::currentChanged, this, [this]() {
        refreshDashboard();
    });
}

void OperatorDashboard::setupBookTab(QWidget *tab)
{
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(14);

    auto *formGroup = new QGroupBox("Book a Stall on Behalf of a Vendor");
    auto *formLayout = new QFormLayout(formGroup);
    formLayout->setSpacing(12);

    m_vendorCombo = new QComboBox;
    m_vendorCombo->setMinimumHeight(30);
    formLayout->addRow("Select Vendor:", m_vendorCombo);

    m_weekCombo = new QComboBox;
    m_weekCombo->setMinimumHeight(30);
    formLayout->addRow("Select Market Week:", m_weekCombo);

    m_bookButton = new QPushButton("Book Stall");
    m_bookButton->setMinimumHeight(36);
    m_bookButton->setMaximumWidth(200);
    m_bookButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #27ae60; color: white; border: none;"
        "  border-radius: 4px; padding: 8px 16px; font-weight: bold; font-size: 12px;"
        "}"
        "QPushButton:hover { background-color: #219a52; }"
    );

    connect(m_bookButton, &QPushButton::clicked,
            this, &OperatorDashboard::onBookForVendorClicked);

    layout->addWidget(formGroup);
    layout->addWidget(m_bookButton);
    layout->addStretch();
}

void OperatorDashboard::setupCancelTab(QWidget *tab)
{
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);

    auto *label = new QLabel("All Current Bookings");
    QFont f;
    f.setPointSize(12);
    f.setBold(true);
    label->setFont(f);
    label->setStyleSheet("color: #2c3e50; margin-bottom: 6px;");
    layout->addWidget(label);

    m_bookingsTable = new QTableWidget;
    m_bookingsTable->setColumnCount(4);
    m_bookingsTable->setHorizontalHeaderLabels(
        {"Vendor Name", "Category", "Market Week", "Action"});
    m_bookingsTable->horizontalHeader()->setStretchLastSection(true);
    m_bookingsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_bookingsTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_bookingsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_bookingsTable->verticalHeader()->setVisible(false);

    layout->addWidget(m_bookingsTable);
}

void OperatorDashboard::setupWaitlistTab(QWidget *tab)
{
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);

    auto *label = new QLabel("All Waitlist Entries");
    QFont f;
    f.setPointSize(12);
    f.setBold(true);
    label->setFont(f);
    label->setStyleSheet("color: #2c3e50; margin-bottom: 6px;");
    layout->addWidget(label);

    m_waitlistTable = new QTableWidget;
    m_waitlistTable->setColumnCount(5);
    m_waitlistTable->setHorizontalHeaderLabels(
        {"Position", "Vendor Name", "Category", "Market Week", "Action"});
    m_waitlistTable->horizontalHeader()->setStretchLastSection(true);
    m_waitlistTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_waitlistTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_waitlistTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_waitlistTable->verticalHeader()->setVisible(false);

    layout->addWidget(m_waitlistTable);
}

void OperatorDashboard::setOperator(User *op)
{
    m_operator = op;
    refreshDashboard();
}

void OperatorDashboard::refreshDashboard()
{
    refreshBookTab();
    refreshCancelTab();
    refreshWaitlistTab();
}

void OperatorDashboard::refreshBookTab()
{
    auto &db = DatabaseManager::instance();

    // Populate vendor combo
    m_vendorCombo->clear();
    QList<Vendor *> vendors = db.getAllVendors();
    for (auto *v : vendors) {
        m_vendorCombo->addItem(
            QString("%1 (%2)").arg(v->getName(), v->getCategory()),
            v->getId());
    }
    qDeleteAll(vendors);

    // Populate week combo
    m_weekCombo->clear();
    QList<MarketSchedule> schedules = db.getAllSchedules();
    for (const auto &s : schedules) {
        QString info = QString("%1  [Food: %2/%3, Artisan: %4/%5]")
            .arg(s.getWeekLabel())
            .arg(s.getMaxFoodStalls() - s.getBookedFoodStalls())
            .arg(s.getMaxFoodStalls())
            .arg(s.getMaxArtisanStalls() - s.getBookedArtisanStalls())
            .arg(s.getMaxArtisanStalls());
        m_weekCombo->addItem(info, s.getId());
    }
}

void OperatorDashboard::refreshCancelTab()
{
    auto &db = DatabaseManager::instance();
    QList<Booking> bookings = db.getAllBookings();

    m_bookingsTable->setRowCount(bookings.size());

    for (int row = 0; row < bookings.size(); ++row) {
        const Booking &b = bookings[row];

        // Look up vendor name
        User *user = db.getUserById(b.getVendorId());
        QString vendorName = user ? user->getName() : "Unknown";
        delete user;

        MarketSchedule sched = db.getScheduleById(b.getScheduleId());

        m_bookingsTable->setItem(row, 0, new QTableWidgetItem(vendorName));
        m_bookingsTable->setItem(row, 1, new QTableWidgetItem(b.getVendorCategory()));
        m_bookingsTable->setItem(row, 2, new QTableWidgetItem(sched.getWeekLabel()));

        auto *cancelBtn = new QPushButton("Cancel");
        cancelBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #e74c3c; color: white; border: none;"
            "  border-radius: 3px; padding: 4px 10px; font-weight: bold;"
            "}"
            "QPushButton:hover { background-color: #c0392b; }"
        );
        int bookingId = b.getId();
        connect(cancelBtn, &QPushButton::clicked, this, [this, bookingId]() {
            onCancelBookingClicked(bookingId);
        });
        m_bookingsTable->setCellWidget(row, 3, cancelBtn);
    }
}

void OperatorDashboard::refreshWaitlistTab()
{
    auto &db = DatabaseManager::instance();
    QList<MarketSchedule> schedules = db.getAllSchedules();

    // Collect all waitlist entries across all schedules and categories
    struct WaitlistRow {
        int entryId;
        int position;
        QString vendorName;
        QString category;
        QString weekLabel;
    };
    QList<WaitlistRow> allRows;

    for (const auto &sched : schedules) {
        for (const QString &cat : {"Food", "Artisan"}) {
            QList<WaitlistEntry> entries =
                db.getWaitlistForScheduleAndCategory(sched.getId(), cat);
            for (const auto &e : entries) {
                User *user = db.getUserById(e.getVendorId());
                QString name = user ? user->getName() : "Unknown";
                delete user;
                allRows.append({e.getId(), e.getPosition(), name,
                                e.getCategory(), sched.getWeekLabel()});
            }
        }
    }

    m_waitlistTable->setRowCount(allRows.size());

    for (int row = 0; row < allRows.size(); ++row) {
        const auto &wr = allRows[row];

        m_waitlistTable->setItem(row, 0,
            new QTableWidgetItem(QString::number(wr.position)));
        m_waitlistTable->setItem(row, 1, new QTableWidgetItem(wr.vendorName));
        m_waitlistTable->setItem(row, 2, new QTableWidgetItem(wr.category));
        m_waitlistTable->setItem(row, 3, new QTableWidgetItem(wr.weekLabel));

        auto *removeBtn = new QPushButton("Remove");
        removeBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #e74c3c; color: white; border: none;"
            "  border-radius: 3px; padding: 4px 10px; font-weight: bold;"
            "}"
            "QPushButton:hover { background-color: #c0392b; }"
        );
        int entryId = wr.entryId;
        connect(removeBtn, &QPushButton::clicked, this, [this, entryId]() {
            onRemoveWaitlistClicked(entryId);
        });
        m_waitlistTable->setCellWidget(row, 4, removeBtn);
    }
}

void OperatorDashboard::onBookForVendorClicked()
{
    if (m_vendorCombo->count() == 0 || m_weekCombo->count() == 0) {
        QMessageBox::warning(this, "Error", "No vendors or market weeks available.");
        return;
    }

    auto &db = DatabaseManager::instance();
    int vendorId = m_vendorCombo->currentData().toInt();
    int scheduleId = m_weekCombo->currentData().toInt();

    // Get vendor to determine category
    Vendor *vendor = db.getVendorById(vendorId);
    if (!vendor) {
        QMessageBox::warning(this, "Error", "Selected vendor not found.");
        return;
    }

    QString category = vendor->getCategory();

    // Validate no active booking
    if (db.hasActiveBooking(vendorId)) {
        QMessageBox::warning(this, "Cannot Book",
            QString("%1 already has an active booking.").arg(vendor->getName()));
        delete vendor;
        return;
    }

    // Validate availability
    MarketSchedule sched = db.getScheduleById(scheduleId);
    bool available = (category == "Food" && sched.hasFoodAvailability()) ||
                     (category == "Artisan" && sched.hasArtisanAvailability());

    if (!available) {
        QMessageBox::warning(this, "No Availability",
            QString("No %1 stalls available for %2.").arg(category, sched.getWeekLabel()));
        delete vendor;
        return;
    }

    auto reply = QMessageBox::question(this, "Confirm Booking",
        QString("Book a %1 stall for %2 during %3?")
            .arg(category, vendor->getName(), sched.getWeekLabel()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (db.createBooking(vendorId, scheduleId, category)) {
            QMessageBox::information(this, "Success", "Stall booked successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to book stall.");
        }
    }

    delete vendor;
    refreshDashboard();
}

void OperatorDashboard::onCancelBookingClicked(int bookingId)
{
    auto reply = QMessageBox::question(this, "Confirm Cancellation",
        "Are you sure you want to cancel this booking?\n"
        "The next vendor on the waitlist (if any) will be automatically booked.",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    if (DatabaseManager::instance().cancelBooking(bookingId)) {
        QMessageBox::information(this, "Success", "Booking cancelled successfully.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to cancel booking.");
    }
    refreshDashboard();
}

void OperatorDashboard::onRemoveWaitlistClicked(int entryId)
{
    auto reply = QMessageBox::question(this, "Confirm Removal",
        "Are you sure you want to remove this vendor from the waitlist?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    if (DatabaseManager::instance().removeFromWaitlist(entryId)) {
        QMessageBox::information(this, "Success", "Removed from waitlist.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to remove from waitlist.");
    }
    refreshDashboard();
}
