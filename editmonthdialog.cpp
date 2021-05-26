#include "editmonthdialog.h"
#include "ui_editmonthdialog.h"

editMonthDialog::editMonthDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editMonthDialog)
{
    ui->setupUi(this);
    this->tableName = "";
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
}

editMonthDialog::~editMonthDialog()
{
    delete ui;
}

void editMonthDialog::setTable(QString tableName, QString animalName){
    this->tableName = tableName;
    this->animalName = animalName;
    if (tableName == TABLE_ANIMAL_ACT_MONTH){
        animalKey = ACT_ANIMAL;
        monthKey = ACT_MONTH;
        ui->label->setText("Выберите месяцы активности для " + animalName);
    } else if (tableName == TABLE_ANIMAL_REP_MONTH){
        animalKey = REP_ANIMAL;
        monthKey = REP_MONTH;
        ui->label->setText("Выберите месяцы размножения для " + animalName);
    } else if (tableName == TABLE_ANIMAL_HUMAN_MONTH){
        animalKey = HUMAN_ANIMAL;
        monthKey = HUMAN_MONTH;
        ui->label->setText("Выберите месяцы влияния для " + animalName);
    }
    this->createUi();

}

void editMonthDialog::createUi(){
    QList<QCheckBox *> allBox = this->findChildren<QCheckBox *>();
    QSqlQuery query;
    query.exec("SELECT * FROM " + tableName + " WHERE " + animalKey + " = '" + animalName + "'");
    while (query.next()) {
        for (int i = 0; i < allBox.length(); i++){
            if (allBox.at(i)->text() == query.value(1).toString()){
                allBox.at(i)->setChecked(true);
            }
        }
    }
}

void editMonthDialog::closeWindow(){
    this->close();
}

void editMonthDialog::saveClicked(){
    QSqlQuery deleteQuery, insertQuery;
    qDebug() << "Delete part";
    if (!deleteQuery.exec("DELETE FROM " + tableName + " WHERE "  + animalKey + " = '" + animalName + "'")){
        qDebug() << deleteQuery.lastError();
        return;
    } else {
        qDebug() << "Insert part";
        QList<QCheckBox *> allBox = this->findChildren<QCheckBox *>();
        for (int i = 0; i < allBox.length(); i++){
            if (allBox.at(i)->isChecked()){
                if (!insertQuery.exec("INSERT INTO " + tableName + " (" + animalKey +", " + monthKey + ") VALUES ('" + animalName + "', " + allBox.at(i)->text()+" )")){
                    qDebug() << insertQuery.lastError();
                }
            }
        }
        emit updateMonth();
    }
    this->close();
}
