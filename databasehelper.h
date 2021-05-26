#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#define DATABASE_HOSTNAME "AnimalPredict"
#define DATABASE_NAME "AnimalDB.db"

#define TABLE_AREA "areatable"
#define AREAL_NAME "name"
#define AREAL_LATITUDE "latitude"
#define AREAL_LONGITUDE "longitude"
#define AREAL_LATITUDE_VALUE "latitudevalue"
#define AREAL_LONGITUDE_VALUE "longitudevalue"
#define AREAL_RADIUS "radius"

#define TABLE_ANIMAL "animaltable"
#define ANIMAL_NAME "name"
#define ANIMAL_TYPE "type"
#define ANIMAL_AREA "area"
#define ANIMAL_NORM_CAL "normcalories"
#define ANIMAL_CALORIES "calories"
#define ANIMAL_FEMALE "female"
#define ANIMAL_CHILDREN "children"
#define ANIMAL_DEATH "death"
#define ANIMAL_HUMAN "human"
#define ANIMAL_ACT_MONTHS "activity"
#define ANIMAL_CHILD_MONTHS "reproduction"
#define ANIMAL_HUMAN_MONTHS "humantime"

#define TABLE_RELATIONS "relationstable"
#define RELATIONS_PREDATOR "predator"
#define RELATIONS_PREY "prey"

//вспомогательные таблицы
#define TABLE_MONTH "monthtable"
#define MONTH_NUMBER "number"
#define MONTH_DAYS "days"

#define TABLE_ANIMAL_ACT_MONTH "actmonthtable"
#define ACT_ANIMAL "name"
#define ACT_MONTH "month"

#define TABLE_ANIMAL_REP_MONTH "repmonthtable"
#define REP_ANIMAL "name"
#define REP_MONTH "month"

#define TABLE_ANIMAL_HUMAN_MONTH "humanmonthtable"
#define HUMAN_ANIMAL "name"
#define HUMAN_MONTH "month"


class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectToDataBase();
    bool deleteRow(QString table, QString id, QString value);
    bool execQuery(QString queryStr);
    bool isExist(QString tableName, QString keyName, QString key);

private:
    QSqlDatabase    db;

private:
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();

public slots:
//    bool inserIntoTable(const QVariantList &data);
//    bool removeRecord(const int id);
};

#endif // DATABASEHELPER_H
