#ifndef NOTEPADWINDOW_H
#define NOTEPADWINDOW_H

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

#include "GenericRequestMessage.h"
#include "JsonRequestMessageGenerator.h"
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
    void belongsTo(QString username);
    QString getUsername();
    void check();
    bool eventFilter(QObject *object, QEvent *event);
    bool okToContinue();
    void closeEvent(QCloseEvent *event);

public slots:
    void handleReceiveFileFromPartner(QString content);
    void handleReceivePeerUsername(QString peerUsername);

private slots:
     void openFile();
     void saveFile();
     void logout();
     void OnPairButtonPressed();
     void OnUnpairButtonPressed();

private:
    Ui::NotepadWindow *ui;
    QMenu *fileMenu;
    QAction *openAction;
    QAction *saveAction;
    QAction *logoutAction;
    QString username;
};

#endif // NOTEPADWINDOW_H
