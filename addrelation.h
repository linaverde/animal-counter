#ifndef ADDRELATION_H
#define ADDRELATION_H

#include <QWidget>
#include <QErrorMessage>
#include "databasehelper.h"

namespace Ui {
class AddRelation;
}

class AddRelation : public QWidget
{
    Q_OBJECT

public:
    explicit AddRelation(QWidget *parent = nullptr);
    ~AddRelation();

signals:
    void successSave();

private:
    Ui::AddRelation *ui;
    QList <QPair<QString, QString>> *predators;
    QString selectedArea;
    void saveRelation(QString predator, QString prey);

private Q_SLOTS:
    void closeClicked();
    void saveClicked();
    void areaChanged(int index);
    void predatorChanged(int index);
};

#endif // ADDRELATION_H
