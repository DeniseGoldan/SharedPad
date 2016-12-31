#ifndef NOTEPADWINDOW_H
#define NOTEPADWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <string.h>

#include "TextFileEditor.h"
#include "GenericRequestMessage.h"
#include "JsonRequestMessageGenerator.h"
#include "StatusCodesAndDescriptions.h"
#include "Mediator.h"

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

    //TextFileEditor * textFileEditor;

private slots:
     void openFile();
     void saveFile();
     void logout();

private:
    Ui::NotepadWindow *ui;
    QMenu *fileMenu;
    QAction *openAction;
    QAction *saveAction;
    QAction *logoutAction;
    QString username;
};

#endif // NOTEPADWINDOW_H
