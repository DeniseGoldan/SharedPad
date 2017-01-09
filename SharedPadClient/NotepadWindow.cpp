#include "NotepadWindow.h"
#include "ui_NotepadWindow.h"

auto pair_logger = spd::stdout_color_mt("pair_logger");

NotepadWindow::NotepadWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NotepadWindow)
{
    ui->setupUi(this);

    connect(ui->pairButton, SIGNAL(clicked()), this, SLOT(OnPairButtonPressed()));

    openAction = new QAction(tr("&Open file"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAction = new QAction(tr("&Save file"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the document to disk"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    logoutAction = new QAction(tr("&Logout"), this);
    logoutAction->setStatusTip(tr("Logout and exit the application"));
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(logout()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(logoutAction);

}

void NotepadWindow::OnPairButtonPressed()
{
    QString sender = ui->usernameTag->text();
    QString peerUsername =  ui->peerSelectLineEdit->text();
    pair_logger->warn(sender.toStdString());
    pair_logger->warn(peerUsername.toStdString());

    if (peerUsername.isEmpty())
    {
        QMessageBox::information(this, tr("Error message"), "Peer username can't be blank!");
    }
    else
    {
        Client * client = new Client();
        GenericResponseMessage * responseFromServer = client->pair(sender.toStdString(), peerUsername.toStdString());
        pair_logger->warn(responseFromServer->getCode());
        pair_logger->warn(responseFromServer->getCodeDescription());
        pair_logger->warn(responseFromServer->getSender());
        pair_logger->warn(responseFromServer->getReceiver());

        string serverResponsePeer = responseFromServer->getReceiver();

        if (!serverResponsePeer.empty())
        {
            pair_logger->warn(responseFromServer->getReceiver());
            ui->peerUsernameTag->setText(serverResponsePeer.c_str());
        }

        switch(responseFromServer->getCode())
        {
            case PAIR_ADDED_CODE :
            {
                QMessageBox::information(this,"Pair approved!","You have a pair.");
                ui->peerUsernameTag->setText(peerUsername);
                break;
            }
            default:
            {
                QMessageBox::information(this,"Pair not approved!","Sorry...");
                break;
            }
        }
    }
}


void NotepadWindow::openFile(){

    if (ui->textEdit->toPlainText() != nullptr){
        QMessageBox confirm;
        confirm.setText(tr("Opening a new file means discarding the editing done on the current file. Do you wish to proceed?"));
        confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        confirm.setDefaultButton(QMessageBox::No);

        if(confirm.exec() == QMessageBox::No)
        {
            return;
        }
    }

    // Remove characters from textEdit
    ui->textEdit->clear();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "/home", tr("Text Files (*.txt)"));

    if(!fileName.isEmpty())
    {

        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), file.errorString());
            return;
        }
        QTextStream stream(&file);
        ui->textEdit->setText(stream.readAll());
        file.close();
    }
    else
    {
        QMessageBox confirm;
        confirm.setText(tr("No file has been selected."));
        confirm.setStandardButtons(QMessageBox::Ok | QMessageBox::Retry);
        confirm.setDefaultButton(QMessageBox::Retry);

        if(confirm.exec() == QMessageBox::Retry)
        {
            openFile();
        }
    }
}

void NotepadWindow::saveFile(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), QString(), tr("Text Files (*.txt)"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this, tr("Error"),file.errorString());
            return;
        }
        else
        {
            QTextStream stream(&file);
            stream << ui->textEdit->toPlainText();
            stream.flush();
            file.close();
            QMessageBox::information(this, "Success","The file has been saved.");
        }
    }
}

void NotepadWindow::logout(){

    if (ui->textEdit->toPlainText() != nullptr)
    {
        QMessageBox confirm;
        confirm.setText(tr("Logging out would mean discarding the editing done. Make sure to save your file before logging out. Do you wish to proceed?"));
        confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        confirm.setDefaultButton(QMessageBox::No);

        if(confirm.exec() == QMessageBox::No)
        {
            return;
        }
    }

    GenericRequestMessage logoutRequest;
    logoutRequest.setCommand(LOGOUT);
    logoutRequest.setUsername(username.toStdString());

    string jsonLogoutRequest = JsonRequestMessageGenerator::getJsonLogRequestMessage(logoutRequest);
    Client::sendRequestToServer(jsonLogoutRequest);

    this->hide();
    exit(EXIT_SUCCESS);
}

void NotepadWindow::belongsTo(QString username){
    this->username = username;
    ui->usernameTag->setText(username);
}

QString NotepadWindow::getUsername(){
    return username;
}

NotepadWindow::~NotepadWindow()
{
    delete ui;
}
