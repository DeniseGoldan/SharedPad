#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

#include "ErrorHandler.h"
#include "GenericResponseMessage.h"
#include "JsonRequestMessageGenerator.h"
#include "NamespaceSPP.h"
#include "Client.h"
#include "spdlog/spdlog.h"

#include <string.h>
#include <stdio.h>

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(QWidget* parent = 0);
    ~LoginWindow();

private slots:
    void OnLogin();

private:
    QLabel* usernameLabel;
    QLineEdit* userLineEdit;
    QPushButton* loginButton;
    GenericResponseMessage* tryToLogin(std::string username);
};

#endif //LOGINWINDOW_H
