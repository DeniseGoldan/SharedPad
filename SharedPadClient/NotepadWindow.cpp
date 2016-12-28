#include "NotepadWindow.h"
#include "ui_NotepadWindow.h"

NotepadWindow::NotepadWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotepadWindow)
{
    ui->setupUi(this);
}

NotepadWindow::~NotepadWindow()
{
    delete ui;
}
