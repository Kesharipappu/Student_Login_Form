#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QObject>
#include <QSqlDatabase>


class DbManager : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(QObject *parent = nullptr);

    DbManager(const QString& path);

    ~DbManager();
    bool isOpen() const;


    bool createTable();

    bool addRecord(const QString &name, const QString &fatherName,
                   const QString &motherName, const QString &mobileNumber,
                   const QString &selectedGender, const QString &qualification,
                   const QString &skills, const QString &state,
                   const QString &city, const QString &landmark,
                   const QString &pincode);

    bool removeRecord(const QString& name);

    bool recordExists(const QString& name) const;

    void printAllRecords() const;

    bool removeAllRecords();
public slots:

    //It will Save Data From QML UI to Sqlite
    Q_INVOKABLE bool saveToSqlite(const QString &data);
private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H

