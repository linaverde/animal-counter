#ifndef ADDANIMAL_H
#define ADDANIMAL_H

#include <QWidget>
#include "databasehelper.h"
#include "customvalidator.h"
#include <QErrorMessage>

namespace Ui {
class AddAnimal;
}

class AddAnimal : public QWidget
{
    Q_OBJECT

public:
    explicit AddAnimal(QWidget *parent = nullptr);
    ~AddAnimal();

signals:
    void successSave();

private:
    Ui::AddAnimal *ui;
    void saveAnimal(QString name, QString type, QString area, QString cal, QString norm,
                    QString female, QString child, QString death, QString human);
    void clearData();

private Q_SLOTS:
    void closeClicked();
    void saveClicked();
    void typeChanged(int index);
};

#endif // ADDANIMAL_H
