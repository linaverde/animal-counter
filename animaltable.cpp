#include "animaltable.h"
#include "ui_animaltable.h"

AnimalTable::AnimalTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnimalTable)
{
    ui->setupUi(this);
    this->setupModel(TABLE_ANIMAL,
                     QStringList() << trUtf8("Название")
                     << trUtf8("Тип")
                     << trUtf8("Ареал")
                     << trUtf8("Суточная норма, кал")
                     << trUtf8("Калорийность")
                     << trUtf8("Жен. особи, %")
                     << trUtf8("Ср. кол-во потомства")
                     << trUtf8("Ср. смертность")
                     << trUtf8("Влияние человека, особи")
                     );
    this->createUI();
    this->dialog = new EditAnimalDialog();
    this->addWindow = new AddAnimal();
    activityModel = new QSqlRelationalTableModel(this);
    repModel = new QSqlRelationalTableModel(this);
    humanModel = new QSqlRelationalTableModel(this);
    monthDialog = new editMonthDialog();
    dialog->setMonthDialod(monthDialog);
    connect(ui->animalTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(rowSelected(QModelIndex)));
    connect(ui->animalTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(rowClicked(QModelIndex)));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(this->dialog, SIGNAL(deleteSignal()), this, SLOT(deleteRow()));
    connect(this->addWindow, SIGNAL(successSave()), this, SLOT(addRow()));
    connect(ui->helpButton, SIGNAL(clicked()), this, SLOT(showHelp()));
    connect(monthDialog, SIGNAL(updateMonth()), this, SLOT(setSelected()));
}

void AnimalTable::setSelected(){
    rowSelected(selectedIndex);
}

void AnimalTable::showHelp(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Справка");
    msgBox.setText("Популяции животных или растений представляют собой группы, обособленные от других существ того же вида. "
                   "Не смотря на то, что особи принадлежат одному виду, популяции могут различаться между собой. "
                   "В каждой популяции одного вида могут отличаться месяцы активности, рацион особей. Это все зависит от условий, в которых живет популяция. "
                   "Название популяции - уникальное название, по которому эксперты ПО различают популяции между собой. "
                   "Ареал - место, где обитает популяция. "
                   "Суточная норма калорий - сколько требуется одной особи калорий в день, чтобы не умереть. "
                   "Калорийность - сколько калорий содержится в одной особи. "
                   "Процент женский особей - сколько особей от общего числа могут приносить потомство. "
                   "Среднее количество потомства - сколько в среднем новых особей появляется на свет. "
                   "Средняя смертность - какой процент популяции в месяц умирает естественной смертью. "
                   "Влияние человека - количество особей, на которое в месяц влияет человек. "
                   "Может быть как положительным числом (если популяция находится под защитой, особей выращивают и возвращают в естественную среду), "
                   "так и отрицательным (если на особей популяции открыта охота). ");
    msgBox.exec();
}

AnimalTable::~AnimalTable()
{
    delete ui;
}

void AnimalTable::deleteRow(){
    QSqlQuery query, querySelect;
    querySelect.exec("SELECT COUNT(*) FROM " TABLE_RELATIONS " WHERE " RELATIONS_PREDATOR  " = '" + selectedAnimal + "' OR " RELATIONS_PREY " = '" + selectedAnimal + "'");
    querySelect.next();
    if (querySelect.value(0).toInt() > 0){
        (new QErrorMessage(this))->showMessage("Невозможно удалить популяцию, так как существуют зависимые популяции");
    } else {
        if (!query.exec("DELETE FROM " TABLE_ANIMAL  " WHERE " ANIMAL_NAME  "= '" + selectedAnimal + "'")){
            (new QErrorMessage(this))->showMessage("Ошибка выполнения запроса");
        } else {
            query.exec("DELETE FROM " TABLE_ANIMAL_ACT_MONTH  " WHERE " ACT_ANIMAL  "= '" + selectedAnimal + "'");
            query.exec("DELETE FROM " TABLE_ANIMAL_REP_MONTH  " WHERE " REP_ANIMAL  "= '" + selectedAnimal + "'");
            query.exec("DELETE FROM " TABLE_ANIMAL_HUMAN_MONTH  " WHERE " HUMAN_ANIMAL  "= '" + selectedAnimal + "'");
            this->createUI();
        }
    }
}

void AnimalTable::addRow(){
    activityModel->clear();
    repModel->clear();
    humanModel->clear();
    this->createUI();
}

void AnimalTable::rowSelected(const QModelIndex &index){
    qDebug() << "Row selected";
    selectedAnimal = index.model()->data(index.model()->index(index.row(), 0)).toString();
    selectedIndex = index;

    activityModel->setTable(TABLE_ANIMAL_ACT_MONTH);
    activityModel->setFilter( ACT_ANIMAL " = '" + selectedAnimal + "'");
    activityModel->select();
    ui->activityTableView->setModel(activityModel);
    ui->activityTableView->setColumnHidden(0, true);
    activityModel->setHeaderData(1,Qt::Horizontal, "Месяцы активности");
    ui->activityTableView->verticalHeader()->setVisible(false);
    ui->activityTableView->resizeColumnsToContents();
    ui->activityTableView->resizeRowsToContents();
    ui->activityTableView->update();

    repModel->setTable(TABLE_ANIMAL_REP_MONTH);
    repModel->setFilter( REP_ANIMAL " = '" + selectedAnimal + "'");
    repModel->select();
    ui->repTableView->setModel(repModel);
    ui->repTableView->setColumnHidden(0, true);
    repModel->setHeaderData(1,Qt::Horizontal, "Месяцы размножения");
    ui->repTableView->verticalHeader()->setVisible(false);
    ui->repTableView->resizeColumnsToContents();
    ui->repTableView->resizeRowsToContents();
    ui->repTableView->update();

    humanModel->setTable(TABLE_ANIMAL_HUMAN_MONTH);
    humanModel->setFilter( HUMAN_ANIMAL " = '" + selectedAnimal + "'");
    humanModel->select();
    ui->humanTableView->setModel(humanModel);
    ui->humanTableView->setColumnHidden(0, true);
    humanModel->setHeaderData(1,Qt::Horizontal, "Месяцы влияния человека");
    ui->humanTableView->verticalHeader()->setVisible(false);
    ui->humanTableView->resizeColumnsToContents();
    ui->humanTableView->resizeRowsToContents();
    ui->humanTableView->update();

}

void AnimalTable::rowClicked(const QModelIndex &index){
    selectedAnimal = index.model()->data(index.model()->index(index.row(), 0)).toString();
    dialog->setAnimal(selectedAnimal);
    dialog->show();
}

void AnimalTable::addClicked(){
    if (addWindow->isVisible()){
        addWindow->close();
    }
    addWindow->show();
}


void AnimalTable::setupModel(const QString &tableName, const QStringList &headers)
{
    model = new QSqlRelationalTableModel(this);
    model->setTable(tableName);
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    model->setSort(0,Qt::AscendingOrder);
}

void AnimalTable::createUI()
{
    ui->animalTableView->setModel(model);
    //ui->animalTableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->animalTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->animalTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->animalTableView->resizeColumnsToContents();
    ui->animalTableView->resizeRowsToContents();
    ui->animalTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->animalTableView->horizontalHeader()->setStretchLastSection(true);

    model->select(); // Делаем выборку данных из таблицы
}
