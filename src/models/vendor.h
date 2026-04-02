#pragma once

#include "user.h"
#include <QString>

class Vendor : public User {
public:
    Vendor() = default;
    Vendor(int id, const QString &username, const QString &password,
           const QString &name, Role role,
           const QString &businessName, const QString &address,
           const QString &complianceDoc);
    ~Vendor() override = default;

    QString getBusinessName() const;
    void setBusinessName(const QString &businessName);

    QString getAddress() const;
    void setAddress(const QString &address);

    QString getComplianceDoc() const;
    void setComplianceDoc(const QString &complianceDoc);

    virtual QString getCategory() const = 0;

private:
    QString m_businessName;
    QString m_address;
    QString m_complianceDoc;
};
