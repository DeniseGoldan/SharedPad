#ifndef CLIENT_LOGINWINDOW_H
#define CLIENT_LOGINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "Client.h"
#include "NotepadWindow.h"
#include "HeartBeatSender.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void onLoginButtonPressed();

private:
    Ui::LoginWindow *ui;
    NotepadWindow *notepadWindow;
};

#endif // CLIENT_LOGINWINDOW_H
