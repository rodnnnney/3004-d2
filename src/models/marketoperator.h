#pragma once

#include "user.h"

class MarketOperator : public User {
public:
    MarketOperator() = default;
    MarketOperator(int id, const QString &username, const QString &password,
                   const QString &name);
    ~MarketOperator() override = default;
};
