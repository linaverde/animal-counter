#include "databasehelper.h"


DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

void DataBase::connectToDataBase()
{
    qDebug() << "Current path" << QCoreApplication::applicationDirPath();
    if(!QFile(QCoreApplication::applicationDirPath() + DATABASE_NAME).exists()){
        qDebug() << "Восстановление базы данных";
        this->restoreDataBase();
    } else {
        this->openDataBase();
        qDebug() << "Открытие базы данных";
    }
}


bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        return (this->createTable()) ? true : false;
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}


bool DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(QCoreApplication::applicationDirPath() + DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}


void DataBase::closeDataBase()
{
    db.close();
}

bool DataBase::deleteRow(QString table, QString id, QString value){
    QSqlQuery query;
    if (!query.exec("DELETE FROM " + table + " WHERE " + id + "= '" + value + "'")){
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase::execQuery(QString queryStr){
    QSqlQuery query;
    if (!query.exec(queryStr)){
        qDebug() << "Ошибка выполнения запроса";
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase::isExist(QString tableName, QString keyName, QString key){
    QSqlQuery querySelect;
    if(!querySelect.exec("SELECT COUNT(*) FROM " + tableName + " WHERE " + keyName + " = '" + key + "'"))
    {
        qDebug() << "Error select from table " + tableName;
        qDebug() << querySelect.lastError().text();
        return false;
    } else {
        querySelect.next();
        return querySelect.value(0).toInt() > 0;
    }
}

bool DataBase::createTable()
{
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys=on");
    //создание заполненной таблицы с месяцами и количеством дней
    if (!query.exec( "CREATE TABLE " TABLE_MONTH " ("
                     MONTH_NUMBER " INTEGER PRIMARY KEY NOT NULL, "
                     MONTH_DAYS " INTEGER NOT NULL )"
                     )){
        qDebug() << "DataBase: error of create " << TABLE_MONTH;
        qDebug() << query.lastError().text();
        return false;
    } else {
        //заполнение таблицы месяцев
        if (!query.exec("INSERT INTO " TABLE_MONTH "(" MONTH_NUMBER ", " MONTH_DAYS ") VALUES "
                        "(1, 31),"
                        "(2, 28),"
                        "(3, 31),"
                        "(4, 30),"
                        "(5, 31),"
                        "(6, 30),"
                        "(7, 31),"
                        "(8, 31),"
                        "(9, 30),"
                        "(10, 31),"
                        "(11, 30),"
                        "(12, 31)")){
            qDebug() << "DataBase: error of create " << TABLE_MONTH;
            qDebug() << query.lastError().text();
            return false;
        };
    }
    //создание таблицы ареалов
    if(!query.exec( "CREATE TABLE " TABLE_AREA " ("
                    AREAL_NAME " VARCHAR(255) PRIMARY KEY NOT NULL, "
                    AREAL_LATITUDE " VARCHAR(1) NOT NULL, "
                    AREAL_LATITUDE_VALUE " REAL NOT NULL, "
                    AREAL_LONGITUDE " VARCHAR(1) NOT NULL, "
                    AREAL_LONGITUDE_VALUE " REAL NOT NULL, "
                    AREAL_RADIUS " INTEGER NOT NULL"
                    " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE_AREA;
        qDebug() << query.lastError().text();
        return false;
    } else {
        //заполнение ареалов по умолчанию
        if (!query.exec("INSERT INTO " TABLE_AREA
                        "(" AREAL_NAME ", " AREAL_LATITUDE ", " AREAL_LONGITUDE ", "
                        AREAL_LATITUDE_VALUE ", " AREAL_LONGITUDE_VALUE ", " AREAL_RADIUS ") VALUES "
                        "('Тайга', 'N', 'E', 56.06, 85.62, 40),"
                        "('Саванна', 'N', 'W', 10, 10, 50)")){
            qDebug() << "DataBase: error of create " << TABLE_AREA;
            qDebug() << query.lastError().text();
            return false;
        }
    }
    //создание таблицы животных
    if(!query.exec("CREATE TABLE " TABLE_ANIMAL "("
                   ANIMAL_NAME " VARCHAR(255) PRIMARY KEY NOT NULL, "
                   ANIMAL_TYPE " VARCHAR(10) NOT NULL, "
                   ANIMAL_AREA " VARCHAR(255) NOT NULL, "
                   ANIMAL_NORM_CAL " INTEGER NOT NULL, "
                   ANIMAL_CALORIES " INTEGER NOT NULL, "
                   ANIMAL_FEMALE " INTEGER NOT NULL, "
                   ANIMAL_CHILDREN " INTEGER NOT NULL, "
                   ANIMAL_DEATH " INTEGER NOT NULL, "
                   ANIMAL_HUMAN " INTEGER NOT NULL, "
                   " FOREIGN KEY (" ANIMAL_AREA ") REFERENCES " TABLE_AREA "(" AREAL_NAME "))")){
        qDebug() << "DataBase: error of create " << TABLE_ANIMAL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        //заполнение таблицы животных по умолчанию
        if (!query.exec("INSERT INTO " TABLE_ANIMAL
                        "(" ANIMAL_NAME ", " ANIMAL_TYPE ", " ANIMAL_AREA ", "
                        ANIMAL_NORM_CAL ", " ANIMAL_CALORIES ", " ANIMAL_FEMALE ", "
                        ANIMAL_CHILDREN ", " ANIMAL_DEATH ", " ANIMAL_HUMAN
                        ") VALUES "
                        "('Иван-чай', 'Растение', 'Тайга', 0, 10, 90, 30, 100, -60),"
                        "('Слоновая трава', 'Растение', 'Саванна', 0, 70, 80, 40, 70, -30),"
                        "('Заяц', 'Травоядное', 'Тайга', 400, 800, 45, 7, 15, -15), "
                        "('Антилопа', 'Травоядное', 'Саванна', 700, 2200, 60, 3, 4, -7),"
                        "('Волк', 'Плотоядное', 'Тайга', 150, 5000, 50, 7, 5, -2),"
                        "('Лев', 'Плотоядное', 'Саванна', 300, 8000, 70, 5, 2, 3)")){
            qDebug() << "DataBase: error of create " << TABLE_AREA;
            qDebug() << query.lastError().text();
            return false;
        }
    }
    //создание таблицы отношений
    if (!query.exec("CREATE TABLE " TABLE_RELATIONS " ("
                    RELATIONS_PREDATOR " VARCHAR(255) NOT NULL, "
                    RELATIONS_PREY " VARCHAR(255) NOT NULL, "
                    "FOREIGN KEY (" RELATIONS_PREDATOR ") REFERENCES " TABLE_ANIMAL "(" ANIMAL_NAME "), "
                    "FOREIGN KEY (" RELATIONS_PREY ") REFERENCES " TABLE_ANIMAL "(" ANIMAL_NAME "), "
                    "PRIMARY KEY (" RELATIONS_PREDATOR ", " RELATIONS_PREY")"
                    " )")){
        qDebug() << "DataBase: error of create " << TABLE_RELATIONS;
        qDebug() << query.lastError().text();
        return false;
    } else {
        if (!query.exec("INSERT INTO " TABLE_RELATIONS " (" RELATIONS_PREDATOR ", " RELATIONS_PREY ") VALUES "
                   "('Заяц', 'Иван-чай'), ('Волк', 'Заяц'), "
                   "('Антилопа', 'Слоновая трава'), ('Лев', 'Антилопа')")){
            qDebug() << "DataBase: error of create " << TABLE_RELATIONS;
            qDebug() << query.lastError().text();
            return false;
        }
    }
    //создание вспомогательных таблиц для животных
    if (!query.exec("CREATE TABLE " TABLE_ANIMAL_ACT_MONTH " ("
                    ACT_ANIMAL " VARCHAR(255) NOT NULL, "
                    ACT_MONTH " INTEGER NOT NULL, "
                    "FOREIGN KEY (" ACT_MONTH ") REFERENCES " TABLE_MONTH "(" MONTH_NUMBER "), "
                    "FOREIGN KEY (" ACT_ANIMAL ") REFERENCES " TABLE_ANIMAL "(" ANIMAL_NAME "), "
                    "PRIMARY KEY (" ACT_ANIMAL ", " ACT_MONTH")"
                    " )")){
        qDebug() << "DataBase: error of create " << TABLE_ANIMAL_ACT_MONTH;
        qDebug() << query.lastError().text();
        return false;
    } else {
        query.exec("INSERT INTO " TABLE_ANIMAL_ACT_MONTH
                   " (" ACT_ANIMAL ", " ACT_MONTH ") VALUES "
                                                  "('Иван-чай', 4), ('Иван-чай', 5), ('Иван-чай', 6), ('Иван-чай', 7), ('Иван-чай', 8), ('Иван-чай', 9), ('Иван-чай', 10),"
                                                  "('Слоновая трава', 1), ('Слоновая трава', 2), ('Слоновая трава', 3), ('Слоновая трава', 4), ('Слоновая трава', 5), ('Слоновая трава', 6),"
                                                  "('Слоновая трава', 9), ('Слоновая трава', 10), ('Слоновая трава', 11), ('Слоновая трава', 12)");
        for (int i = 1; i <=12; i++){
            query.exec("INSERT INTO " TABLE_ANIMAL_ACT_MONTH
                       " (" ACT_ANIMAL ", " ACT_MONTH ") VALUES "
                                                      "('Заяц', " + QString::number(i) +  ")");
            query.exec("INSERT INTO " TABLE_ANIMAL_ACT_MONTH
                       " (" ACT_ANIMAL ", " ACT_MONTH ") VALUES "
                                                      "('Волк', " + QString::number(i) +  ")");
            query.exec("INSERT INTO " TABLE_ANIMAL_ACT_MONTH
                       " (" ACT_ANIMAL ", " ACT_MONTH ") VALUES "
                                                      "('Антилопа', " + QString::number(i) +  ")");
            query.exec("INSERT INTO " TABLE_ANIMAL_ACT_MONTH
                       " (" ACT_ANIMAL ", " ACT_MONTH ") VALUES "
                                                      "('Лев', " + QString::number(i) +  ")");
        }
    }
    if (!query.exec("CREATE TABLE " TABLE_ANIMAL_REP_MONTH " ("
                    REP_ANIMAL " VARCHAR(255) NOT NULL, "
                    REP_MONTH " INTEGER NOT NULL, "
                    "FOREIGN KEY (" REP_MONTH ") REFERENCES " TABLE_MONTH "(" MONTH_NUMBER "), "
                    "FOREIGN KEY (" REP_ANIMAL ") REFERENCES " TABLE_ANIMAL "(" ANIMAL_NAME "), "
                    "PRIMARY KEY (" REP_ANIMAL ", " REP_MONTH")"
                    " )")){
        qDebug() << "DataBase: error of create " << TABLE_ANIMAL_REP_MONTH;
        qDebug() << query.lastError().text();
        return false;
    } else {
        query.exec("INSERT INTO " TABLE_ANIMAL_REP_MONTH
                   " (" REP_ANIMAL ", " REP_MONTH ") VALUES "
                                                  "('Иван-чай', 4), ('Иван-чай', 5), ('Иван-чай', 6), ('Иван-чай', 7), ('Иван-чай', 8), ('Иван-чай', 9), ('Иван-чай', 10), "
                                                  "('Слоновая трава', 1), ('Слоновая трава', 2), ('Слоновая трава', 3), ('Слоновая трава', 4), ('Слоновая трава', 5), "
                                                  "('Слоновая трава', 6), ('Слоновая трава', 10), ('Слоновая трава', 11), ('Слоновая трава', 12), "
                                                  "('Заяц', 3), ('Заяц', 5), ('Заяц', 7), ('Заяц', 9),"
                                                  "('Антилопа', 1), ('Антилопа', 2), ('Антилопа', 3), ('Антилопа', 10), ('Антилопа', 11), ('Антилопа', 12),"
                                                  "('Волк', 4), ('Волк', 5), ('Волк', 6), "
                                                  "('Лев', 4), ('Лев', 7), ('Лев', 10), ('Лев', 12)");
    }
    if (!query.exec("CREATE TABLE " TABLE_ANIMAL_HUMAN_MONTH " ("
                    HUMAN_ANIMAL " VARCHAR(255) NOT NULL, "
                    HUMAN_MONTH " INTEGER NOT NULL, "
                    "FOREIGN KEY (" HUMAN_MONTH ") REFERENCES " TABLE_MONTH "(" MONTH_NUMBER "), "
                    "FOREIGN KEY (" HUMAN_ANIMAL ") REFERENCES " TABLE_ANIMAL "(" ANIMAL_NAME "), "
                    "PRIMARY KEY (" HUMAN_ANIMAL ", " HUMAN_MONTH")"
                    " )")){
        qDebug() << "DataBase: error of create " << TABLE_ANIMAL_HUMAN_MONTH;
        qDebug() << query.lastError().text();
        return false;
    } else {
        query.exec("INSERT INTO " TABLE_ANIMAL_HUMAN_MONTH
                   " (" HUMAN_ANIMAL ", " HUMAN_MONTH ") VALUES "
                                                      "('Иван-чай', 8), ('Иван-чай', 9), "
                                                      "('Слоновая трава', 10), ('Слоновая трава', 11), "
                                                      "('Заяц', 3), ('Заяц', 5), ('Заяц', 9),"
                                                      "('Антилопа', 1), ('Антилопа', 2), ('Антилопа', 3), ('Антилопа', 10), ('Антилопа', 11), ('Антилопа', 12),"
                                                      "('Волк', 4), ('Волк', 5), ('Волк', 6)");
    }
    return true;
}
