#include "relationstable.h"
#include "ui_relationstable.h"

RelationsTable::RelationsTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RelationsTable)
{
    ui->setupUi(this);
    this->setupModel(TABLE_RELATIONS,
                     QStringList() << trUtf8("Хищник")
                     << trUtf8("Жертва"));
    this->createUI();
    dialog = new EditDeleteDialog();
    addRelation = new AddRelation();
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(rowClicked(QModelIndex)));
    connect(this->dialog, SIGNAL(deleteSignal()), this, SLOT(deleteRow()));
    connect(this->addRelation, SIGNAL(successSave()), this, SLOT(addRow()));
    connect(ui->helpButton, SIGNAL(clicked()), this, SLOT(showHelp()));
}

void RelationsTable::showHelp(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Справка");
    msgBox.setText("Популяции взаимодействуют между собой по принципу хищник-жертва. "
                   "Таким образом, при наличии отношений между двумя популяциями, одна из них обязательно будет в роли хищника, а вторая - жертвой. "
                   "Особи хищники едят особей жертв. "
                   "Существуют ограничения формирования этих отношений. "
                   "Популяции растений не могут быть хищниками. Популяции травоядных животных могут есть только растения. "
                   "Популяции хищников могут есть как травоядных животных, так и других хищников, но не могут есть сами себя. ");
    msgBox.exec();
}

RelationsTable::~RelationsTable()
{
    delete ui;
}

void RelationsTable::setupModel(const QString &tableName, const QStringList &headers)
{
    model = new QSqlRelationalTableModel(this);
    model->setTable(tableName);
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    model->setSort(0,Qt::AscendingOrder);
}

void RelationsTable::createUI()
{
    ui->tableView->setModel(model);
    //ui->animalTableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    model->select(); // Делаем выборку данных из таблицы
}

void RelationsTable::rowClicked(const QModelIndex &index){
    qDebug() << "Row " << index.row() << "clicked";
    selectedRelation = index;
    if (dialog->isVisible()){
        dialog->close();
    }
    dialog->show();
}

void RelationsTable::addClicked(){
    addRelation->show();
}

void RelationsTable::deleteRow(){
    QSqlQuery deleteQuery;
    QString predator, prey;
    predator = selectedRelation.model()->data(selectedRelation.model()->index(selectedRelation.row(), 0)).toString();
    prey = selectedRelation.model()->data(selectedRelation.model()->index(selectedRelation.row(), 1)).toString();
    if (!deleteQuery.exec("DELETE FROM " TABLE_RELATIONS  " WHERE "
                          RELATIONS_PREDATOR  "= '" + predator + "'"
                          " AND " RELATIONS_PREY "= '" + prey +"'")){
        qDebug() << deleteQuery.lastError();
    } else {
        this->createUI();
    }
}

void RelationsTable::addRow(){
    this->createUI();
}
