#ifndef TEXTEDITORWINDOW_H
#define TEXTEDITORWINDOW_H

#include <QDialog>

namespace Ui {
class NotepadWindow;
}

class NotepadWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NotepadWindow(QWidget *parent = 0);
    ~NotepadWindow();

private:
    Ui::NotepadWindow *ui;
};

#endif // TEXTEDITORWINDOW_H
