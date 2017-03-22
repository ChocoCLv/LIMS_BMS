#include "excelengine.h"
#include "qt_windows.h"

ExcelEngine::ExcelEngine()
{
    pExcel     = NULL;
    pWorkbooks = NULL;
    pWorkbook  = NULL;
    pWorksheet = NULL;

    sXlsFile     = "";
    nRowCount    = 0;
    nColumnCount = 0;
    nStartRow    = 0;
    nStartColumn = 0;

    bIsOpen     = false;
    bIsValid    = false;
    bIsANewFile = false;
    bIsSaveAlready = false;

    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
    }
}

ExcelEngine::ExcelEngine(QString xlsFile)
{
    pExcel     = NULL;
    pWorkbooks = NULL;
    pWorkbook  = NULL;
    pWorksheet = NULL;

    sXlsFile     = xlsFile;
    nRowCount    = 0;
    nColumnCount = 0;
    nStartRow    = 0;
    nStartColumn = 0;

    bIsOpen     = false;
    bIsValid    = false;
    bIsANewFile = false;
    bIsSaveAlready = false;

    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
    }
}

ExcelEngine::~ExcelEngine()
{
    if ( bIsOpen )
    {
        //析构前，先保存数据，然后关闭workbook
        Close();
    }
    OleUninitialize();
}

/**
  *@brief 打开sXlsFile指定的excel报表
  *@return true : 打开成功
  *        false: 打开失败
  */
bool ExcelEngine::Open(UINT nSheet, bool visible)
{
    timer.start();
    if ( bIsOpen )
    {
        //return bIsOpen;
        Close();
    }

    nCurrSheet = nSheet;
    bIsVisible = visible;

    if ( NULL == pExcel )
    {
        pExcel = new QAxObject("Excel.Application");
        if ( pExcel )
        {
            bIsValid = true;
        }
        else
        {
            bIsValid = false;
            bIsOpen  = false;
            return bIsOpen;
        }

        pExcel->dynamicCall("SetVisible(bool)", bIsVisible);
    }

    if ( !bIsValid )
    {
        bIsOpen  = false;
        return bIsOpen;
    }

    if ( sXlsFile.isEmpty() )
    {
        bIsOpen  = false;
        return bIsOpen;
    }

    /*如果指向的文件不存在，则需要新建一个*/
    QFile f(sXlsFile);
    if (!f.exists())
    {
        bIsANewFile = true;
    }
    else
    {
        bIsANewFile = false;
    }

    if (!bIsANewFile)
    {
        pWorkbooks = pExcel->querySubObject("WorkBooks"); //获取工作簿
        pWorkbook = pWorkbooks->querySubObject("Open(QString, QVariant)",sXlsFile,QVariant(0)); //打开xls对应的工作簿
    }
    else
    {
        pWorkbooks = pExcel->querySubObject("WorkBooks");     //获取工作簿
        pWorkbooks->dynamicCall("Add");                       //添加一个新的工作薄
        pWorkbook  = pExcel->querySubObject("ActiveWorkBook"); //新建一个xls
    }

    pWorksheet = pWorkbook->querySubObject("WorkSheets(int)", nCurrSheet);//打开第一个sheet

    //至此已打开，开始获取相应属性
    QAxObject *usedrange = pWorksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject *rows = usedrange->querySubObject("Rows");
    QAxObject *columns = usedrange->querySubObject("Columns");

    //因为excel可以从任意行列填数据而不一定是从0,0开始，因此要获取首行列下标
    nStartRow    = usedrange->property("Row").toInt();    //第一行的起始位置
    nStartColumn = usedrange->property("Column").toInt(); //第一列的起始位置

    nRowCount    = rows->property("Count").toInt();       //获取行数
    nColumnCount = columns->property("Count").toInt();    //获取列数

    bIsOpen  = true;
    qDebug()<<"open cost:"<<timer.elapsed();
    return bIsOpen;
}

/**
  *@brief Open()的重载函数
  */
bool ExcelEngine::Open(QString xlsFile, UINT nSheet, bool visible)
{

    sXlsFile = xlsFile;
    nCurrSheet = nSheet;
    bIsVisible = visible;

    return Open(nCurrSheet,bIsVisible);
}


