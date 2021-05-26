#ifndef ADDAREA_H
#define ADDAREA_H

#include <QDialog>
#include <QMessageBox>
#include "databasehelper.h"

namespace Ui {
class AddArea;
}

class AddArea : public QDialog
{
    Q_OBJECT

public:
    explicit AddArea(QWidget *parent = nullptr);
    ~AddArea();

signals:
    void successSave();

private:
    Ui::AddArea *ui;
    void saveArea(QString name, QString latitude, QString latitudeValue, QString longitude, QString longitudeValue, QString radius);
    void clearData();

private Q_SLOTS:
    void closeClicked();
    void saveClicked();
};

#endif // ADDAREA_H
