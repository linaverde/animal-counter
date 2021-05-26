#include "predictwindow.h"
#include "ui_predictwindow.h"

PredictWindow::PredictWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PredictWindow)
{
    ui->setupUi(this);
    QSqlQuery setupQuery;
    setupQuery.exec("SELECT * FROM " TABLE_MONTH);
    while(setupQuery.next()){
        ui->monthBox->addItem(setupQuery.value(0).toString());
    }
    ui->areaBox->addItem("");
    setupQuery.exec("SELECT " AREAL_NAME " FROM " TABLE_AREA);
    while(setupQuery.next()){
        ui->areaBox->addItem(setupQuery.value(0).toString());
    }
    ui->animalBox->setEnabled(false);
    countValidator = new QIntValidator(0, INT_MAX, this);
    ui->countEdit->setValidator(countValidator);
    connect(ui->areaBox, SIGNAL(currentIndexChanged(int)), this, SLOT(areaChanged(int)));
    predators = new QList<QPair<QString, QLineEdit*>>;
    preys = new QList<QPair<QString, QLineEdit*>>;
    connect(ui->animalBox, SIGNAL(currentIndexChanged(int)), this, SLOT(animalSelected(int)));
    connect(ui->predictButton, SIGNAL(clicked()), this, SLOT(predictClicked()));
    result = new ResultWindow();
}

PredictWindow::~PredictWindow()
{
    delete ui;
}

void PredictWindow::predictClicked(){

    if (selectedAnimal == ""){
        (new QErrorMessage(this))->showMessage("Выберите отслеживаемую популяцию!");
        return;
    }
    if (ui->countEdit->text() == ""){
        (new QErrorMessage(this))->showMessage("Введите исходных резмер отслеживаемой популяции!");
        return;
    }

    QList <QPair<QString, int>> *predatorStrings = new QList <QPair<QString, int>>;
    QList <QPair<QString, int>> *preyStrings = new QList <QPair<QString, int>>;

    for(int i = 0; i < predators->length(); i++){
        if(predators->at(i).second->text() == ""){
            (new QErrorMessage(this))->showMessage("Заполните количество особей для популяции " + predators->at(i).first);
            return;
        } else {
            QPair <QString, int> pair;
            pair.first = predators->at(i).first;
            pair.second = predators->at(i).second->text().toInt();
            predatorStrings->push_back(pair);
        }
    }

    for(int i = 0; i < preys->length(); i++){
        if(preys->at(i).second->text() == ""){
            (new QErrorMessage(this))->showMessage("Заполните количество особей для популяции " + preys->at(i).first);
            return;
        } else {
            QPair <QString, int> pair;
            pair.first = preys->at(i).first;
            pair.second = preys->at(i).second->text().toInt();
            preyStrings->push_back(pair);
        }
    }

    result->setSelected(selectedAnimal, ui->monthBox->currentText().toInt(), ui->countEdit->text().toInt());
    result->setPredators(predatorStrings);
    result->setPreys(preyStrings);
    result->predict();
    result->show();

}

void PredictWindow::areaChanged(int index){
    if (ui->areaBox->itemText(index) == ""){
        for (int i = ui->animalBox->count()-1; i >=0; i --){
            ui->animalBox->removeItem(i);
        }
        ui->animalBox->setEnabled(false);
    } else {
        selectedArea = ui->areaBox->itemText(index);
        for (int i = ui->animalBox->count()-1; i >=0; i --){
            ui->animalBox->removeItem(i);
        }
        QSqlQuery animalQuery;
        animalQuery.exec("SELECT " ANIMAL_NAME " FROM " TABLE_ANIMAL " WHERE " ANIMAL_AREA " = '" + selectedArea +"'");
        ui->animalBox->setEnabled(true);
        ui->animalBox->addItem("");
        while (animalQuery.next()){
            ui->animalBox->addItem(animalQuery.value(0).toString());
        }
    }
}


void removeLayout ( QLayout* layout )
{
    QLayoutItem* child;
    while ( layout->count() != 0 ) {
        child = layout->takeAt ( 0 );
        if ( child->layout() != 0 ) {
            removeLayout ( child->layout() );
        } else if ( child->widget() != 0 ) {
            delete child->widget();
        }
        delete child;
    }
}

void PredictWindow::animalSelected(int index){
    if (ui->animalBox->itemText(index) == ""){
        predators->clear();
        preys->clear();
        removeLayout(ui->relationsLayout);
        ui->relationsLayout->update();
    } else {
        selectedAnimal = ui->animalBox->itemText(index);
        QSqlQuery selectQuery;
        selectQuery.exec("SELECT " RELATIONS_PREDATOR " FROM " TABLE_RELATIONS " WHERE " RELATIONS_PREY " = '" + selectedAnimal + "'");
        predators->clear();
        removeLayout(ui->relationsLayout);
        ui->relationsLayout->update();
        labelPredator = new QLabel();
        labelPredator->setText("Являются рационом для: ");
        labelPredator->setStyleSheet("font-weight: bold");
        ui->relationsLayout->addWidget(labelPredator);
        while(selectQuery.next()){
            QLabel *label = new QLabel;
            label->setText(selectQuery.value(0).toString());
            QLineEdit *lineEdit = new QLineEdit;
            lineEdit->setValidator(countValidator);
            QPair<QString, QLineEdit*> newPredator;
            newPredator.first = selectQuery.value(0).toString();
            newPredator.second = lineEdit;
            predators->push_back(newPredator);
            QHBoxLayout *layout = new QHBoxLayout();
            layout->addWidget(label);
            layout->addWidget(lineEdit);
            ui->relationsLayout->addLayout(layout);
        }
        selectQuery.exec("SELECT " RELATIONS_PREY " FROM " TABLE_RELATIONS " WHERE " RELATIONS_PREDATOR  " = '" + selectedAnimal + "'");
        preys->clear();
        labelPrey = new QLabel();
        labelPrey->setText("Питаются:");
        labelPrey->setStyleSheet("font-weight: bold");
        ui->relationsLayout->addWidget(labelPrey);
        while(selectQuery.next()){
            QLabel *label = new QLabel;
            label->setText(selectQuery.value(0).toString());
            QLineEdit *lineEdit = new QLineEdit;
            lineEdit->setValidator(countValidator);
            QPair<QString, QLineEdit*> newPrey;
            newPrey.first = selectQuery.value(0).toString();
            newPrey.second = lineEdit;
            preys->push_back(newPrey);
            QHBoxLayout *layout = new QHBoxLayout();
            layout->addWidget(label);
            layout->addWidget(lineEdit);
            ui->relationsLayout->addLayout(layout);
        }
        ui->relationsLayout->update();
    }
}
