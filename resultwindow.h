#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include "databasehelper.h"

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = nullptr);
    ~ResultWindow();
    void setSelected(QString selected, int month, int count);
    void setPredators(QList <QPair<QString, int>> *list);
    void setPreys(QList <QPair<QString, int>> *list);
    void predict();


private:
    Ui::ResultWindow *ui;
    QString selectedAnimal;
    int selectesMonth, startCount;
    QList <QPair<QString, int>> *predators;
    QList <QPair<QString, int>> *preys;
    QString explain;
};

#endif // RESULTWINDOW_H
