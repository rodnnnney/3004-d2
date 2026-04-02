#include "foodvendor.h"

FoodVendor::FoodVendor(int id, const QString &username, const QString &password,
                       const QString &name,
                       const QString &businessName, const QString &address,
                       const QString &complianceDoc)
    : Vendor(id, username, password, name, Role::VENDOR_FOOD,
             businessName, address, complianceDoc)
{
}

QString FoodVendor::getCategory() const
{
    return QStringLiteral("Food");
}
