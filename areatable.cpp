#include "areatable.h"
#include "ui_areatable.h"

AreaTable::AreaTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AreaTable)
{
    ui->setupUi(this);

    db = new DataBase();
    db->connectToDataBase();

    this->setupModel(TABLE_AREA,
                     QStringList() << trUtf8("Название")
                     << trUtf8("Широта")
                     << trUtf8("Градус широты")
                     << trUtf8("Долгота")
                     << trUtf8("Градус долготы")
                     << trUtf8("Радиус, км")
                     );

    /* Инициализируем внешний вид таблицы с данными
         * */
    this->createUI();
    addWindow = new AddArea();
    dialog = new EditDeleteDialog();
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(rowClicked(QModelIndex)));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(this->dialog, SIGNAL(deleteSignal()), this, SLOT(deleteRow()));
    connect(this->addWindow, SIGNAL(successSave()), this, SLOT(addRow()));
    connect(ui->helpButton, SIGNAL(clicked()), this, SLOT(showHelp()));
}

void AreaTable::showHelp(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Справка");
    msgBox.setText("Ареалы представляют собой сложыне географические объекты. Ареалы используются при описании популяции. "
                   "Каждый ареал представляет собой окружность, характеризуется герграфическими координатами центра окружности и радиусом. "
                   "Ареалы могут пересекаться между собой. Характеристики двух ареалов, за исключением уникального названия, могут совпадать. "
                   "Однако, приведем простой пример - в одном лесу птицы, живущие в верхних уровнях ареала и питающиеся плодами деревьев, никогда не пересекаются с мелкими грызунами, живущими у земли и поедающими ягоды с кустов. "
                   "Географически лес представляет собой один ареал, однако, при большом количестве отслеживаемых популяций, разделить его на два ареала будет удобно эксперту. "
                   "Учитывайте это, заполняя базу данных. ");
    msgBox.exec();
}

AreaTable::~AreaTable()
{
    delete ui;
}

void AreaTable::rowClicked(const QModelIndex &index){
    qDebug() << "Row " << index.row() << "clicked";
    selectedArea = (index.model()->data(index.model()->index(index.row(), 0)).toString());
    if (dialog->isVisible()){
        dialog->close();
    }
    dialog->show();
}

void AreaTable::deleteRow(){
    QSqlQuery query;
    DataBase *db = new DataBase;
    if (!db->isExist(TABLE_ANIMAL, ANIMAL_AREA, selectedArea)){
        if (!query.exec("DELETE FROM " TABLE_AREA  " WHERE " AREAL_NAME  "= '" + selectedArea + "'")){
            (new QErrorMessage(this))->showMessage("Ошибка выполнения запроса");
        } else {
            this->createUI();
        }
    } else {
        (new QErrorMessage(this))->showMessage("Невозможно удалить ареал, так как существуют популяции, проживающие на нем");
    }
}

void AreaTable::addRow(){
    this->createUI();
}

void AreaTable::addClicked(){
    if (addWindow->isVisible()){
        addWindow->close();
    }
    addWindow->show();
}

void AreaTable::setupModel(const QString &tableName, const QStringList &headers)
{
    model = new QSqlTableModel(this);
    model->setTable(tableName);

    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    model->setSort(0,Qt::AscendingOrder);
}

void AreaTable::createUI()
{
    ui->tableView->setModel(model);
    //ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
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
