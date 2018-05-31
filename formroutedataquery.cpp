#include "formroutedataquery.h"
#include "ui_formroutedataquery.h"
#include <QAxObject>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include "delegate.h"

FormRouteDataQuery::FormRouteDataQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormRouteDataQuery)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable("ROUTE_ARRANGEMENT");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->select();

    model->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("作业单位")  );
    model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("航线名称"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("航线代码")  );
    model->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("航区")  );
    model->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("最大船长"));
    model->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("航线经营人"));
    model->setHeaderData(6, Qt::Horizontal, QString::fromUtf8("上下港"));
    model->setHeaderData(7, Qt::Horizontal, QString::fromUtf8("船代"));
    model->setHeaderData(8, Qt::Horizontal, QString::fromUtf8("月均箱量"));
    model->setHeaderData(9, Qt::Horizontal, QString::fromUtf8("船型"));
    model->setHeaderData(10, Qt::Horizontal, QString::fromUtf8("计划靠泊日期"));
    model->setHeaderData(11, Qt::Horizontal, QString::fromUtf8("计划靠泊时间")  );
    model->setHeaderData(12, Qt::Horizontal, QString::fromUtf8("计划离泊日期"));
    model->setHeaderData(13, Qt::Horizontal, QString::fromUtf8("计划离泊时间"));
    model->setHeaderData(14, Qt::Horizontal, QString::fromUtf8("靠泊起点")  );
    model->setHeaderData(15, Qt::Horizontal, QString::fromUtf8("锁定标识")  );

    ui->tableView->setModel(model);
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(OnSubmit()));
    connect(ui->exportButton, SIGNAL(clicked()), this, SLOT(OnExport()));

    ui->tableView->setItemDelegate(new MyComboDelegate(this,1,15));


    setAttribute(Qt::WA_DeleteOnClose);
}

FormRouteDataQuery::~FormRouteDataQuery()
{
    delete ui;
}

void FormRouteDataQuery::OnSubmit()
{
    //int curRow = ui->tableView->currentIndex().row();
    //model->removeRow(curRow);
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    }
    else {
        model->database().rollback();
    }

}

void FormRouteDataQuery::OnDelete()
{
    int curRow = ui->tableView->currentIndex().row();
    model->removeRow(curRow);
}


void FormRouteDataQuery::OnExport()
{
    QString fileName = QFileDialog::getSaveFileName(this, "save",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"Excel file(*.xls *.xlsx)");
    if (fileName!="")
    {

        QAxObject *excel = new QAxObject;
        if (excel->setControl("Excel.Application"))
        {
            ui->exportButton->setDisabled(true);
            excel->dynamicCall("SetVisible (bool Visible)","false");
            excel->setProperty("DisplayAlerts", false);
            QAxObject *workbooks = excel->querySubObject("WorkBooks");
            workbooks->dynamicCall("Add");
            QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
            QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

            int i,j;

            int colcount = model->columnCount()-1;

            int rowcount = model->rowCount();

            QAxObject *cell,*col;

            for(i=0;i<colcount;i++)
            {
                QString columnName;
                columnName.append(QChar(i  + 'A'));
                columnName.append(":");
                columnName.append(QChar(i + 'A'));
                col = worksheet->querySubObject("Columns(const QString&)", columnName);
                col->setProperty("ColumnWidth", ui->tableView->columnWidth(i)/6);
                cell=worksheet->querySubObject("Cells(int,int)", 1, i+1);

                columnName=model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
                cell->dynamicCall("SetValue(const QString&)", columnName);
                cell->querySubObject("Font")->setProperty("Bold", true);
                cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
                cell->setProperty("HorizontalAlignment", -4108);
                cell->setProperty("VerticalAlignment", -4108);
            }

            for(i=0;i<rowcount;i++)
            {
                for (j=0;j<colcount;j++)
                {
                    QModelIndex index = model->index(i, j);
                    QString strdata = model->data(index).toString();

                    if((j == 8) || (j == 10)){
                        strdata = "  " + strdata;
                    }

                    worksheet->querySubObject("Cells(int,int)", i+2, j+1)->dynamicCall("SetValue(const QString&)", strdata);
                }
            }

            QString lrange;
            lrange.append("A2:");
            lrange.append(colcount - 1 + 'A');
            lrange.append(QString::number(model->rowCount() + 1));
            QAxObject *range;
            range = worksheet->querySubObject("Range(const QString&)", lrange);
            range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
            range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));

            QString rowsName;
            rowsName.append("2:");
            rowsName.append(QString::number(model->rowCount() + 1));
            range = worksheet->querySubObject("Range(const QString&)", rowsName);
            range->setProperty("RowHeight", 20);
            workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));
            workbook->dynamicCall("Close()");
            excel->dynamicCall("Quit()");
            delete excel;
            excel=NULL;
            ui->exportButton->setEnabled(true);
            QMessageBox::information(this, "information", QString::fromUtf8("恭喜您! Excle文件已成功导出了."));


        }
        else
        {
            ui->exportButton->setEnabled(true);
            QMessageBox::warning(NULL,"error",QString::fromUtf8("未能创建 Excel 对象，请安装 Microsoft Excel"),QMessageBox::Apply);

        }
    }



}
