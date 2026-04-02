#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>

class User;

class OperatorDashboard : public QWidget {
    Q_OBJECT

public:
    explicit OperatorDashboard(QWidget *parent = nullptr);

    void setOperator(User *op);
    void refreshDashboard();

private slots:
    void onBookForVendorClicked();
    void onCancelBookingClicked(int bookingId);
    void onRemoveWaitlistClicked(int entryId);

private:
    void setupUi();
    void setupBookTab(QWidget *tab);
    void setupCancelTab(QWidget *tab);
    void setupWaitlistTab(QWidget *tab);

    void refreshBookTab();
    void refreshCancelTab();
    void refreshWaitlistTab();

    User *m_operator = nullptr;

    QLabel *m_headerLabel = nullptr;
    QTabWidget *m_tabWidget = nullptr;

    // Book tab
    QComboBox *m_vendorCombo = nullptr;
    QComboBox *m_weekCombo = nullptr;
    QPushButton *m_bookButton = nullptr;

    // Cancel tab
    QTableWidget *m_bookingsTable = nullptr;

    // Waitlist tab
    QTableWidget *m_waitlistTable = nullptr;
};
