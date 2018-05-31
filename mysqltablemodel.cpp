#include "mysqltablemodel.h"
#include <QColor>
#include <QDebug>
#include <QBrush>

MySqlTableModel::MySqlTableModel(QObject *parent)
    : QSqlTableModel(parent)
{

}

QVariant MySqlTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::BackgroundRole) {

        if (index.column() == 1)
        {
            QColor color = QColor(index.data().toString());
            return QBrush(color);
        }
        else
        {

        }
    }
    return QSqlTableModel::data(index, role);
}
