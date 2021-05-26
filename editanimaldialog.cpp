#include "editanimaldialog.h"
#include "ui_editanimaldialog.h"

EditAnimalDialog::EditAnimalDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditAnimalDialog)
{
    ui->setupUi(this);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(ui->editActMonthButton, SIGNAL(clicked()), this, SLOT(editActMonthClicked()));
    connect(ui->editRepMonthButton, SIGNAL(clicked()), this, SLOT(editRepMonthClicked()));
    connect(ui->editHumanMonthButton, SIGNAL(clicked()), this, SLOT(editHumanMonthClicked()));
    connect(ui->deleteAnimalButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
}

EditAnimalDialog::~EditAnimalDialog()
{
    delete ui;
}

void EditAnimalDialog::setMonthDialod(editMonthDialog *dialog){
    editMonth = dialog;
}

void EditAnimalDialog::closeWindow(){
    this->close();
}

void EditAnimalDialog::deleteClicked(){
    emit deleteSignal();
    this->close();
}

void EditAnimalDialog::editActMonthClicked(){
    //editMonth = new editMonthDialog();
    editMonth->setTable(TABLE_ANIMAL_ACT_MONTH, this->animalName);
    editMonth->show();
    this->close();
}

void EditAnimalDialog::editRepMonthClicked(){
    //editMonth = new editMonthDialog();
    editMonth->setTable(TABLE_ANIMAL_REP_MONTH, this->animalName);
    editMonth->show();
    this->close();
}

void EditAnimalDialog::editHumanMonthClicked(){
    //editMonth = new editMonthDialog();
    editMonth->setTable(TABLE_ANIMAL_HUMAN_MONTH, this->animalName);
    editMonth->show();
    this->close();
}

void EditAnimalDialog::setAnimal(QString animalName){
    this->animalName = animalName;
    setWindowTitle("Редактирование популяции " + animalName);
}
