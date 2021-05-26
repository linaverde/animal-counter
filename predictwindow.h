#ifndef PREDICTWINDOW_H
#define PREDICTWINDOW_H

#include <QWidget>
#include "databasehelper.h"
#include <QLineEdit>
#include <QIntValidator>
#include <QLabel>
#include "resultwindow.h"
#include <QErrorMessage>

namespace Ui {
class PredictWindow;
}

class PredictWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PredictWindow(QWidget *parent = nullptr);
    ~PredictWindow();

private:
    Ui::PredictWindow *ui;
    QString selectedArea, selectedAnimal;
    QList <QPair<QString, QLineEdit*>> *predators, *preys;
    QIntValidator *countValidator;
    QLabel *labelPredator, *labelPrey;
    ResultWindow *result;
private Q_SLOTS:
    void areaChanged(int index);
    void animalSelected(int index);
    void predictClicked();
};

#endif // PREDICTWINDOW_H
