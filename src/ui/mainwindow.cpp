#include "mainwindow.h"
#include "loginwindow.h"
#include "vendordashboard.h"
#include "operatordashboard.h"
#include "admindashboard.h"

#include "../models/user.h"
#include "../models/vendor.h"

#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupMenuBar();
}

void MainWindow::setupUi()
{
    setWindowTitle("HintonMarket System");
    setMinimumSize(900, 600);

    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    // ── Create pages ─────────────────────────────────────────────────
    m_loginWindow = new LoginWindow;
    m_vendorDashboard = new VendorDashboard;
    m_operatorDashboard = new OperatorDashboard;
    m_adminDashboard = new AdminDashboard;

    m_stack->addWidget(m_loginWindow);        // index 0
    m_stack->addWidget(m_vendorDashboard);    // index 1
    m_stack->addWidget(m_operatorDashboard);  // index 2
    m_stack->addWidget(m_adminDashboard);     // index 3

    m_stack->setCurrentIndex(0);

    // ── Connect login signal ─────────────────────────────────────────
    connect(m_loginWindow, &LoginWindow::loginSuccess,
            this, &MainWindow::onLoginSuccess);
}

void MainWindow::setupMenuBar()
{
    auto *fileMenu = menuBar()->addMenu("&File");

    m_logoutAction = new QAction("Logout", this);
    m_logoutAction->setEnabled(false);
    connect(m_logoutAction, &QAction::triggered, this, &MainWindow::onLogout);

    m_exitAction = new QAction("Exit", this);
    connect(m_exitAction, &QAction::triggered, qApp, &QApplication::quit);

    fileMenu->addAction(m_logoutAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exitAction);
}

void MainWindow::onLoginSuccess(User *user)
{
    m_currentUser = user;
    m_logoutAction->setEnabled(true);

    switch (user->getRole()) {
    case User::Role::VENDOR_FOOD:
    case User::Role::VENDOR_ARTISAN: {
        auto *vendor = dynamic_cast<Vendor *>(user);
        if (vendor) {
            m_vendorDashboard->setVendor(vendor);
            m_stack->setCurrentWidget(m_vendorDashboard);
            setWindowTitle("HintonMarket System - Vendor Dashboard");
        }
        break;
    }
    case User::Role::MARKET_OPERATOR:
        m_operatorDashboard->setOperator(user);
        m_stack->setCurrentWidget(m_operatorDashboard);
        setWindowTitle("HintonMarket System - Operator Dashboard");
        break;

    case User::Role::SYSTEM_ADMIN:
        m_adminDashboard->setAdmin(user);
        m_stack->setCurrentWidget(m_adminDashboard);
        setWindowTitle("HintonMarket System - Admin Dashboard");
        break;
    }
}

void MainWindow::onLogout()
{
    m_stack->setCurrentWidget(m_loginWindow);
    m_logoutAction->setEnabled(false);
    setWindowTitle("HintonMarket System");

    delete m_currentUser;
    m_currentUser = nullptr;
}
