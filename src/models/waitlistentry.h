#pragma once

#include <QDateTime>
#include <QString>

class WaitlistEntry {
public:
    WaitlistEntry() = default;
    WaitlistEntry(int id, int vendorId, int scheduleId,
                  const QString &category, const QDateTime &joinDate,
                  int position);

    int getId() const;
    void setId(int id);

    int getVendorId() const;
    void setVendorId(int vendorId);

    int getScheduleId() const;
    void setScheduleId(int scheduleId);

    QString getCategory() const;
    void setCategory(const QString &category);

    QDateTime getJoinDate() const;
    void setJoinDate(const QDateTime &joinDate);

    int getPosition() const;
    void setPosition(int position);

private:
    int m_id = -1;
    int m_vendorId = -1;
    int m_scheduleId = -1;
    QString m_category;
    QDateTime m_joinDate;
    int m_position = 0;
};
