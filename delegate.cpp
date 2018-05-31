#include "delegate.h"

MyComboDelegate::MyComboDelegate(QObject *parent,int type,int column)  //空的构造函数也是必须的，之前忘了写，编译报错
        : QItemDelegate(parent)
        , m_type(type)
        , m_column(column)
{
}


QWidget* MyComboDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    QStringList names;
    QComboBox *channelBox = new QComboBox(parent);
    switch (m_type){
    case 0:
        if(index.column() == m_column)   //colRycchannel是列的枚举类型，也可以用立即数，那样比较low
        {

    //此处如果不加parent，edit不会内嵌在表格中 会跑出来
            channelBox->setFixedHeight(option.rect.height());
            names = QColor::colorNames();
            for (int i = 0; i < names.size(); ++i) {
                QColor color(names[i]);
                channelBox->insertItem(i, names[i]);
                channelBox->setItemData(i, color, Qt::DecorationRole);
            }
            return channelBox;
        }else{

            return QItemDelegate::createEditor(parent, option, index);

        }
        break;
    case 1:
    case 2:
        if(index.column() == m_column)   //colRycchannel是列的枚举类型，也可以用立即数，那样比较low
        {

    //此处如果不加parent，edit不会内嵌在表格中 会跑出来
            channelBox->setFixedHeight(option.rect.height());
            names.append("N");
            names.append("Y");
            for (int i = 0; i < names.size(); ++i) {
                channelBox->insertItem(i, names[i]);
                channelBox->setItemData(i,names[i]);
            }
            return channelBox;
        }else{

            return QItemDelegate::createEditor(parent, option, index);

        }
        break;
    default:
        break;
    }
    return QItemDelegate::createEditor(parent, option, index);

}

//以下几个函数都可以直接拷贝用
void MyComboDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{

    switch (m_type){

    case 0:
        if(index.column() == m_column)
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            if(comboBox)
            {
                QString str = index.model()->data(index, Qt::EditRole).toString();
                comboBox->setCurrentIndex(comboBox->findText(str));
            }
        }
        else
        {
            QItemDelegate::setEditorData(editor, index);
        }
        break;
    case 1:
    case 2:
        if(index.column() == m_column)
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            if(comboBox)
            {
                QString str = index.model()->data(index, Qt::EditRole).toString();
                comboBox->setCurrentIndex(comboBox->findText(str));
            }
        }
        else
        {
            QItemDelegate::setEditorData(editor, index);
        }
        break;
    default:
        QItemDelegate::setEditorData(editor, index);
        break;
    }

}


void MyComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    int nColumn = index.column();
    switch (m_type){
    case 0:
        if((nColumn==m_column))
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            if(comboBox!=0)
                model->setData(index, comboBox->currentText(), Qt::EditRole);
        }
        else
        {
            QItemDelegate::setModelData(editor, model, index);
        }
        break;
    case 1:
    case 2:
        if((nColumn==m_column))
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            if(comboBox!=0)
                model->setData(index, comboBox->currentText(), Qt::EditRole);
        }
        else
        {
            QItemDelegate::setModelData(editor, model, index);
        }
        break;
    default:
        QItemDelegate::setModelData(editor, model, index);
        break;

    }


}


void MyComboDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

