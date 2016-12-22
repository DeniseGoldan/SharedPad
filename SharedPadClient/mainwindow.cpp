#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Client.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Client * clientAna = new Client();
    clientAna->login("ana");
}

MainWindow::~MainWindow()
{
    delete ui;
}
