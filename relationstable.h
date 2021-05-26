#ifndef RELATIONSTABLE_H
#define RELATIONSTABLE_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include "databasehelper.h"
#include "addrelation.h"
#include "editdeletedialog.h"
#include <QMessageBox>

namespace Ui {
class RelationsTable;
}

class RelationsTable : public QWidget
{
    Q_OBJECT

public:
    explicit RelationsTable(QWidget *parent = nullptr);
    ~RelationsTable();

private:
    Ui::RelationsTable *ui;
    QSqlRelationalTableModel  *model;
    AddRelation *addRelation;
    EditDeleteDialog *dialog;
    QModelIndex selectedRelation;
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();

public Q_SLOTS:
    void rowClicked(const QModelIndex &index);
    void addClicked();
    void deleteRow();
    void addRow();
    void showHelp();
};

#endif // RELATIONSTABLE_H
