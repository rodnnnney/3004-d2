#include <QApplication>
#include <QMessageBox>
#include "ui/mainwindow.h"
#include "database/databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("HintonMarket");

    // Open the SQLite database
    QString dbPath = QCoreApplication::applicationDirPath() + "/hintonMarket.sqlite3";
    if (!DatabaseManager::instance().openDatabase(dbPath)) {
        QMessageBox::critical(nullptr, "Database Error",
                              "Failed to open database at:\n" + dbPath +
                              "\n\nPlease ensure hintonMarket.sqlite3 is in the same directory as the executable.");
        return 1;
    }

    MainWindow mainWindow;
    mainWindow.show();

    int result = app.exec();
    DatabaseManager::instance().closeDatabase();
    return result;
}
