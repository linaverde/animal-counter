#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);


    knowlegeEditor = new KnowlegeEditorWindow();

    connect(ui->knowlegeEditButton, SIGNAL(clicked()), this, SLOT(knowlegeEditClicked()));
    connect(ui->predictButton, SIGNAL(clicked()), this, SLOT(predictClicked()));
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::predictClicked()
{
    predictWindow = new PredictWindow();
    predictWindow->show();
    //this->close();
}

void StartWindow::knowlegeEditClicked()
{
    knowlegeEditor->show();
    //this->close();
}
