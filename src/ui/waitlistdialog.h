#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class WaitlistDialog : public QDialog {
    Q_OBJECT

public:
    explicit WaitlistDialog(const QString &vendorName,
                            const QString &marketWeek,
                            const QString &category,
                            int currentQueueSize,
                            QWidget *parent = nullptr);

private:
    void setupUi(const QString &vendorName,
                 const QString &marketWeek,
                 const QString &category,
                 int currentQueueSize);
};
