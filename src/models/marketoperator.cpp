#include "marketoperator.h"

MarketOperator::MarketOperator(int id, const QString &username,
                               const QString &password, const QString &name)
    : User(id, username, password, name, Role::MARKET_OPERATOR)
{
}
