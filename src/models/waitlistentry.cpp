#include "waitlistentry.h"

WaitlistEntry::WaitlistEntry(int id, int vendorId, int scheduleId,
                             const QString &category, const QDateTime &joinDate,
                             int position)
    : m_id(id)
    , m_vendorId(vendorId)
    , m_scheduleId(scheduleId)
    , m_category(category)
    , m_joinDate(joinDate)
    , m_position(position)
{
}

int WaitlistEntry::getId() const { return m_id; }
void WaitlistEntry::setId(int id) { m_id = id; }

int WaitlistEntry::getVendorId() const { return m_vendorId; }
void WaitlistEntry::setVendorId(int vendorId) { m_vendorId = vendorId; }

int WaitlistEntry::getScheduleId() const { return m_scheduleId; }
void WaitlistEntry::setScheduleId(int scheduleId) { m_scheduleId = scheduleId; }

QString WaitlistEntry::getCategory() const { return m_category; }
void WaitlistEntry::setCategory(const QString &category) { m_category = category; }

QDateTime WaitlistEntry::getJoinDate() const { return m_joinDate; }
void WaitlistEntry::setJoinDate(const QDateTime &joinDate) { m_joinDate = joinDate; }

int WaitlistEntry::getPosition() const { return m_position; }
void WaitlistEntry::setPosition(int position) { m_position = position; }
