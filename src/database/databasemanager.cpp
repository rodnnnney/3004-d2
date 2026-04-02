#include "databasemanager.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

#include "../models/artisanvendor.h"
#include "../models/foodvendor.h"
#include "../models/marketoperator.h"
#include "../models/systemadmin.h"

// ═══════════════════════════════════════════════════════════════════════
// Singleton
// ═══════════════════════════════════════════════════════════════════════

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager inst;
    return inst;
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

// ═══════════════════════════════════════════════════════════════════════
// Open / Close
// ═══════════════════════════════════════════════════════════════════════

bool DatabaseManager::openDatabase(const QString &path)
{
    if (m_db.isOpen()) {
        return true;
    }

    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));

    // Resolve path relative to the application directory when not absolute
    QString dbPath = path;
    if (!QDir::isAbsolutePath(dbPath)) {
        dbPath = QCoreApplication::applicationDirPath() + QStringLiteral("/") + dbPath;
    }

    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qWarning() << "DatabaseManager: failed to open database:" << m_db.lastError().text();
        return false;
    }

    // Enable foreign-key enforcement
    QSqlQuery pragma(m_db);
    pragma.exec(QStringLiteral("PRAGMA foreign_keys = ON"));

    return true;
}

void DatabaseManager::closeDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

// ═══════════════════════════════════════════════════════════════════════
// User operations
// ═══════════════════════════════════════════════════════════════════════

User *DatabaseManager::authenticateUser(const QString &username, const QString &password)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, username, password, name, role, "
        "       business_name, address, compliance_doc "
        "FROM users WHERE username = :username AND password = :password"));
    query.bindValue(QStringLiteral(":username"), username);
    query.bindValue(QStringLiteral(":password"), password);

    if (!query.exec()) {
        qWarning() << "authenticateUser: query failed:" << query.lastError().text();
        return nullptr;
    }

    if (!query.next()) {
        return nullptr;
    }

    const int    id   = query.value(0).toInt();
    const QString uname = query.value(1).toString();
    const QString pwd   = query.value(2).toString();
    const QString name  = query.value(3).toString();
    const QString role  = query.value(4).toString();
    const QString biz   = query.value(5).toString();
    const QString addr  = query.value(6).toString();
    const QString comp  = query.value(7).toString();

    const User::Role userRole = User::stringToRole(role);

    switch (userRole) {
    case User::Role::VENDOR_FOOD:
        return new FoodVendor(id, uname, pwd, name, biz, addr, comp);
    case User::Role::VENDOR_ARTISAN:
        return new ArtisanVendor(id, uname, pwd, name, biz, addr, comp);
    case User::Role::MARKET_OPERATOR:
        return new MarketOperator(id, uname, pwd, name);
    case User::Role::SYSTEM_ADMIN:
        return new SystemAdmin(id, uname, pwd, name);
    }

    return nullptr;
}

QList<Vendor *> DatabaseManager::getAllVendors()
{
    QList<Vendor *> vendors;
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, username, password, name, role, "
        "       business_name, address, compliance_doc "
        "FROM users WHERE role IN (:food, :artisan)"));
    query.bindValue(QStringLiteral(":food"),
                    User::roleToString(User::Role::VENDOR_FOOD));
    query.bindValue(QStringLiteral(":artisan"),
                    User::roleToString(User::Role::VENDOR_ARTISAN));

    if (!query.exec()) {
        qWarning() << "getAllVendors: query failed:" << query.lastError().text();
        return vendors;
    }

    while (query.next()) {
        const int     id   = query.value(0).toInt();
        const QString uname = query.value(1).toString();
        const QString pwd   = query.value(2).toString();
        const QString name  = query.value(3).toString();
        const QString role  = query.value(4).toString();
        const QString biz   = query.value(5).toString();
        const QString addr  = query.value(6).toString();
        const QString comp  = query.value(7).toString();

        const User::Role userRole = User::stringToRole(role);

        if (userRole == User::Role::VENDOR_FOOD) {
            vendors.append(new FoodVendor(id, uname, pwd, name, biz, addr, comp));
        } else {
            vendors.append(new ArtisanVendor(id, uname, pwd, name, biz, addr, comp));
        }
    }

    return vendors;
}

