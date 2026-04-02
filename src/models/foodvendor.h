#pragma once

#include "vendor.h"

class FoodVendor : public Vendor {
public:
    FoodVendor() = default;
    FoodVendor(int id, const QString &username, const QString &password,
               const QString &name,
               const QString &businessName, const QString &address,
               const QString &complianceDoc);
    ~FoodVendor() override = default;

    QString getCategory() const override;
};
