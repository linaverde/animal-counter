#include "knowlegeeditorwindow.h"
#include "ui_knowlegeeditorwindow.h"

KnowlegeEditorWindow::KnowlegeEditorWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KnowlegeEditorWindow)
{
    ui->setupUi(this);

    areaTable = new AreaTable();

    relationsTable = new RelationsTable();

    connect(ui->areaButton, SIGNAL(clicked()), this, SLOT(areaClicked()));
    connect(ui->animalButton, SIGNAL(clicked()), this, SLOT(animalClicked()));
    connect(ui->relationsButton, SIGNAL(clicked()), this, SLOT(relationsClicked()));

}

KnowlegeEditorWindow::~KnowlegeEditorWindow()
{
    delete ui;
}

void KnowlegeEditorWindow::areaClicked()
{
    areaTable->show();
}

void KnowlegeEditorWindow::animalClicked()
{
    animalTable = new AnimalTable();
    animalTable->show();
}

void KnowlegeEditorWindow::relationsClicked()
{
    relationsTable->show();
}
