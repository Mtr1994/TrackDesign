#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Public/threadpool.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

public slots:
    void slot_reload_map();
    void slot_export_data(const QString& data);

private slots:
    void on_btnLoad_clicked();

private:
    Ui::MainWindow *ui;

    ThreadPool mThreadPool;
};
#endif // MAINWINDOW_H
