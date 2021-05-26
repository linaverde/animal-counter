#ifndef EDITMONTHDIALOG_H
#define EDITMONTHDIALOG_H

#include <QWidget>
#include "databasehelper.h"

namespace Ui {
class editMonthDialog;
}

class editMonthDialog : public QWidget
{
    Q_OBJECT

public:
    explicit editMonthDialog(QWidget *parent = nullptr);
    ~editMonthDialog();
    void setTable(QString tableName, QString animalName);

signals:
    void updateMonth();


private:
    Ui::editMonthDialog *ui;
    QString tableName;
    QString animalName;
    QString animalKey;
    QString monthKey;
    void createUi();

private Q_SLOTS:
    void closeWindow();
    void saveClicked();

};

#endif // EDITMONTHDIALOG_H