/**
  *@brief 关闭前先保存数据，然后关闭当前Excel COM对象，并释放内存
  */
void ExcelEngine::Close()
{
    if ( pExcel && pWorkbook )
    {
        pWorkbook->dynamicCall("Close(bool)", true);
        pExcel->dynamicCall("Quit()");

        delete pExcel;
        pExcel = NULL;

        bIsOpen     = false;
        bIsValid    = false;
        bIsANewFile = false;
        bIsSaveAlready = true;
    }
}



/**
  *@brief 从指定的xls文件中把数据导入到tableWidget中
  *@param tableWidget : 执行要导入到的tablewidget指针
  *@return 导入成功与否 true : 成功
  *                   false: 失败
  */
bool ExcelEngine::ReadDataToTable(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget )
    {
        return false;
    }

    //先把table的内容清空
    int tableColumn = tableWidget->columnCount();
    tableWidget->clear();
    for (int n=0; n<tableColumn; n++)
    {
        tableWidget->removeColumn(0);
    }

    int rowcnt    = nStartRow + nRowCount;
    int columncnt = nStartColumn + nColumnCount;

    timer.restart();
    QStringList headerList;
    for (int n = nStartColumn; n<columncnt; n++ )
    {
        QAxObject * cell = pWorksheet->querySubObject("Cells(int,int)",nStartRow, n);
        if ( cell )
        {
            headerList<<cell->dynamicCall("Value2()").toString();
        }
        delete cell;
    }

    //重新创建表头
    tableWidget->setColumnCount(nColumnCount);
    tableWidget->setHorizontalHeaderLabels(headerList);


    //插入新数据
    for (int i = nStartRow+1, r = 0; i < rowcnt; i++, r++ )  //行
    {
        tableWidget->insertRow(r); //插入新行
        for (int j = nStartColumn, c = 0; j < columncnt; j++, c++ )  //列
        {
            QAxObject * cell = pWorksheet->querySubObject("Cells(int,int)", i, j );//获取单元格

            //在r新行中添加子项数据
            if ( cell )
            {
                tableWidget->setItem(r,c,new QTableWidgetItem(cell->dynamicCall("Value2()").toString()));
            }
            delete cell;
        }
    }
    qDebug()<<"read one cost:"<<timer.elapsed();

    return true;
}

/**
  *@brief 清空除报表之外的数据
  */
void ExcelEngine::Clear()
{
    sXlsFile     = "";
    nRowCount    = 0;
    nColumnCount = 0;
    nStartRow    = 0;
    nStartColumn = 0;
}

/**
  *@brief 判断excel是否已被打开
  *@return true : 已打开
  *        false: 未打开
  */
bool ExcelEngine::IsOpen()
{
    return bIsOpen;
}

/**
  *@brief 判断excel COM对象是否调用成功，excel是否可用
  *@return true : 可用
  *        false: 不可用
  */
bool ExcelEngine::IsValid()
{
    return bIsValid;
}

/**
  *@brief 获取excel的行数
  */
UINT ExcelEngine::GetRowCount()const
{
    return nRowCount;
}

/**
  *@brief 获取excel的列数
  */
UINT ExcelEngine::GetColumnCount()const
{
    return nColumnCount;
}


/**
  *@brief 获取excel的数据
  */
QList<QList<QVariant> > ExcelEngine::GetExcelData()
{

    QVariant var;
    QList<QList<QVariant> > res;
    if (this->pWorksheet != NULL && ! this->pWorksheet->isNull())
    {
        QAxObject *usedRange = this->pWorksheet->querySubObject("UsedRange");
        if(NULL == usedRange || usedRange->isNull())
        {
            return res;
        }
        var = usedRange->dynamicCall("Value");
        delete usedRange;
    }
    QVariantList varRows = var.toList();
    if(varRows.isEmpty())
    {
        return res;
    }
    const int rowCount = varRows.size();
    QVariantList rowData;
    for(int i=0;i<rowCount;++i)
    {
        rowData = varRows[i].toList();
        res.push_back(rowData);
    }
    return res;
}
