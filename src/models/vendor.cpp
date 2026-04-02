#include "vendor.h"

Vendor::Vendor(int id, const QString &username, const QString &password,
               const QString &name, Role role,
               const QString &businessName, const QString &address,
               const QString &complianceDoc)
    : User(id, username, password, name, role)
    , m_businessName(businessName)
    , m_address(address)
    , m_complianceDoc(complianceDoc)
{
}

QString Vendor::getBusinessName() const { return m_businessName; }
void Vendor::setBusinessName(const QString &businessName) { m_businessName = businessName; }

QString Vendor::getAddress() const { return m_address; }
void Vendor::setAddress(const QString &address) { m_address = address; }

QString Vendor::getComplianceDoc() const { return m_complianceDoc; }
void Vendor::setComplianceDoc(const QString &complianceDoc) { m_complianceDoc = complianceDoc; }
