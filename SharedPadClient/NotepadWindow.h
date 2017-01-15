#ifndef CLIENT_NOTEPADWINDOW_H
#define CLIENT_NOTEPADWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QTime>
#include <QEvent>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QTextStream>
#include <QThread>
#include <string.h>

#include "GenericRequest.h"
#include "JsonRequestGenerator.h"
#include "StatusCodesAndDescriptions.h"
#include "Client.h"

class QPushButton;
class QLabel;
class QLineEdit;
class QTextEdit;

using namespace std;

namespace Ui {
class NotepadWindow;
}

class NotepadWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NotepadWindow(QWidget *parent = 0);
    ~NotepadWindow();
    QString getUsername();
    void setUsername(QString username);
    void check();
    bool eventFilter(QObject *object, QEvent *event);
    bool okToContinue();
    void closeEvent(QCloseEvent *event);

public slots:
    void handleReceiveNewsFromPeer(QString news);
    void handleReceivePeerUsername(QString peerUsername);

private slots:
     void openFile();
     void saveFile();
     void logout();
     void onPairButtonPressed();
     void onUnpairButtonPressed();

private:
    QString username;
    Ui::NotepadWindow *ui;
    QMenu *fileMenu;
    QAction *openAction;
    QAction *saveAction;
    QAction *logoutAction;
};

#endif // CLIENT_NOTEPADWINDOW_H
