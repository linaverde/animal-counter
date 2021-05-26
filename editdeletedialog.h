#ifndef EDITDELETEDIALOG_H
#define EDITDELETEDIALOG_H

#include <QDialog>

namespace Ui {
class EditDeleteDialog;
}

class EditDeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDeleteDialog(QWidget *parent = nullptr);
    ~EditDeleteDialog();

signals:
    void deleteSignal();

private:
    Ui::EditDeleteDialog *ui;

private Q_SLOTS:
    void closeWindow();
    void deleteClicked();
};

#endif // EDITDELETEDIALOG_H
