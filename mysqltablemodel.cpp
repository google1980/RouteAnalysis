#include "mysqltablemodel.h"
#include <QColor>
#include <QBrush>

MySqlTableModel::MySqlTableModel(QObject *parent, QSqlDatabase db,int type)
    : QSqlTableModel(parent,db)
    , m_type(type)
{

}

MySqlTableModel::~MySqlTableModel()
{

}

QVariant MySqlTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value  = QSqlTableModel::data(index,role);
    if(Qt::DisplayRole == role){
        if(18 == index.column())
        {
            switch (value.toInt()){
            case 0:
                value = "Center";
                break;
            case 1:
                value = "Top";
                break;
            case 2:
                value = "Bottom";
                break;
            default:
                value = "";
                break;
            }

            return value;
        }
    }else if(Qt::BackgroundRole == role){

        if ((1 == index.column())&&(index.data().toString().isEmpty() == false)&&(m_type == 1))
        {
            QColor color = QColor(index.data().toString());
            return QBrush(color);
        }
        else{

        }
    }
    return value;
}
