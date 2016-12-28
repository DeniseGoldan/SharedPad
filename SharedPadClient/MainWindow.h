#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "NotepadWindow.h"
#include "Client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void OnLoginButtonPressed();

private:
    Ui::MainWindow *ui;
    NotepadWindow *notepadWindow;
};

#endif // MAINWINDOW_H