Vendor *DatabaseManager::getVendorById(int id)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, username, password, name, role, "
        "       business_name, address, compliance_doc "
        "FROM users WHERE id = :id AND role IN (:food, :artisan)"));
    query.bindValue(QStringLiteral(":id"), id);
    query.bindValue(QStringLiteral(":food"),
                    User::roleToString(User::Role::VENDOR_FOOD));
    query.bindValue(QStringLiteral(":artisan"),
                    User::roleToString(User::Role::VENDOR_ARTISAN));

    if (!query.exec()) {
        qWarning() << "getVendorById: query failed:" << query.lastError().text();
        return nullptr;
    }

    if (!query.next()) {
        return nullptr;
    }

    const int     vid  = query.value(0).toInt();
    const QString uname = query.value(1).toString();
    const QString pwd   = query.value(2).toString();
    const QString name  = query.value(3).toString();
    const QString role  = query.value(4).toString();
    const QString biz   = query.value(5).toString();
    const QString addr  = query.value(6).toString();
    const QString comp  = query.value(7).toString();

    if (User::stringToRole(role) == User::Role::VENDOR_FOOD) {
        return new FoodVendor(vid, uname, pwd, name, biz, addr, comp);
    }
    return new ArtisanVendor(vid, uname, pwd, name, biz, addr, comp);
}

User *DatabaseManager::getUserById(int id)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, username, password, name, role, "
        "       business_name, address, compliance_doc "
        "FROM users WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        qWarning() << "getUserById: query failed:" << query.lastError().text();
        return nullptr;
    }

    if (!query.next()) {
        return nullptr;
    }

    const int     uid  = query.value(0).toInt();
    const QString uname = query.value(1).toString();
    const QString pwd   = query.value(2).toString();
    const QString name  = query.value(3).toString();
    const QString role  = query.value(4).toString();
    const QString biz   = query.value(5).toString();
    const QString addr  = query.value(6).toString();
    const QString comp  = query.value(7).toString();

    const User::Role userRole = User::stringToRole(role);

    switch (userRole) {
    case User::Role::VENDOR_FOOD:
        return new FoodVendor(uid, uname, pwd, name, biz, addr, comp);
    case User::Role::VENDOR_ARTISAN:
        return new ArtisanVendor(uid, uname, pwd, name, biz, addr, comp);
    case User::Role::MARKET_OPERATOR:
        return new MarketOperator(uid, uname, pwd, name);
    case User::Role::SYSTEM_ADMIN:
        return new SystemAdmin(uid, uname, pwd, name);
    }

    return nullptr;
}

// ═══════════════════════════════════════════════════════════════════════
// Schedule operations
// ═══════════════════════════════════════════════════════════════════════

QList<MarketSchedule> DatabaseManager::getAllSchedules()
{
    QList<MarketSchedule> schedules;
    QSqlQuery query(m_db);

    if (!query.exec(QStringLiteral(
            "SELECT id, week_date, max_food_stalls, max_artisan_stalls, "
            "       booked_food_stalls, booked_artisan_stalls "
            "FROM market_schedule ORDER BY week_date ASC"))) {
        qWarning() << "getAllSchedules: query failed:" << query.lastError().text();
        return schedules;
    }

    while (query.next()) {
        MarketSchedule s(query.value(0).toInt(),
                         QDate::fromString(query.value(1).toString(), Qt::ISODate),
                         query.value(2).toInt(),
                         query.value(3).toInt(),
                         query.value(4).toInt(),
                         query.value(5).toInt());
        schedules.append(s);
    }

    return schedules;
}

MarketSchedule DatabaseManager::getScheduleById(int id)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, week_date, max_food_stalls, max_artisan_stalls, "
        "       booked_food_stalls, booked_artisan_stalls "
        "FROM market_schedule WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        qWarning() << "getScheduleById: query failed:" << query.lastError().text();
        return {};
    }

    if (!query.next()) {
        return {};
    }

    return MarketSchedule(query.value(0).toInt(),
                          QDate::fromString(query.value(1).toString(), Qt::ISODate),
                          query.value(2).toInt(),
                          query.value(3).toInt(),
                          query.value(4).toInt(),
                          query.value(5).toInt());
}

void DatabaseManager::updateScheduleStallCounts(int schedule_id,
                                                 int foodBooked,
                                                 int artisanBooked)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "UPDATE market_schedule SET booked_food_stalls = :food, "
        "booked_artisan_stalls = :artisan WHERE id = :id"));
    query.bindValue(QStringLiteral(":food"), foodBooked);
    query.bindValue(QStringLiteral(":artisan"), artisanBooked);
    query.bindValue(QStringLiteral(":id"), schedule_id);

    if (!query.exec()) {
        qWarning() << "updateScheduleStallCounts: query failed:"
                    << query.lastError().text();
    }
}

