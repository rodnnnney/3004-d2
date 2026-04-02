#pragma once

#include <QDateTime>
#include <QString>

class Booking {
public:
    Booking() = default;
    Booking(int id, int vendorId, int scheduleId,
            const QString &vendorCategory, const QDateTime &bookingDate);

    int getId() const;
    void setId(int id);

    int getVendorId() const;
    void setVendorId(int vendorId);

    int getScheduleId() const;
    void setScheduleId(int scheduleId);

    QString getVendorCategory() const;
    void setVendorCategory(const QString &vendorCategory);

    QDateTime getBookingDate() const;
    void setBookingDate(const QDateTime &bookingDate);

private:
    int m_id = -1;
    int m_vendorId = -1;
    int m_scheduleId = -1;
    QString m_vendorCategory;
    QDateTime m_bookingDate;
};
