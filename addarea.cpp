#include "addarea.h"
#include "ui_addarea.h"
#include "customvalidator.h"
#include <QErrorMessage>

AddArea::AddArea(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddArea)
{
    ui->setupUi(this);
    ui->latitudeCombo->addItem("N");
    ui->latitudeCombo->addItem("S");
    ui->longitudeCombo->addItem("E");
    ui->longitudeCombo->addItem("W");
    CustomDoubleValidator *latitudeValidator = new CustomDoubleValidator(0, 90, 2, this);
    latitudeValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->latitudeEdit->setValidator(latitudeValidator);
    CustomDoubleValidator *longitudeValidator = new CustomDoubleValidator(0, 180.0, 2, this);
    longitudeValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->longitudeEdit->setValidator(longitudeValidator);
    ui->arealNameEdit->setMaxLength(255);
    ui->radiusEdit->setValidator(new CustomIntValidator(1, 100, this));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(closeClicked()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

}

AddArea::~AddArea()
{
    delete ui;
}

void AddArea::closeClicked(){
    this->clearData();
    this->close();
}

void AddArea::clearData(){
    ui->arealNameEdit->setText("");
    ui->latitudeEdit->setText("");
    ui->longitudeEdit->setText("");
    ui->latitudeCombo->setCurrentIndex(0);
    ui->longitudeCombo->setCurrentIndex(0);
    ui->radiusEdit->setText("");
}

void AddArea::saveClicked(){
    if (ui->arealNameEdit->text() == ""){
        (new QErrorMessage(this))->showMessage("Заполните название ареала!");
    } else
        if (ui->latitudeEdit->text() == ""){
            (new QErrorMessage(this))->showMessage("Заполните градусную меру широты!");
        } else
            if (ui->longitudeEdit->text() == ""){
                (new QErrorMessage(this))->showMessage("Заполните градусную меру долготы!");

            } else
                if (ui->radiusEdit->text() == "") {
                    (new QErrorMessage(this))->showMessage("Заполните радиус ареала!");
                } else {
                    saveArea(ui->arealNameEdit->text(), ui->latitudeCombo->itemText(ui->latitudeCombo->currentIndex()), ui->latitudeEdit->text(),
                             ui->longitudeCombo->itemText(ui->longitudeCombo->currentIndex()), ui->longitudeEdit->text(), ui->radiusEdit->text());
                }
}

void AddArea::saveArea(QString name, QString latitude, QString latitudeValue, QString longitude, QString longitudeValue, QString radius){
    DataBase *db = new DataBase();
    latitudeValue = latitudeValue.replace(',', '.');
    longitudeValue = longitudeValue.replace(',', '.');
    if (db->isExist(TABLE_AREA, AREAL_NAME, name)){
        (new QErrorMessage(this))->showMessage("Ареал с таким названием уже существует!");
    } else {
        QSqlQuery insertQuery;
        if (insertQuery.exec("INSERT INTO " TABLE_AREA " (" AREAL_NAME ", " AREAL_LATITUDE ", " AREAL_LATITUDE_VALUE ", " AREAL_LONGITUDE ", " AREAL_LONGITUDE_VALUE ", " AREAL_RADIUS ")"
                          " VALUES ('" + name + "', '" + latitude + "', " + latitudeValue + ", '" + longitude + "', " + longitudeValue + ", " + radius + ")")){
            emit successSave();
            this->clearData();
            this->close();
        } else {
            (new QErrorMessage(this))->showMessage("Ошибка выполнения запроса");
        }
    }
}