// ═══════════════════════════════════════════════════════════════════════
// Booking operations
// ═══════════════════════════════════════════════════════════════════════

QList<Booking> DatabaseManager::getBookingsForVendor(int vendor_id)
{
    QList<Booking> bookings;
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, vendor_id, schedule_id, vendor_category, booking_date "
        "FROM bookings WHERE vendor_id = :vid"));
    query.bindValue(QStringLiteral(":vid"), vendor_id);

    if (!query.exec()) {
        qWarning() << "getBookingsForVendor: query failed:" << query.lastError().text();
        return bookings;
    }

    while (query.next()) {
        Booking b(query.value(0).toInt(),
                  query.value(1).toInt(),
                  query.value(2).toInt(),
                  query.value(3).toString(),
                  QDateTime::fromString(query.value(4).toString(), Qt::ISODate));
        bookings.append(b);
    }

    return bookings;
}

bool DatabaseManager::hasActiveBooking(int vendor_id)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT COUNT(*) FROM bookings WHERE vendor_id = :vid"));
    query.bindValue(QStringLiteral(":vid"), vendor_id);

    if (!query.exec() || !query.next()) {
        qWarning() << "hasActiveBooking: query failed:" << query.lastError().text();
        return false;
    }

    return query.value(0).toInt() > 0;
}

Booking DatabaseManager::getActiveBookingForVendor(int vendor_id)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, vendor_id, schedule_id, vendor_category, booking_date "
        "FROM bookings WHERE vendor_id = :vid LIMIT 1"));
    query.bindValue(QStringLiteral(":vid"), vendor_id);

    if (!query.exec()) {
        qWarning() << "getActiveBookingForVendor: query failed:"
                    << query.lastError().text();
        return {};
    }

    if (!query.next()) {
        return {};
    }

    return Booking(query.value(0).toInt(),
                   query.value(1).toInt(),
                   query.value(2).toInt(),
                   query.value(3).toString(),
                   QDateTime::fromString(query.value(4).toString(), Qt::ISODate));
}

