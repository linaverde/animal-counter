#ifndef KNOWLEGEEDITORWINDOW_H
#define KNOWLEGEEDITORWINDOW_H

#include <QWidget>
#include "areatable.h"
#include "animaltable.h"
#include "relationstable.h"

namespace Ui {
class KnowlegeEditorWindow;
}

class KnowlegeEditorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit KnowlegeEditorWindow(QWidget *parent = nullptr);
    ~KnowlegeEditorWindow();

private:
    Ui::KnowlegeEditorWindow *ui;
    AreaTable *areaTable;
    AnimalTable *animalTable;
    RelationsTable *relationsTable;


private Q_SLOTS:
    void areaClicked();
    void animalClicked();
    void relationsClicked();

};

#endif // KNOWLEGEEDITORWINDOW_H
