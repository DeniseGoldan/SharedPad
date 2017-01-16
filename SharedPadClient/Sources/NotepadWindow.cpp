#include "NotepadWindow.h"
#include "ui_NotepadWindow.h"

auto pair_logger = spd::stdout_color_mt("pair_logger");
auto checkNews_logger = spd::stdout_color_mt("checkNews_logger");
auto handleReceiveNews_logger = spd::stdout_color_mt("handleReceiveNews_logger");
auto typing_logger = spd::stdout_color_mt("typing_logger");

NotepadWindow::NotepadWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NotepadWindow)
{
    ui->setupUi(this);
    ui->textEdit->installEventFilter(this);
    configureButtons();
    configureFileMenuActions();
    configureFileMenu();
}

NotepadWindow::~NotepadWindow()
{
    delete ui;
}

QString NotepadWindow::getUsername()
{
    return username;
}

void NotepadWindow::setUsername(QString username)
{
    this->username = username;
    ui->usernameTag->setText(username);
    check();
}

void NotepadWindow::configureButtons()
{
    connect(ui->pairButton, SIGNAL(clicked()), this, SLOT(onPairButtonPressed()));
    connect(ui->unpairButton, SIGNAL(clicked()), this, SLOT (onUnpairButtonPressed()));
}

void NotepadWindow::configureFileMenuActions()
{
    openAction = new QAction(tr("&Open file"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file from your computer"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAction = new QAction(tr("&Save file"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the document to disk"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    logoutAction = new QAction(tr("&Logout"), this);
    logoutAction->setStatusTip(tr("Logout and exit the application"));
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(logout()));
}

void NotepadWindow::configureFileMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(logoutAction);
}

void NotepadWindow::closeEvent(QCloseEvent *event)
{
    if (okToClose())
    {
        GenericRequest* logoutRequest = SpecializedRequest::getLogoutRequest(username.toStdString());
        string jsonLogoutRequest = JsonRequestGenerator::getJsonLogRequest(*logoutRequest);
        Client::sendRequest(jsonLogoutRequest);
        exit(EXIT_SUCCESS);
    }
    event->ignore();
}

bool NotepadWindow::okToClose()
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

bool NotepadWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->textEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->matches(QKeySequence::SelectAll)
                || keyEvent->matches(QKeySequence::Copy)
                || keyEvent->matches(QKeySequence::Paste))
        {
            return true;
        }
        else
        {
            if (keyEvent->text().length() != 0)
            {
                typing_logger->warn(keyEvent->text().toStdString().c_str());
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
        GenericResponse * responseFromServer = Client::pair(sender.toStdString(), peerUsername.toStdString());
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
            QMessageBox::information(this,"Approved!","You have a pair.");
            ui->peerUsernameTag->setText(peerUsername);
            break;
        }
        case INVITED_YOURSELF_CODE :
        {
            QMessageBox::information(this,"Not approved!","You can't invite yourself.");
            break;
        }
        case ALREADY_PAIRED_CODE :
        {
            QMessageBox::information(this,"Not approved!","You already have a pair.");
            break;
        }
        case USER_NOT_LOGGED_IN_CODE :
        {
            QMessageBox::information(this,"Not approved!","The receiver is not logged in.");
            break;
        }
        default:
        {
            QMessageBox::information(this,"Not approved!","Server crashed.");
            break;
        }
        }
    }
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
    GenericRequest * unpairRequest = SpecializedRequest::getUnpairRequest(username.toStdString());
    string jsonLogoutRequest = JsonRequestGenerator::getJsonLogRequest(*unpairRequest);
    Client::sendRequest(jsonLogoutRequest);
    ui->peerUsernameTag->setText("");
}

/**
 * Each notepad has a worker that constantly asks the server for updates. These updates can be either:
 * - a "server has crashed" update
 * - a "you have news from your peer" update
 * - a "you have a collaborator (or peer)" update
 * - a "you do not have a collaborator" update
 * The worker sends a signal, letting its notepad know about the update, and letting it handle the update,
 * using one of the handle functions available.
**/
void NotepadWindow::check()
{
    thread = new QThread;
    NewsChecker * worker = new NewsChecker(username.toStdString());

    QObject::connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(started()), worker, SLOT(checkInbox()));

    QObject::connect(worker, SIGNAL(receiveNewsFromPeer(QString)), this, SLOT(handleReceiveNewsFromPeer(QString)));
    QObject::connect(worker, SIGNAL(receivePeerUsername(QString)), this, SLOT(handleReceivePeerUsername(QString)));
    QObject::connect(worker, SIGNAL(serverCrashed()), this, SLOT(handleServerCrashed()));

    worker->moveToThread(thread);
    thread->start();
}

void NotepadWindow::handleReceiveNewsFromPeer(QString content)
{
    ui->textEdit->setText(content);
}

void NotepadWindow::handleReceivePeerUsername(QString peerUsername)
{
    ui->peerUsernameTag->setText(peerUsername);
}

void NotepadWindow::handleServerCrashed()
{
    ui->peerUsernameTag->setText("...you can't choose a pair");
    thread->terminate();
    if (announced == false)
    {
        announced = true;
        QMessageBox msgBox(QMessageBox::Question,
                           tr("Server crashed"),
                           "How do you wish to proceed?",
                           QMessageBox::Yes | QMessageBox::No);

        msgBox.setButtonText(QMessageBox::Yes, tr("Continue editing"));
        msgBox.setButtonText(QMessageBox::No, tr("Exit application"));
        if(msgBox.exec() == QMessageBox::Yes)
        {
            return;
        }
        exit(EXIT_SUCCESS);
    }
}

/**
 * File menu actions
**/
void NotepadWindow::openFile()
{
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

void NotepadWindow::saveFile()
{
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

void NotepadWindow::logout()
{
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
    GenericRequest *logoutRequest = SpecializedRequest::getLogoutRequest(username.toStdString());
    string jsonLogoutRequest = JsonRequestGenerator::getJsonLogRequest(*logoutRequest);
    Client::sendRequest(jsonLogoutRequest);

    this->hide();
    exit(EXIT_SUCCESS);
}
