#include "dbmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

DbManager::DbManager(QObject *parent) : QObject(parent)
{

}

DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database Connected Successfully !!!";
    }
    createTable();
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

bool DbManager::createTable()
{
    bool success = false;

    QSqlQuery query;

    query.prepare("CREATE TABLE  student (id INTEGER PRIMARY KEY, name TEXT, "
                  "  fatherName TEXT, motherName TEXT, mobileNumber TEXT, "
                  "gender TEXT, qualification TEXT, skills TEXT, state TEXT, "
                  "city TEXT, landmark TEXT, pincode TEXT)");
    if (!query.exec())
    {
        qDebug() << "Couldn't create the table 'student': one might already exist.";
        success = false;
    }

    return success;
}

bool DbManager::addRecord(const QString &name, const QString &fatherName,
                          const QString &motherName, const QString &mobileNumber,
                          const QString &selectedGender, const QString &qualification,
                          const QString &skills, const QString &state,
                          const QString &city, const QString &landmark,
                          const QString &pincode)
{
    if (isOpen())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO student (name, fatherName, motherName, mobileNumber, gender, qualification, skills, state, city, landmark, pincode) "
                         "VALUES (:name, :fatherName, :motherName, :mobileNumber, :gender, :qualification, :skills, :state, :city, :landmark, :pincode)");
        queryAdd.bindValue(":name", name);
        queryAdd.bindValue(":fatherName", fatherName);
        queryAdd.bindValue(":motherName", motherName);
        queryAdd.bindValue(":mobileNumber", mobileNumber);
        queryAdd.bindValue(":gender", selectedGender);
        queryAdd.bindValue(":qualification", qualification);
        queryAdd.bindValue(":skills", skills);
        queryAdd.bindValue(":state", state);
        queryAdd.bindValue(":city", city);
        queryAdd.bindValue(":landmark", landmark);
        queryAdd.bindValue(":pincode", pincode);

        if (queryAdd.exec())
        {
            return true;
        }
        else
        {
            qDebug() << "Data insertion failed: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "Database is not open!";
    }

    return false;
}

bool DbManager::removeRecord(const QString& name)
{
    bool success = false;

    if (recordExists(name))
    {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM people WHERE name = (:name)");     ///////////////////////////////////////
        queryDelete.bindValue(":name", name);
        success = queryDelete.exec();

        if(!success)
        {
            qDebug() << "remove person failed: " << queryDelete.lastError();
        }
    }
    else
    {
        qDebug() << "remove person failed: person doesnt exist";
    }

    return success;
}

void DbManager::printAllRecords() const
{
    qDebug() << "Persons in Database:";
    QSqlQuery query("SELECT * FROM student");

    if (!query.exec())
    {
        qDebug() << "Error executing query: " << query.lastError().text();
        return;
    }

    while (query.next())
    {
        QString name = query.value("name").toString();
        QString fatherName = query.value("fatherName").toString();
        QString motherName = query.value("motherName").toString();
        QString mobileNumber = query.value("mobileNumber").toString();
        QString selectedGender = query.value("gender").toString();
        QString qualification = query.value("qualification").toString();
        QString skills = query.value("skills").toString();
        QString state = query.value("state").toString();
        QString city = query.value("city").toString();
        QString landmark = query.value("landmark").toString();
        QString pincode = query.value("pincode").toString();

        qDebug() << "Name: " << name;
        qDebug() << "Father's Name: " << fatherName;
        qDebug() << "Mother's Name: " << motherName;
        qDebug() << "Mobile Number: " << mobileNumber;
        qDebug() << "Gender: " << selectedGender;
        qDebug() << "Qualification: " << qualification;
        qDebug() << "Skills: " << skills;
        qDebug() << "State: " << state;
        qDebug() << "City: " << city;
        qDebug() << "Landmark: " << landmark;
        qDebug() << "Pincode: " << pincode;
    }
}

bool DbManager::recordExists(const QString& name) const
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT name FROM people WHERE name = (:name)");              ///////////////////////////////////////
    checkQuery.bindValue(":name", name);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }
    else
    {
        qDebug() << "person exists failed: " << checkQuery.lastError();
    }

    return exists;
}

bool DbManager::removeAllRecords()
{
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM people");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all persons failed: " << removeQuery.lastError();
    }

    return success;
}

bool DbManager::saveToSqlite(const QString &data)
{
    QStringList list =data.split(",");
    qDebug()<<"Input Data" <<list;
    //printAllRecords();

    addRecord(list.at(0),list.at(1),list.at(2),list.at(3),list.at(4),list.at(5),list.at(6),list.at(7)
              ,list.at(8),list.at(9),list.at(10));

    printAllRecords();
}

