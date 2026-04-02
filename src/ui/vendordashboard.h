#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>

class Vendor;

class VendorDashboard : public QWidget {
    Q_OBJECT

public:
    explicit VendorDashboard(QWidget *parent = nullptr);

    void setVendor(Vendor *vendor);
    void refreshDashboard();

private slots:
    void onCancelBookingClicked();
    void onBookClicked(int scheduleId);
    void onJoinWaitlistClicked(int scheduleId);
    void onRemoveWaitlistClicked(int entryId);

private:
    void setupUi();
    void refreshNotifications();
    void refreshMyBooking();
    void refreshScheduleTable();
    void refreshWaitlistTable();

    Vendor *m_vendor = nullptr;

    // Header
    QLabel *m_welcomeLabel = nullptr;
    QLabel *m_businessLabel = nullptr;
    QLabel *m_categoryLabel = nullptr;

    // Notifications
    QLabel *m_notificationLabel = nullptr;
    QGroupBox *m_notificationGroup = nullptr;

    // My Booking
    QLabel *m_bookingInfoLabel = nullptr;
    QPushButton *m_cancelBookingButton = nullptr;
    QGroupBox *m_bookingGroup = nullptr;

    // Book a Stall
    QTableWidget *m_scheduleTable = nullptr;

    // My Waitlist Entries
    QTableWidget *m_waitlistTable = nullptr;
};
