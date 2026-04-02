#include "systemadmin.h"

SystemAdmin::SystemAdmin(int id, const QString &username,
                         const QString &password, const QString &name)
    : User(id, username, password, name, Role::SYSTEM_ADMIN)
{
}
