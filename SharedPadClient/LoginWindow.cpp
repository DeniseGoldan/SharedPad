#include "LoginWindow.h"
#include "ui_LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(onLoginButtonPressed()));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::onLoginButtonPressed()
{
    QString username =  ui->usernameLineEdit->text();
    if (username.isEmpty())
    {
        QMessageBox::information(this, tr("Error message"), "Username can't be blank!");
    }
    else
    {
        Client * client = new Client();
        GenericResponse * responseFromServer = client->login(username.toStdString());
        switch(responseFromServer->getCode())
        {
        case LOGIN_FAILED_CODE :
        {
            QMessageBox::critical(this,"Login failed!","The username you provided is already registered.");
            break;
        }
        case LOGIN_APPROVED_CODE :
        {
            notepadWindow = new NotepadWindow(this);
            notepadWindow->setUsername(username);
            notepadWindow->show();
            HeartBeatSender *sender = new HeartBeatSender();
            sender->setUsername(username);
            sender->sendUpdates();
            break;
        }
        case CONNECTION_FAILED_CODE:
        {
            QMessageBox::critical(this,"Error","Server failed.");
            exit(EXIT_FAILURE);
        }
        }
    }
    this->destroy();
}
