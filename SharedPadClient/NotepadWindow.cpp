#include "NotepadWindow.h"
#include "ui_NotepadWindow.h"

NotepadWindow::NotepadWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NotepadWindow)
{
    ui->setupUi(this);

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
