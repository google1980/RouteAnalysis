#ifndef DELEGATE_H
#define DELEGATE_H

#include <QItemDelegate>
#include <QComboBox>

class MyComboDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    MyComboDelegate(QObject *parent = 0,int type = 0,int column = 1);

//任何代理类重写，都需要重写以下至少前三个函数
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;  //创建一个EDIT
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
//设置EDIT内的文本显示
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
//设置edit内的data值（有时需要，有时不需要看情况）
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const; //设置形状之类，可以省略
private:
    int m_type;
    int m_column;

};

#endif // DELEGATE_H
