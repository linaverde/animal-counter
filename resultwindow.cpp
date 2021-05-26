#include "resultwindow.h"
#include "ui_resultwindow.h"

ResultWindow::ResultWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::setSelected(QString selected, int month, int count){
    selectedAnimal = selected;
    selectesMonth = month;
    startCount = count;
    this->setWindowTitle("Прогноз для популяции " + selectedAnimal);
}

void ResultWindow::setPredators(QList <QPair<QString, int>> *list){
    this->predators = list;
}

void ResultWindow::setPreys(QList <QPair<QString, int>> *list){
    this->preys = list;
}

void ResultWindow::predict(){
    explain = "<b>Прогноз для популяции " + selectedAnimal + ".</b><br><br>";

    QSqlQuery query;
    int act = -1, rep = -1, human = -1;
    query.exec("SELECT COUNT (*) FROM " TABLE_ANIMAL_ACT_MONTH " WHERE " ACT_ANIMAL " = '" + selectedAnimal + "' AND " ACT_MONTH  " = " +  QString::number(selectesMonth));
    query.next();
    act = query.value(0).toInt();

    query.exec("SELECT COUNT (*) FROM " TABLE_ANIMAL_REP_MONTH " WHERE " REP_ANIMAL " = '" + selectedAnimal + "' AND " REP_MONTH  " = " +  QString::number(selectesMonth));
    query.next();
    rep = query.value(0).toInt();

    query.exec("SELECT COUNT (*) FROM " TABLE_ANIMAL_HUMAN_MONTH " WHERE " HUMAN_ANIMAL " = '" + selectedAnimal + "' AND " HUMAN_MONTH  " = " +  QString::number(selectesMonth));
    query.next();
    human = query.value(0).toInt();

    qDebug() << "ACT: " + QString::number(act);
    qDebug() << "REP: " + QString::number(rep);
    qDebug() << "HUMAN: " + QString::number(human);

    query.exec("SELECT " ANIMAL_CHILDREN "," ANIMAL_FEMALE " FROM " TABLE_ANIMAL " WHERE " ANIMAL_NAME " = '" + selectedAnimal +"'");
    query.next();
    int increace = rep * query.value(0).toInt() * startCount * (query.value(1).toDouble()/100.0);
    if (rep == 0){
        explain += "Месяц " + QString::number(selectesMonth) + " не является месяцем размножения для популяции " + selectedAnimal +". Прирост популяции составляет 0. <br><br>";
    } else {
        explain += "Месяц " + QString::number(selectesMonth) + " является месяцем размножения  для популяции " + selectedAnimal +". ";
        explain += "Процент женских особей " + query.value(1).toString()+ "%, что составляет "
                + QString::number(startCount * (query.value(1).toDouble()/100.0)) +" особей. ";
        explain += "Среднее количество потомства для " + selectedAnimal + " - " + query.value(0).toString() +". ";
        explain += "Таким образом <b> популяцию пополнят " + QString::number(increace) +" новорожденных особей. </b><br><br>";
    }

    query.exec("SELECT " MONTH_DAYS " FROM " TABLE_MONTH " WHERE " MONTH_NUMBER " = "  + QString::number(selectesMonth));
    query.next();
    int days = query.value(0).toInt();
    qDebug() << "В месяце " + QString::number(selectesMonth) + " количество дней " + QString::number(days);

    query.exec("SELECT " ANIMAL_NORM_CAL "," ANIMAL_CALORIES " FROM " TABLE_ANIMAL " WHERE " ANIMAL_NAME " = '" + selectedAnimal +"'");
    query.next();
    int norm = query.value(0).toInt();
    int cal = query.value(0).toInt();
    qDebug() << selectedAnimal + " потребляет " + QString::number(norm) + " калорий в день";
    qDebug() << selectedAnimal + " вкусный аж на " + QString::number(cal) + " калорий";

    int decreace = 0;
    for (int i = 0; i < predators->length(); i++){
        int predAct;
        query.exec("SELECT COUNT (*) FROM " TABLE_ANIMAL_ACT_MONTH " WHERE " ACT_ANIMAL " = '" + predators->at(i).first + "'"
                   " AND " ACT_MONTH  " = " +  QString::number(selectesMonth));
        query.next();
        predAct = query.value(0).toInt();
        int predNorm;
        query.exec("SELECT " ANIMAL_NORM_CAL " FROM " TABLE_ANIMAL " WHERE " ANIMAL_NAME " = '" + predators->at(i).first +"'");
        query.next();
        predNorm = query.value(0).toInt();
        int currDec = act * predAct * days * predNorm * predators->at(i).second / cal;
        decreace += currDec;

        if (predAct == 0){
            explain += "Месяц " + QString::number(selectesMonth) + " не является месяцем активности для <b>" + predators->at(i).first +"</b>.";
            explain += " Эта популяция не повлияет на размер популяции " + selectedAnimal +". <br>";
        } else {
            explain += "В месяце " + QString::number(selectesMonth) + " популяция " + predators->at(i).first +" активна. ";
            explain += predators->at(i).first + " съедят " + QString::number(currDec) +" особей. <br>";
        }
    }

    explain += "<b>Всего было съедено " + QString::number(decreace) + " особей.</b> <br><br>";

    query.exec("SELECT " ANIMAL_DEATH " FROM " TABLE_ANIMAL " WHERE " ANIMAL_NAME " = '" + selectedAnimal +"'");
    query.next();
    int death = startCount * query.value(0).toDouble()/100;

    explain += "<b>Своей смертью погибнет " + QString::number(death) + " особей. </b> <br><br>";

    decreace += death;

    double food = 0;
    for (int i = 0; i < preys->length(); i++){
        int preyAct;
        query.exec("SELECT COUNT (*) FROM " TABLE_ANIMAL_ACT_MONTH " WHERE " ACT_ANIMAL " = '" + preys->at(i).first + "'"
                   " AND " ACT_MONTH  " = " +  QString::number(selectesMonth));
        query.next();
        preyAct = query.value(0).toInt();
        qDebug() << "Активность жертвы " + preys->at(i).first + " " + QString::number(preyAct);
        if (preyAct == 1){
            explain += "Особи популяции " + preys->at(i).first + "активны в месяце " + QString::number(selectesMonth) +" и будут использоваться для пропитания. <br>";
        } else {
            explain += "Особи популяции " + preys->at(i).first + " не активны в месяце " + QString::number(selectesMonth) +", питаться ими не получится. <br>";
        }
        int preyCal;
        query.exec("SELECT " ANIMAL_CALORIES " FROM " TABLE_ANIMAL " WHERE " ANIMAL_NAME " = '" + preys->at(i).first +"'");
        query.next();
        preyCal = query.value(0).toInt();
        qDebug() << "Калорийность жертвы " + preys->at(i).first + " " + QString::number(preyCal);
        int currFood = preyAct * preyCal * preys->at(i).second;
        food = food + currFood;
    }

    food = food / double (norm*days*startCount);
    qDebug() << "Достаток еды " + QString::number(food);

    int survived;
    if (food >= 1){
        survived = startCount;
        explain += "Вокруг достаточно еды для пропитания " + selectedAnimal +", поэтому выживут все особи. <br><br>";
    } else {
        survived = (double) startCount * food;
        explain += "Еды для пропитания " + selectedAnimal +" недостаточно, её хватит лишь на " + QString::number(food*100) + " процентов популяции. ";
        explain += "<b>Выживет лишь " + QString::number(survived) + " особей. </b><br><br>";
    }
    qDebug() << "Выжило " + QString::number(survived) + " особей из исходной популяции. ";

    query.exec("SELECT " ANIMAL_HUMAN " FROM " TABLE_ANIMAL " WHERE " ANIMAL_NAME " = '" + selectedAnimal +"'");
    query.next();
    int humanCount = query.value(0).toInt()*human;
    explain += "<b>Человек повлияет на популяцию на " + QString::number(humanCount) + " особей </b>.";
    int predict = survived + increace - decreace + humanCount;
    if (predict < 0) predict = 0;
    ui->resultCounter->setText(QString::number(predict) + " особей");
    ui->textBrowser->setText(explain);
}
