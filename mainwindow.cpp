#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Parser/dataparser.h"
#include "Utils/dataexport.h"

#include <QScreen>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化窗口
    init();

    setWindowTitle("轨迹规划工具");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    QRect rect = QGuiApplication::screens().at(0)->availableGeometry();
    resize(rect.width() * 0.6, rect.height() * 0.72);

    ui->widgetViewContainer->setVisible(false);

    // 更新数据
    connect(DataParser::getInstance(), &DataParser::sgl_read_poly_data_finish, ui->widgetView, &WidgetOpenGL::slot_update_data);

    // 重新加载地形
    connect(ui->widgetView, &WidgetOpenGL::sgl_reload_map, this, &MainWindow::slot_reload_map);

    // 导出数据
    connect(ui->widgetView, &WidgetOpenGL::sgl_export_data, this, &MainWindow::slot_export_data);
}

void MainWindow::slot_reload_map()
{
    ui->widgetViewContainer->setVisible(false);
    ui->widgetStartup->setVisible(true);
}

void MainWindow::slot_export_data(const QString &data)
{
    QString filter = "文本文件( *.txt)";
    QString path = QFileDialog::getSaveFileName(this, tr("导出文件"), "C:/", tr(filter.toStdString().data()));
    if (path.isEmpty()) return;

    auto func = std::bind(&DataExport::exportData, DataExport::getInstance(), std::placeholders::_1, std::placeholders::_2);
    mThreadPool.enqueue(func, path, data);
}

void MainWindow::on_btnLoad_clicked()
{
    // 版本支持读取 栅格文件 和 VTK支持的格式文件
    QString filter;
    QString path = QFileDialog::getOpenFileName(this, "选择文件", "C:/", "支持的文件(*.hgt *.grd *.tif *.xyz);;"
                                                                     "栅格文件(*.hgt *.grd *.tif *.xyz);;", &filter);
    if (path.isEmpty()) return;

    auto func = std::bind(&DataParser::parse, DataParser::getInstance(), std::placeholders::_1, std::placeholders::_2);
    mThreadPool.enqueue(func, path, filter);

    ui->widgetStartup->setVisible(false);
    ui->widgetViewContainer->setVisible(true);
}
