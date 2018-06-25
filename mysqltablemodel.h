#ifndef MYSQLTABLEMODEL_H
#define MYSQLTABLEMODEL_H

#include <QVariant>
#include <QSqlDatabase>
#include <QSqlTableModel>


class MySqlTableModel : public QSqlTableModel
{
public:

    MySqlTableModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase(), int type = 0);
    ~MySqlTableModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
private:

    int m_type;

};

#endif // MYSQLTABLEMODEL_H
