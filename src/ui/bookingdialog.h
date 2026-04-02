#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class BookingDialog : public QDialog {
    Q_OBJECT

public:
    explicit BookingDialog(const QString &vendorName,
                           const QString &marketWeek,
                           const QString &category,
                           QWidget *parent = nullptr);

private:
    void setupUi(const QString &vendorName,
                 const QString &marketWeek,
                 const QString &category);
};
