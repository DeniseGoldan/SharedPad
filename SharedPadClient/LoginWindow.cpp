#include "LoginWindow.h"
#include "ui_LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(OnLoginButtonPressed()));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::OnLoginButtonPressed()
{
    QString username =  ui->usernameLineEdit->text();
    if (username.isEmpty())
    {
        QMessageBox::information(this, tr("Error message"), "Username can't be blank!");
    }
    else
    {
        Client * client = new Client();
        GenericResponseMessage * responseFromServer = client->login(username.toStdString());
        switch(responseFromServer->getCode())
        {
            case LOGIN_FAILED_CODE :
            {
                QMessageBox::critical(this,"Login failed!","The username you provided is already registered.");
                break;
            }
            case LOGIN_APPROVED_CODE :
            {
                QMessageBox::information(this,"Login approved!","The username you provided has now been registered.");
                notepadWindow = new NotepadWindow(this);
                notepadWindow->belongsTo(username);
                notepadWindow->show();
                QuerySender *sender = new QuerySender();
                sender->setUsername(username);
                sender->sendUpdates();
                break;
            }
        }
    }
    this->destroy();
}
