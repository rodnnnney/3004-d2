#pragma once

#include "vendor.h"

class ArtisanVendor : public Vendor {
public:
    ArtisanVendor() = default;
    ArtisanVendor(int id, const QString &username, const QString &password,
                  const QString &name,
                  const QString &businessName, const QString &address,
                  const QString &complianceDoc);
    ~ArtisanVendor() override = default;

    QString getCategory() const override;
};
