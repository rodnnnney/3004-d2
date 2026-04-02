#pragma once

#include "user.h"

class SystemAdmin : public User {
public:
    SystemAdmin() = default;
    SystemAdmin(int id, const QString &username, const QString &password,
                const QString &name);
    ~SystemAdmin() override = default;
};
