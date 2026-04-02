#pragma once

#include <QString>

class User {
public:
    enum class Role {
        VENDOR_FOOD,
        VENDOR_ARTISAN,
        MARKET_OPERATOR,
        SYSTEM_ADMIN
    };

    User() = default;
    User(int id, const QString &username, const QString &password,
         const QString &name, Role role);
    virtual ~User() = default;

    int getId() const;
    void setId(int id);

    QString getUsername() const;
    void setUsername(const QString &username);

    QString getPassword() const;
    void setPassword(const QString &password);

    QString getName() const;
    void setName(const QString &name);

    Role getRole() const;
    void setRole(Role role);

    static QString roleToString(Role role);
    static Role stringToRole(const QString &str);

private:
    int m_id = -1;
    QString m_username;
    QString m_password;
    QString m_name;
    Role m_role = Role::VENDOR_FOOD;
};
