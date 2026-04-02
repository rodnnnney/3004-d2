#include "waitlistdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFont>

WaitlistDialog::WaitlistDialog(const QString &vendorName,
                               const QString &marketWeek,
                               const QString &category,
                               int currentQueueSize,
                               QWidget *parent)
    : QDialog(parent)
{
    setupUi(vendorName, marketWeek, category, currentQueueSize);
}

void WaitlistDialog::setupUi(const QString &vendorName,
                              const QString &marketWeek,
                              const QString &category,
                              int currentQueueSize)
{
    setWindowTitle("Join Waitlist");
    setMinimumWidth(360);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // ── Header ───────────────────────────────────────────────────────
    auto *headerLabel = new QLabel("Join Waitlist");
    QFont headerFont;
    headerFont.setPointSize(14);
    headerFont.setBold(true);
    headerLabel->setFont(headerFont);
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("color: #2c3e50; margin-bottom: 6px;");

    mainLayout->addWidget(headerLabel);

    // ── Details ──────────────────────────────────────────────────────
    auto *detailsGroup = new QGroupBox("Waitlist Details");
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
    formLayout->addRow("Current Queue Size:", makeValueLabel(QString::number(currentQueueSize)));

    mainLayout->addWidget(detailsGroup);

    // ── Info Label ───────────────────────────────────────────────────
    auto *infoLabel = new QLabel(
        QString("You will be position %1 in the queue. "
                "You will be automatically booked if a stall becomes available.")
            .arg(currentQueueSize + 1));
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet(
        "background-color: #eaf2f8; border: 1px solid #aed6f1;"
        "border-radius: 4px; padding: 10px; color: #2471a3; font-size: 11px;"
    );
    mainLayout->addWidget(infoLabel);

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

    auto *okBtn = new QPushButton("Join Waitlist");
    okBtn->setMinimumHeight(34);
    okBtn->setMinimumWidth(140);
    okBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #f39c12; color: white; border: none;"
        "  border-radius: 4px; padding: 6px 16px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #e67e22; }"
    );

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(okBtn);

    mainLayout->addLayout(buttonLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
}
