#include "addanimal.h"
#include "ui_addanimal.h"

AddAnimal::AddAnimal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddAnimal)
{
    ui->setupUi(this);
    QSqlQuery query;
    if (query.exec("SELECT " AREAL_NAME " FROM " TABLE_AREA)){
        while (query.next()){
            ui->areaComboBox->addItem(query.value(0).toString());
        }
    }
    ui->typeComboBox->addItem("Растение");
    ui->typeComboBox->addItem("Травоядное");
    ui->typeComboBox->addItem("Плотоядное");
    connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));

    CustomIntValidator *infValidator = new CustomIntValidator(1, INT_MAX, this);

    ui->normEdit->setValidator(infValidator);
    ui->normEdit->setText("0");
    ui->normEdit->setEnabled(false);

    ui->calEdit->setValidator(infValidator);

    ui->femaleEdit->setValidator(new CustomIntValidator(1, 100, this));
    ui->childEdit->setValidator(infValidator);
    ui->deathEdit->setValidator(infValidator);

    ui->humanEdit->setValidator(new QIntValidator());
    ui->humanEdit->setText("0");

    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(closeClicked()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

}

AddAnimal::~AddAnimal()
{
    delete ui;
}

void AddAnimal::closeClicked(){
    this->clearData();
    this->close();
}

void AddAnimal::saveClicked(){
    if (ui->nameEdit->text() == ""){
        (new QErrorMessage(this))->showMessage("Заполните название популяции!");
    } else if (ui->normEdit->text() == ""){
        (new QErrorMessage(this))->showMessage("Заполните калорийность особей!");
    } else if (ui->calEdit->text() == ""){
        (new QErrorMessage(this))->showMessage("Заполните норму калорий!");
    } else if (ui->femaleEdit->text() == ""){
        (new QErrorMessage(this))->showMessage("Заполните процент женских особей!");
    } else if (ui->childEdit->text() == ""){
        (new QErrorMessage(this))->showMessage("Заполните количество потомства!");
    } else if (ui->deathEdit->text() == ""){
        (new QErrorMessage(this))->showMessage("Заполните смертность!");
    } else if (ui->humanEdit->text() == ""){
        (new QErrorMessage(this))->showMessage("Заполните влияние человека!");
    } else {
        saveAnimal(ui->nameEdit->text(), ui->typeComboBox->itemText(ui->typeComboBox->currentIndex()), ui->areaComboBox->itemText(ui->areaComboBox->currentIndex()),
                   ui->calEdit->text(), ui->normEdit->text(), ui->femaleEdit->text(), ui->childEdit->text(),
                   ui->deathEdit->text(), ui->humanEdit->text());
    }
}

void AddAnimal::saveAnimal(QString name, QString type, QString area, QString cal, QString norm,
                           QString female, QString child, QString death, QString human){
    DataBase *db = new DataBase();
    if (db->isExist(TABLE_ANIMAL, ANIMAL_NAME, name)){
        (new QErrorMessage(this))->showMessage("Популяция с таким названием уже существует!");
    } else {
        QSqlQuery insertQuery;
        if (insertQuery.exec("INSERT INTO " TABLE_ANIMAL " (" ANIMAL_NAME ", " ANIMAL_TYPE ", " ANIMAL_AREA ", " ANIMAL_CALORIES ", "
                          ANIMAL_NORM_CAL ", " ANIMAL_FEMALE ", " ANIMAL_CHILDREN ", " ANIMAL_DEATH ", " ANIMAL_HUMAN ")"
                          " VALUES ('" + name + "', '" + type + "', '" + area + "', "
                          + cal + ", " + norm + ", " + female + ", " + child + ", " + death + ", " + human+ ")")){
            emit successSave();
            this->clearData();
            this->close();
        } else {
            (new QErrorMessage(this))->showMessage("Ошибка выполнения запроса");
        }
    }

}


void AddAnimal::typeChanged(int index){
    if (ui->typeComboBox->itemText(index) == "Растение"){
        ui->normEdit->setText("0");
        ui->normEdit->setEnabled(false);
    } else {
        ui->normEdit->setEnabled(true);
    }
}

void AddAnimal::clearData(){
    ui->typeComboBox->setCurrentIndex(0);
    ui->normEdit->setText("0");
    ui->normEdit->setEnabled(false);
    ui->areaComboBox->setCurrentIndex(0);
    ui->nameEdit->setText("");
    ui->calEdit->setText("");
    ui->femaleEdit->setText("");
    ui->childEdit->setText("");
    ui->deathEdit->setText("");
    ui->humanEdit->setText("0");
}
