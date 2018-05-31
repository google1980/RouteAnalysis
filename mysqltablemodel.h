#ifndef MYSQLTABLEMODEL_H
#define MYSQLTABLEMODEL_H
#include <QSqlTableModel>

class MySqlTableModel : public QSqlTableModel
{
public:
    MySqlTableModel(QObject *parent);
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // MYSQLTABLEMODEL_H
