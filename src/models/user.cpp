#include "user.h"

User::User(int id, const QString &username, const QString &password,
           const QString &name, Role role)
    : m_id(id)
    , m_username(username)
    , m_password(password)
    , m_name(name)
    , m_role(role)
{
}

int User::getId() const { return m_id; }
void User::setId(int id) { m_id = id; }

QString User::getUsername() const { return m_username; }
void User::setUsername(const QString &username) { m_username = username; }

QString User::getPassword() const { return m_password; }
void User::setPassword(const QString &password) { m_password = password; }

QString User::getName() const { return m_name; }
void User::setName(const QString &name) { m_name = name; }

User::Role User::getRole() const { return m_role; }
void User::setRole(Role role) { m_role = role; }

QString User::roleToString(Role role)
{
    switch (role) {
    case Role::VENDOR_FOOD:      return QStringLiteral("VENDOR_FOOD");
    case Role::VENDOR_ARTISAN:   return QStringLiteral("VENDOR_ARTISAN");
    case Role::MARKET_OPERATOR:  return QStringLiteral("MARKET_OPERATOR");
    case Role::SYSTEM_ADMIN:     return QStringLiteral("SYSTEM_ADMIN");
    }
    return QStringLiteral("UNKNOWN");
}

User::Role User::stringToRole(const QString &str)
{
    if (str == QLatin1String("VENDOR_FOOD"))      return Role::VENDOR_FOOD;
    if (str == QLatin1String("VENDOR_ARTISAN"))    return Role::VENDOR_ARTISAN;
    if (str == QLatin1String("MARKET_OPERATOR"))   return Role::MARKET_OPERATOR;
    if (str == QLatin1String("SYSTEM_ADMIN"))      return Role::SYSTEM_ADMIN;
    return Role::VENDOR_FOOD;
}