bool DatabaseManager::createBooking(int vendor_id, int schedule_id,
                                    const QString &category)
{
    // Insert the booking
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "INSERT INTO bookings (vendor_id, schedule_id, vendor_category, booking_date) "
        "VALUES (:vid, :sid, :cat, :date)"));
    query.bindValue(QStringLiteral(":vid"), vendor_id);
    query.bindValue(QStringLiteral(":sid"), schedule_id);
    query.bindValue(QStringLiteral(":cat"), category);
    query.bindValue(QStringLiteral(":date"),
                    QDateTime::currentDateTime().toString(Qt::ISODate));

    if (!query.exec()) {
        qWarning() << "createBooking: insert failed:" << query.lastError().text();
        return false;
    }

    // Increment the appropriate booked stall count
    const QString column = (category == QStringLiteral("Food"))
                               ? QStringLiteral("booked_food_stalls")
                               : QStringLiteral("booked_artisan_stalls");

    QSqlQuery updateQ(m_db);
    updateQ.prepare(QStringLiteral("UPDATE market_schedule SET %1 = %1 + 1 WHERE id = :sid")
                        .arg(column));
    updateQ.bindValue(QStringLiteral(":sid"), schedule_id);

    if (!updateQ.exec()) {
        qWarning() << "createBooking: schedule update failed:"
                    << updateQ.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::cancelBooking(int bookingId)
{
    // Fetch booking details before deletion
    QSqlQuery fetchQ(m_db);
    fetchQ.prepare(QStringLiteral(
        "SELECT vendor_id, schedule_id, vendor_category FROM bookings WHERE id = :id"));
    fetchQ.bindValue(QStringLiteral(":id"), bookingId);

    if (!fetchQ.exec() || !fetchQ.next()) {
        qWarning() << "cancelBooking: booking not found:" << fetchQ.lastError().text();
        return false;
    }

    const int     schedule_id = fetchQ.value(1).toInt();
    const QString category   = fetchQ.value(2).toString();

    // Delete the booking
    QSqlQuery deleteQ(m_db);
    deleteQ.prepare(QStringLiteral("DELETE FROM bookings WHERE id = :id"));
    deleteQ.bindValue(QStringLiteral(":id"), bookingId);

    if (!deleteQ.exec()) {
        qWarning() << "cancelBooking: delete failed:" << deleteQ.lastError().text();
        return false;
    }

    // Decrement the booked stall count
    const QString column = (category == QStringLiteral("Food"))
                               ? QStringLiteral("booked_food_stalls")
                               : QStringLiteral("booked_artisan_stalls");

    QSqlQuery updateQ(m_db);
    updateQ.prepare(QStringLiteral("UPDATE market_schedule SET %1 = MAX(%1 - 1, 0) WHERE id = :sid")
                        .arg(column));
    updateQ.bindValue(QStringLiteral(":sid"), schedule_id);

    if (!updateQ.exec()) {
        qWarning() << "cancelBooking: schedule update failed:"
                    << updateQ.lastError().text();
        return false;
    }

    // ── Auto-book from waitlist (FIFO) ──────────────────────────────
    WaitlistEntry next = getNextInWaitlist(schedule_id, category);
    if (next.getId() != -1) {
        const int waitVendorId = next.getVendorId();

        // Only auto-book if the vendor does not already hold an active booking
        if (!hasActiveBooking(waitVendorId)) {
            if (createBooking(waitVendorId, schedule_id, category)) {
                removeFromWaitlist(next.getId());
                addNotification(waitVendorId,
                                QStringLiteral("You have been automatically booked "
                                               "for schedule %1 (%2) from the waitlist.")
                                    .arg(schedule_id)
                                    .arg(category));
            }
        }
    }

    return true;
}

QList<Booking> DatabaseManager::getAllBookings()
{
    QList<Booking> bookings;
    QSqlQuery query(m_db);

    if (!query.exec(QStringLiteral(
            "SELECT id, vendor_id, schedule_id, vendor_category, booking_date "
            "FROM bookings"))) {
        qWarning() << "getAllBookings: query failed:" << query.lastError().text();
        return bookings;
    }

    while (query.next()) {
        Booking b(query.value(0).toInt(),
                  query.value(1).toInt(),
                  query.value(2).toInt(),
                  query.value(3).toString(),
                  QDateTime::fromString(query.value(4).toString(), Qt::ISODate));
        bookings.append(b);
    }

    return bookings;
}

// ═══════════════════════════════════════════════════════════════════════
// Waitlist operations
// ═══════════════════════════════════════════════════════════════════════

QList<WaitlistEntry> DatabaseManager::getWaitlistForScheduleAndCategory(
    int schedule_id, const QString &category)
{
    QList<WaitlistEntry> entries;
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, vendor_id, schedule_id, category, join_date, position "
        "FROM waitlist WHERE schedule_id = :sid AND category = :cat "
        "ORDER BY join_date ASC"));
    query.bindValue(QStringLiteral(":sid"), schedule_id);
    query.bindValue(QStringLiteral(":cat"), category);

    if (!query.exec()) {
        qWarning() << "getWaitlistForScheduleAndCategory: query failed:"
                    << query.lastError().text();
        return entries;
    }

    while (query.next()) {
        WaitlistEntry e(query.value(0).toInt(),
                        query.value(1).toInt(),
                        query.value(2).toInt(),
                        query.value(3).toString(),
                        QDateTime::fromString(query.value(4).toString(), Qt::ISODate),
                        query.value(5).toInt());
        entries.append(e);
    }

    return entries;
}

QList<WaitlistEntry> DatabaseManager::getWaitlistForVendor(int vendor_id)
{
    QList<WaitlistEntry> entries;
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, vendor_id, schedule_id, category, join_date, position "
        "FROM waitlist WHERE vendor_id = :vid ORDER BY join_date ASC"));
    query.bindValue(QStringLiteral(":vid"), vendor_id);

    if (!query.exec()) {
        qWarning() << "getWaitlistForVendor: query failed:" << query.lastError().text();
        return entries;
    }

    while (query.next()) {
        WaitlistEntry e(query.value(0).toInt(),
                        query.value(1).toInt(),
                        query.value(2).toInt(),
                        query.value(3).toString(),
                        QDateTime::fromString(query.value(4).toString(), Qt::ISODate),
                        query.value(5).toInt());
        entries.append(e);
    }

    return entries;
}

