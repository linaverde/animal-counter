#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include "knowlegeeditorwindow.h"
#include "predictwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StartWindow; }
QT_END_NAMESPACE

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private:
    Ui::StartWindow *ui;
    KnowlegeEditorWindow *knowlegeEditor;
    PredictWindow *predictWindow;


private Q_SLOTS:
    void knowlegeEditClicked();
    void predictClicked();

};
#endif // STARTWINDOW_H
