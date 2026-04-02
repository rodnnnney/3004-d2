#pragma once

#include <QList>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>

#include "../models/booking.h"
#include "../models/marketschedule.h"
#include "../models/user.h"
#include "../models/vendor.h"
#include "../models/waitlistentry.h"

class DatabaseManager {
public:
    static DatabaseManager &instance();

    bool openDatabase(const QString &path = QStringLiteral("hintonMarket.sqlite3"));
    void closeDatabase();

    // ── User operations ──────────────────────────────────────────────
    User *authenticateUser(const QString &username, const QString &password);
    QList<Vendor *> getAllVendors();
    Vendor *getVendorById(int id);
    User *getUserById(int id);

    // ── Schedule operations ──────────────────────────────────────────
    QList<MarketSchedule> getAllSchedules();
    MarketSchedule getScheduleById(int id);
    void updateScheduleStallCounts(int schedule_id, int foodBooked, int artisanBooked);

    // ── Booking operations ───────────────────────────────────────────
    QList<Booking> getBookingsForVendor(int vendor_id);
    bool hasActiveBooking(int vendor_id);
    Booking getActiveBookingForVendor(int vendor_id);
    bool createBooking(int vendor_id, int schedule_id, const QString &category);
    bool cancelBooking(int bookingId);
    QList<Booking> getAllBookings();

    // ── Waitlist operations ──────────────────────────────────────────
    QList<WaitlistEntry> getWaitlistForScheduleAndCategory(int schedule_id,
                                                           const QString &category);
    QList<WaitlistEntry> getWaitlistForVendor(int vendor_id);
    bool addToWaitlist(int vendor_id, int schedule_id, const QString &category);
    bool removeFromWaitlist(int entryId);
    WaitlistEntry getNextInWaitlist(int schedule_id, const QString &category);
    bool isVendorOnWaitlist(int vendor_id, int schedule_id);

    // ── Notification tracking ────────────────────────────────────────
    QStringList getNotificationsForVendor(int vendor_id);
    void addNotification(int vendor_id, const QString &message);

private:
    DatabaseManager() = default;
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;

    QSqlDatabase m_db;
};
