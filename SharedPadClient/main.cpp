#include "mainwindow.h"
#include "LoginWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow* mainWindow = new MainWindow();
    LoginWindow* loginWindow = new LoginWindow(mainWindow);
    loginWindow->show();
    mainWindow->show();

    //Functia de login a clasei client se observa in server
    /*
    Client * clientAna = new Client();
    clientAna->login("ana");
    */
    return a.exec();
}
