#include "bookingdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFont>

BookingDialog::BookingDialog(const QString &vendorName,
                             const QString &marketWeek,
                             const QString &category,
                             QWidget *parent)
    : QDialog(parent)
{
    setupUi(vendorName, marketWeek, category);
}

void BookingDialog::setupUi(const QString &vendorName,
                             const QString &marketWeek,
                             const QString &category)
{
    setWindowTitle("Confirm Booking");
    setMinimumWidth(350);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // ── Header ───────────────────────────────────────────────────────
    auto *headerLabel = new QLabel("Confirm Stall Booking");
    QFont headerFont;
    headerFont.setPointSize(14);
    headerFont.setBold(true);
    headerLabel->setFont(headerFont);
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("color: #2c3e50; margin-bottom: 6px;");

    mainLayout->addWidget(headerLabel);

    // ── Details ──────────────────────────────────────────────────────
    auto *detailsGroup = new QGroupBox("Booking Details");
    auto *formLayout = new QFormLayout(detailsGroup);
    formLayout->setSpacing(10);

    auto makeValueLabel = [](const QString &text) -> QLabel * {
        auto *label = new QLabel(text);
        label->setStyleSheet("font-weight: bold; color: #2c3e50;");
        return label;
    };

    formLayout->addRow("Vendor:", makeValueLabel(vendorName));
    formLayout->addRow("Market Week:", makeValueLabel(marketWeek));
    formLayout->addRow("Category:", makeValueLabel(category));

    mainLayout->addWidget(detailsGroup);

    // ── Buttons ──────────────────────────────────────────────────────
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(12);

    auto *cancelBtn = new QPushButton("Cancel");
    cancelBtn->setMinimumHeight(34);
    cancelBtn->setMinimumWidth(100);
    cancelBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #bdc3c7; color: #2c3e50; border: none;"
        "  border-radius: 4px; padding: 6px 16px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #95a5a6; }"
    );

    auto *okBtn = new QPushButton("Confirm Booking");
    okBtn->setMinimumHeight(34);
    okBtn->setMinimumWidth(140);
    okBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #27ae60; color: white; border: none;"
        "  border-radius: 4px; padding: 6px 16px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #219a52; }"
    );

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(okBtn);

    mainLayout->addLayout(buttonLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
}
