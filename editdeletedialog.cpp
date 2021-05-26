#include "editdeletedialog.h"
#include "ui_editdeletedialog.h"

EditDeleteDialog::EditDeleteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDeleteDialog)
{
    ui->setupUi(this);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
}

EditDeleteDialog::~EditDeleteDialog()
{
    delete ui;
}

void EditDeleteDialog::deleteClicked(){
    emit deleteSignal();
    this->close();
}

void EditDeleteDialog::closeWindow(){
    this->close();
}

