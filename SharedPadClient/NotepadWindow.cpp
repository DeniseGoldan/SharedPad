#include "NotepadWindow.h"
#include "NewsChecker.h"
#include "ui_NotepadWindow.h"

auto pair_logger = spd::stdout_color_mt("pair_logger");
auto checkNews_logger = spd::stdout_color_mt("checkNews_logger");
auto handleReceiverFile_logger = spd::stdout_color_mt("handleReceiverFile_logger");
auto sync_logger = spd::stdout_color_mt("sync_logger");

NotepadWindow::NotepadWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NotepadWindow)
{
    ui->setupUi(this);
    ui->textEdit->installEventFilter(this);

    connect(ui->pairButton, SIGNAL(clicked()), this, SLOT(onPairButtonPressed()));
    connect(ui->unpairButton, SIGNAL(clicked()), this, SLOT (onUnpairButtonPressed()));

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

bool NotepadWindow::okToContinue()
{
    QMessageBox confirm;
    confirm.setText(tr("Are you sure you want to exit the application?"));
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirm.setDefaultButton(QMessageBox::No);

    if(confirm.exec() == QMessageBox::No)
    {
        return false;
    }
    return true;
}

void NotepadWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue())
    {
        GenericRequest logoutRequest;
        logoutRequest.setCommand(LOGOUT);
        logoutRequest.setUsername(username.toStdString());
        string jsonLogoutRequest = JsonRequestGenerator::getJsonLogRequest(logoutRequest);
        Client::sendRequest(jsonLogoutRequest);
        exit(EXIT_SUCCESS);
    }
    event->ignore();
}

bool NotepadWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->textEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->matches(QKeySequence::SelectAll) || keyEvent->matches(QKeySequence::Copy) || keyEvent->matches(QKeySequence::Paste))
        {
            return true;
        }
        else
        {
            if (keyEvent->text().length() != 0)
            {
                sync_logger->warn(keyEvent->text().toStdString().c_str());
                Client::sendNews(username.toStdString(), ui->textEdit->toPlainText().toStdString());
                return QWidget::eventFilter(object,event);
            }
        }
    }
    return false;
}

void NotepadWindow::onPairButtonPressed()
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
        GenericResponse * responseFromServer = client->pair(sender.toStdString(), peerUsername.toStdString());
        pair_logger->warn(responseFromServer->getCode());
        pair_logger->warn(responseFromServer->getCodeDescription());

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

    //case CONNECTION_FAILED_CODE:
    //{
      //  QMessageBox::critical(this,"Error","Server failed.");
       // exit(EXIT_FAILURE);
    //}
}

void NotepadWindow::onUnpairButtonPressed()
{
    QMessageBox confirm;
    confirm.setText(tr("You are going to unpair from your collaborator. Do you wish to proceed?"));
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirm.setDefaultButton(QMessageBox::No);

    if(confirm.exec() == QMessageBox::No)
    {
        return;
    }

    GenericRequest unpairRequest;
    unpairRequest.setCommand(UNPAIR);
    unpairRequest.setUsername(username.toStdString());

    string jsonLogoutRequest = JsonRequestGenerator::getJsonLogRequest(unpairRequest);
    Client::sendRequest(jsonLogoutRequest);

    ui->peerUsernameTag->setText("");
}

void NotepadWindow::check()
{
    QThread *thread = new QThread;
    NewsChecker * worker = new NewsChecker(username.toStdString());

    QObject::connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(started()), worker, SLOT(checkInbox()));

    QObject::connect(worker, SIGNAL(receiveNewsFromPeer(QString)), this, SLOT(handleReceiveNewsFromPeer(QString)));
    QObject::connect(worker, SIGNAL(receivePeerUsername(QString)), this, SLOT(handleReceivePeerUsername(QString)));

    worker->moveToThread(thread);
    thread->start();
}

void NotepadWindow::handleReceiveNewsFromPeer(QString content)
{
    //handleReceiverFile_logger->warn(content.toStdString());
    ui->textEdit->setText(content);
}

void NotepadWindow::handleReceivePeerUsername(QString peerUsername)
{
    //handleReceiverFile_logger->warn(content.toStdString());
    ui->peerUsernameTag->setText(peerUsername);
}

void NotepadWindow::openFile(){

    if (ui->textEdit->toPlainText() != nullptr)
    {
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

    GenericRequest logoutRequest;
    logoutRequest.setCommand(LOGOUT);
    logoutRequest.setUsername(username.toStdString());

    string jsonLogoutRequest = JsonRequestGenerator::getJsonLogRequest(logoutRequest);
    Client::sendRequest(jsonLogoutRequest);

    this->hide();
    exit(EXIT_SUCCESS);
}

void NotepadWindow::setUsername(QString username){
    this->username = username;
    ui->usernameTag->setText(username);
    check();
}

QString NotepadWindow::getUsername(){
    return username;
}

NotepadWindow::~NotepadWindow()
{
    delete ui;
}
