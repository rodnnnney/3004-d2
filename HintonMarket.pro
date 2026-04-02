QT       += core gui widgets sql

CONFIG += c++17

TARGET = HintonMarket
TEMPLATE = app

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/models/user.cpp \
    src/models/vendor.cpp \
    src/models/foodvendor.cpp \
    src/models/artisanvendor.cpp \
    src/models/marketoperator.cpp \
    src/models/systemadmin.cpp \
    src/models/marketschedule.cpp \
    src/models/booking.cpp \
    src/models/waitlistentry.cpp \
    src/database/databasemanager.cpp \
    src/ui/loginwindow.cpp \
    src/ui/mainwindow.cpp \
    src/ui/vendordashboard.cpp \
    src/ui/operatordashboard.cpp \
    src/ui/admindashboard.cpp \
    src/ui/bookingdialog.cpp \
    src/ui/waitlistdialog.cpp

HEADERS += \
    src/models/user.h \
    src/models/vendor.h \
    src/models/foodvendor.h \
    src/models/artisanvendor.h \
    src/models/marketoperator.h \
    src/models/systemadmin.h \
    src/models/marketschedule.h \
    src/models/booking.h \
    src/models/waitlistentry.h \
    src/database/databasemanager.h \
    src/ui/loginwindow.h \
    src/ui/mainwindow.h \
    src/ui/vendordashboard.h \
    src/ui/operatordashboard.h \
    src/ui/admindashboard.h \
    src/ui/bookingdialog.h \
    src/ui/waitlistdialog.h

# Copy database file to build directory
db.files = hintonMarket.sqlite3
db.path = $$OUT_PWD
INSTALLS += db

# Ensure database is copied to build output
QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($$PWD/hintonMarket.sqlite3) $$shell_path($$OUT_PWD/hintonMarket.sqlite3)
