#ifndef EDITANIMALDIALOG_H
#define EDITANIMALDIALOG_H

#include <QWidget>
#include "editmonthdialog.h"

namespace Ui {
class EditAnimalDialog;
}

class EditAnimalDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EditAnimalDialog(QWidget *parent = nullptr);
    ~EditAnimalDialog();

private:
    Ui::EditAnimalDialog *ui;
    QString animalName;
    editMonthDialog *editMonth;

public:
    void setAnimal(QString animalName);
    void setMonthDialod(editMonthDialog *dialog);

signals:
    void deleteSignal();

private Q_SLOTS:
    void closeWindow();
    void deleteClicked();
    void editActMonthClicked();
    void editRepMonthClicked();
    void editHumanMonthClicked();

};

#endif // EDITANIMALDIALOG_H
