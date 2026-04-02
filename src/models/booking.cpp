#include "booking.h"

Booking::Booking(int id, int vendorId, int scheduleId,
                 const QString &vendorCategory, const QDateTime &bookingDate)
    : m_id(id)
    , m_vendorId(vendorId)
    , m_scheduleId(scheduleId)
    , m_vendorCategory(vendorCategory)
    , m_bookingDate(bookingDate)
{
}

int Booking::getId() const { return m_id; }
void Booking::setId(int id) { m_id = id; }

int Booking::getVendorId() const { return m_vendorId; }
void Booking::setVendorId(int vendorId) { m_vendorId = vendorId; }

int Booking::getScheduleId() const { return m_scheduleId; }
void Booking::setScheduleId(int scheduleId) { m_scheduleId = scheduleId; }

QString Booking::getVendorCategory() const { return m_vendorCategory; }
void Booking::setVendorCategory(const QString &vendorCategory) { m_vendorCategory = vendorCategory; }

QDateTime Booking::getBookingDate() const { return m_bookingDate; }
void Booking::setBookingDate(const QDateTime &bookingDate) { m_bookingDate = bookingDate; }
