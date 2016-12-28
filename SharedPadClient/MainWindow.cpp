#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(OnLoginButtonPressed()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnLoginButtonPressed()
{
    QString username =  ui->usernameLineEdit->text();
    // Checking if "Username" field is empty
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
            notepadWindow->show();
            break;
        }
        }
    }
    this->destroy();
}
