#include "addrelation.h"
#include "ui_addrelation.h"

AddRelation::AddRelation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddRelation)
{
    ui->setupUi(this);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(closeClicked()));

    QSqlQuery areaQuery;
    areaQuery.exec("SELECT " AREAL_NAME " FROM " TABLE_AREA);
    ui->areaBox->addItem("");
    while (areaQuery.next()){
        ui->areaBox->addItem(areaQuery.value(0).toString());
    }
    ui->predatorBox->setEnabled(false);
    ui->preyBox->setEnabled(false);
    connect(ui->areaBox, SIGNAL(currentIndexChanged(int)), this, SLOT(areaChanged(int)));
    connect(ui->predatorBox, SIGNAL(currentIndexChanged(int)), this, SLOT(predatorChanged(int)));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(closeClicked()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
    predators = new QList<QPair<QString, QString>>;
}

AddRelation::~AddRelation()
{
    delete ui;
}

void AddRelation::closeClicked(){
    this->close();
}

void AddRelation::saveClicked(){
    QString predator, prey;
    predator = ui->predatorBox->itemText(ui->predatorBox->currentIndex());
    prey = ui->preyBox->itemText(ui->preyBox->currentIndex());
    if (ui->areaBox->itemText(ui->areaBox->currentIndex()) == ""){
        (new QErrorMessage(this))->showMessage("Выберите ареал");
    } else if (predator == ""){
        (new QErrorMessage(this))->showMessage("Выберите хищника");
    } else if (prey == ""){
        (new QErrorMessage(this))->showMessage("Выберите жертву");
    } else {
        QSqlQuery query;
        if(!query.exec("SELECT COUNT(*) FROM " TABLE_RELATIONS " WHERE " RELATIONS_PREDATOR  " = '" + predator + "' AND " RELATIONS_PREY " = '" + prey +"'"))
        {
            qDebug() << "Error select from table " TABLE_RELATIONS;
            qDebug() << query.lastError().text();
        } else {
            query.next();
            if (query.value(0).toInt() > 0){
                (new QErrorMessage(this))->showMessage("Такое отношение уже существует!");
            } else {
                saveRelation(predator, prey);
            }
        }
    }
}

void AddRelation::saveRelation(QString predator, QString prey){
    QSqlQuery insertQuery;
    if (insertQuery.exec("INSERT INTO " TABLE_RELATIONS " (" RELATIONS_PREDATOR ", " RELATIONS_PREY ")"
                         " VALUES ('" + predator + "', '" + prey+ "')")){
        emit successSave();
        this->close();
    } else {
        (new QErrorMessage(this))->showMessage("Ошибка выполнения запроса");
    }
}

void AddRelation::areaChanged(int index){
    qDebug() << "Area changed event";
    ui->predatorBox->setCurrentIndex(0);
    ui->preyBox->setCurrentIndex(0);
    ui->preyBox->setEnabled(false);
    if (ui->areaBox->itemText(index) == ""){
        predators->clear();
        ui->predatorBox->setEnabled(false);
    } else {
        //delete predators;
        selectedArea = ui->areaBox->itemText(index);
        QSqlQuery predQuery;
        predQuery.exec("SELECT " ANIMAL_NAME "," ANIMAL_TYPE " FROM " TABLE_ANIMAL " WHERE " ANIMAL_AREA " = '" + selectedArea +"' AND NOT " ANIMAL_TYPE " = 'Растение'");
        ui->predatorBox->setEnabled(true);
        ui->predatorBox->addItem("");
        predators->clear();
        for (int i = ui->predatorBox->count()-1; i >0; i --){
            ui->predatorBox->removeItem(i);
        }
        ui->preyBox->setEnabled(false);
        while (predQuery.next()){
            QPair<QString, QString> pair;
            pair.first = predQuery.value(0).toString();
            pair.second = predQuery.value(1).toString();
            predators->push_back(pair);
            ui->predatorBox->addItem(pair.first);
        }
    }
}

void AddRelation::predatorChanged(int index){
    qDebug() << "Predaror changed event";
    QSqlQuery preyQuery;
    for (int i = ui->preyBox->count()-1; i >=0; i --){
        ui->preyBox->removeItem(i);
    }
    if (index != 0){
        QPair<QString, QString> predator = predators->at(index-1);
        qDebug() << "Selected predator " + predator.first + ", type " + predator.second;
        if (predator.second == "Травоядное"){
            preyQuery.exec("SELECT " ANIMAL_NAME " FROM " TABLE_ANIMAL " WHERE " ANIMAL_AREA " = '" + selectedArea +"' AND " ANIMAL_TYPE " = 'Растение'");
        } else if (predator.second == "Плотоядное"){
            preyQuery.exec("SELECT " ANIMAL_NAME " FROM " TABLE_ANIMAL " WHERE " ANIMAL_AREA " = '" + selectedArea +"' AND NOT " ANIMAL_TYPE " = 'Растение'");
        }
        ui->preyBox->setEnabled(true);
        ui->preyBox->addItem("");
        while (preyQuery.next()){
            if (predator.first != preyQuery.value(0).toString())
                ui->preyBox->addItem(preyQuery.value(0).toString());
        }
    } else {
        ui->preyBox->setEnabled(false);
    }


}
