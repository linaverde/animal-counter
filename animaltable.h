#ifndef ANIMALTABLE_H
#define ANIMALTABLE_H

#include <QWidget>
#include "databasehelper.h"
#include <QSqlRelationalTableModel>
#include "editanimaldialog.h"
#include "addanimal.h"
#include <QMessageBox>
#include <editmonthdialog.h>

namespace Ui {
class AnimalTable;
}

class AnimalTable : public QWidget
{
    Q_OBJECT

public:
    explicit AnimalTable(QWidget *parent = nullptr);
    ~AnimalTable();

private:
    Ui::AnimalTable *ui;
    DataBase        *db;
    QSqlRelationalTableModel  *model;
    QSqlTableModel *activityModel, *repModel, *humanModel;
    AddAnimal *addWindow;
    EditAnimalDialog *dialog;
    QString selectedAnimal;
    QModelIndex selectedIndex;
    editMonthDialog *monthDialog;
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();

public Q_SLOTS:
    void rowSelected(const QModelIndex &index);
    void rowClicked(const QModelIndex &index);
    void addClicked();
    void deleteRow();
    void addRow();
    void showHelp();
    void setSelected();
};

#endif // ANIMALTABLE_H