bool DatabaseManager::addToWaitlist(int vendor_id, int schedule_id,
                                    const QString &category)
{
    // Determine the next position for this schedule+category
    QSqlQuery posQ(m_db);
    posQ.prepare(QStringLiteral(
        "SELECT COALESCE(MAX(position), 0) + 1 FROM waitlist "
        "WHERE schedule_id = :sid AND category = :cat"));
    posQ.bindValue(QStringLiteral(":sid"), schedule_id);
    posQ.bindValue(QStringLiteral(":cat"), category);

    int nextPos = 1;
    if (posQ.exec() && posQ.next()) {
        nextPos = posQ.value(0).toInt();
    }

    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "INSERT INTO waitlist (vendor_id, schedule_id, category, join_date, position) "
        "VALUES (:vid, :sid, :cat, :date, :pos)"));
    query.bindValue(QStringLiteral(":vid"), vendor_id);
    query.bindValue(QStringLiteral(":sid"), schedule_id);
    query.bindValue(QStringLiteral(":cat"), category);
    query.bindValue(QStringLiteral(":date"),
                    QDateTime::currentDateTime().toString(Qt::ISODate));
    query.bindValue(QStringLiteral(":pos"), nextPos);

    if (!query.exec()) {
        qWarning() << "addToWaitlist: insert failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::removeFromWaitlist(int entryId)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral("DELETE FROM waitlist WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), entryId);

    if (!query.exec()) {
        qWarning() << "removeFromWaitlist: delete failed:" << query.lastError().text();
        return false;
    }

    return true;
}

WaitlistEntry DatabaseManager::getNextInWaitlist(int schedule_id,
                                                  const QString &category)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, vendor_id, schedule_id, category, join_date, position "
        "FROM waitlist WHERE schedule_id = :sid AND category = :cat "
        "ORDER BY join_date ASC LIMIT 1"));
    query.bindValue(QStringLiteral(":sid"), schedule_id);
    query.bindValue(QStringLiteral(":cat"), category);

    if (!query.exec()) {
        qWarning() << "getNextInWaitlist: query failed:" << query.lastError().text();
        return {};
    }

    if (!query.next()) {
        return {};
    }

    return WaitlistEntry(query.value(0).toInt(),
                         query.value(1).toInt(),
                         query.value(2).toInt(),
                         query.value(3).toString(),
                         QDateTime::fromString(query.value(4).toString(), Qt::ISODate),
                         query.value(5).toInt());
}

bool DatabaseManager::isVendorOnWaitlist(int vendor_id, int schedule_id)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT COUNT(*) FROM waitlist "
        "WHERE vendor_id = :vid AND schedule_id = :sid"));
    query.bindValue(QStringLiteral(":vid"), vendor_id);
    query.bindValue(QStringLiteral(":sid"), schedule_id);

    if (!query.exec() || !query.next()) {
        qWarning() << "isVendorOnWaitlist: query failed:" << query.lastError().text();
        return false;
    }

    return query.value(0).toInt() > 0;
}

// ═══════════════════════════════════════════════════════════════════════
// Notification tracking
// ═══════════════════════════════════════════════════════════════════════

QStringList DatabaseManager::getNotificationsForVendor(int vendor_id)
{
    QStringList messages;
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, message FROM notifications WHERE vendor_id = :vid "
        "ORDER BY id ASC"));
    query.bindValue(QStringLiteral(":vid"), vendor_id);

    if (!query.exec()) {
        qWarning() << "getNotificationsForVendor: query failed:"
                    << query.lastError().text();
        return messages;
    }

    QList<int> ids;
    while (query.next()) {
        ids.append(query.value(0).toInt());
        messages.append(query.value(1).toString());
    }

    // Clear the returned notifications
    for (const int id : ids) {
        QSqlQuery delQ(m_db);
        delQ.prepare(QStringLiteral("DELETE FROM notifications WHERE id = :id"));
        delQ.bindValue(QStringLiteral(":id"), id);
        if (!delQ.exec()) {
            qWarning() << "getNotificationsForVendor: delete failed:"
                        << delQ.lastError().text();
        }
    }

    return messages;
}

void DatabaseManager::addNotification(int vendor_id, const QString &message)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "INSERT INTO notifications (vendor_id, message) VALUES (:vid, :msg)"));
    query.bindValue(QStringLiteral(":vid"), vendor_id);
    query.bindValue(QStringLiteral(":msg"), message);

    if (!query.exec()) {
        qWarning() << "addNotification: insert failed:" << query.lastError().text();
    }
}
