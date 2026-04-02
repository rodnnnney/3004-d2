#pragma once

#include <QDate>
#include <QString>

class MarketSchedule {
public:
    MarketSchedule() = default;
    MarketSchedule(int id, const QDate &weekDate,
                   int maxFoodStalls = 2, int maxArtisanStalls = 2,
                   int bookedFoodStalls = 0, int bookedArtisanStalls = 0);

    int getId() const;
    void setId(int id);

    QDate getWeekDate() const;
    void setWeekDate(const QDate &weekDate);

    int getMaxFoodStalls() const;
    void setMaxFoodStalls(int max);

    int getMaxArtisanStalls() const;
    void setMaxArtisanStalls(int max);

    int getBookedFoodStalls() const;
    void setBookedFoodStalls(int count);

    int getBookedArtisanStalls() const;
    void setBookedArtisanStalls(int count);

    bool hasFoodAvailability() const;
    bool hasArtisanAvailability() const;

    QString getWeekLabel() const;

private:
    int m_id = -1;
    QDate m_weekDate;
    int m_maxFoodStalls = 2;
    int m_maxArtisanStalls = 2;
    int m_bookedFoodStalls = 0;
    int m_bookedArtisanStalls = 0;
};
