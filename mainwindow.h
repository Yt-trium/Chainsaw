#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <algorithm>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_generate_clicked();

    void on_pushButton_preview_clicked();

private:
    Ui::MainWindow *ui;

    void setSettings();
    void genNewPixmap();

    std::default_random_engine                          randomEngine;
    std::vector<std::uniform_int_distribution<int>>     randomIntX;
    std::vector<std::uniform_int_distribution<int>>     randomIntY;

    QPixmap     *pixmap;
    QPainter    *painter;
    QPen        *pen;
};

#endif // MAINWINDOW_H
