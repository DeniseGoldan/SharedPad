#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "Mediator.h"
#include "NotepadWindow.h"
#include "QuerySender.h"

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
    void OnLoginButtonPressed();

private:
    Ui::LoginWindow *ui;
    NotepadWindow *notepadWindow;
};

#endif // LOGINWINDOW_H
