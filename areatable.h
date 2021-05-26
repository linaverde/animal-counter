#ifndef AREALTABLE_H
#define AREALTABLE_H

#include <QWidget>
#include "databasehelper.h"
#include <QSqlTableModel>
#include "addarea.h"
#include "editdeletedialog.h"
#include <QErrorMessage>
#include <QMessageBox>

namespace Ui {
class AreaTable;
}

class AreaTable : public QWidget
{
    Q_OBJECT

public:
    explicit AreaTable(QWidget *parent = nullptr);
    ~AreaTable();

private:
    Ui::AreaTable *ui;
    DataBase        *db;
    QSqlTableModel  *model;
    AddArea *addWindow;
    EditDeleteDialog *dialog;
    QString selectedArea;
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();

private Q_SLOTS:
    void rowClicked(const QModelIndex &index);
    void addClicked();
    void deleteRow();
    void addRow();
    void showHelp();
};

#endif // AREALTABLE_H
