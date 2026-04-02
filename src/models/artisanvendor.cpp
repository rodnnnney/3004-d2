#include "artisanvendor.h"

ArtisanVendor::ArtisanVendor(int id, const QString &username, const QString &password,
                             const QString &name,
                             const QString &businessName, const QString &address,
                             const QString &complianceDoc)
    : Vendor(id, username, password, name, Role::VENDOR_ARTISAN,
             businessName, address, complianceDoc)
{
}

QString ArtisanVendor::getCategory() const
{
    return QStringLiteral("Artisan");
}
