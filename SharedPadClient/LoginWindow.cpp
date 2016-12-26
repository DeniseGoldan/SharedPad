#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget* parent)
    : QDialog(parent)
{
    setFixedSize(240, 120);
    setWindowTitle("Login");
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout* vbox = new QVBoxLayout(this);

    QHBoxLayout* hbox1 = new QHBoxLayout();
    usernameLabel = new QLabel("Username:");
    userLineEdit = new QLineEdit();
    hbox1->addWidget(usernameLabel, 1);
    hbox1->addWidget(userLineEdit, 2);

    QHBoxLayout* hbox2 = new QHBoxLayout();
    loginButton = new QPushButton("Login");
    hbox2->addWidget(loginButton, 1, Qt::AlignRight);

    vbox->addSpacing(1);
    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);

    connect(loginButton, SIGNAL(clicked()), this, SLOT(OnLogin()));
}

LoginWindow::~LoginWindow() {}

void LoginWindow::OnLogin()
{
    QString username = userLineEdit->text();
    // Checking if "Username" filed is empty
    if (username.isEmpty())
    {
        QMessageBox::information(this, tr("Error message"), "Username can't be blank!");
    }
    else
    {
        Client * client = new Client();
        client->login(username.toStdString());
    }
    this->destroy();
}

GenericResponseMessage* LoginWindow::tryToLogin(std::string username){

    GenericRequestMessage loginRequest;
    loginRequest.setCommand(LOGIN);
    loginRequest.setUsername(username);

    std::string jsonLoginRequest = JsonRequestMessageGenerator::getJsonLoginRequestMessage(loginRequest);
    GenericResponseMessage* responseFromServer = Client::sendRequestToServer(jsonLoginRequest);
    return responseFromServer;
}

