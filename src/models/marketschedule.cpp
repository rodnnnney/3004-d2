#include "marketschedule.h"

MarketSchedule::MarketSchedule(int id, const QDate &weekDate,
                               int maxFoodStalls, int maxArtisanStalls,
                               int bookedFoodStalls, int bookedArtisanStalls)
    : m_id(id)
    , m_weekDate(weekDate)
    , m_maxFoodStalls(maxFoodStalls)
    , m_maxArtisanStalls(maxArtisanStalls)
    , m_bookedFoodStalls(bookedFoodStalls)
    , m_bookedArtisanStalls(bookedArtisanStalls)
{
}

int MarketSchedule::getId() const { return m_id; }
void MarketSchedule::setId(int id) { m_id = id; }

QDate MarketSchedule::getWeekDate() const { return m_weekDate; }
void MarketSchedule::setWeekDate(const QDate &weekDate) { m_weekDate = weekDate; }

int MarketSchedule::getMaxFoodStalls() const { return m_maxFoodStalls; }
void MarketSchedule::setMaxFoodStalls(int max) { m_maxFoodStalls = max; }

int MarketSchedule::getMaxArtisanStalls() const { return m_maxArtisanStalls; }
void MarketSchedule::setMaxArtisanStalls(int max) { m_maxArtisanStalls = max; }

int MarketSchedule::getBookedFoodStalls() const { return m_bookedFoodStalls; }
void MarketSchedule::setBookedFoodStalls(int count) { m_bookedFoodStalls = count; }

int MarketSchedule::getBookedArtisanStalls() const { return m_bookedArtisanStalls; }
void MarketSchedule::setBookedArtisanStalls(int count) { m_bookedArtisanStalls = count; }

bool MarketSchedule::hasFoodAvailability() const
{
    return m_bookedFoodStalls < m_maxFoodStalls;
}

bool MarketSchedule::hasArtisanAvailability() const
{
    return m_bookedArtisanStalls < m_maxArtisanStalls;
}

QString MarketSchedule::getWeekLabel() const
{
    return QStringLiteral("Week of ") + m_weekDate.toString(Qt::ISODate);
}
