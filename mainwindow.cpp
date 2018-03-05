#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pixmap  = new QPixmap(32,32);
    painter = new QPainter(pixmap);
    pen     = new QPen(Qt::white);

    painter->setPen(*pen);

    randomEngine.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_generate_clicked()
{
    setSettings();
}

void MainWindow::on_pushButton_preview_clicked()
{
    setSettings();
    genNewPixmap()
}

void MainWindow::setSettings()
{
    int i, j, p;
    int size, depth;

    randomIntX.clear();
    randomIntY.clear();

    size    = ui->spinBox_size->value();
    depth   = ui->spinBox_depth->value();

    std::uniform_int_distribution<int> tmpRandom;

    for(i=0;i<=depth;++i)
    {
        tmpRandom = std::uniform_int_distribution<int>(i*(size/(depth+1)),
                                                      (i+1)*(size/(depth+1)));
        randomIntX.push_back(tmpRandom);

        if(i==0)
            p = 1;
        else if(i==1)
            p = 2;
        else
            p = std::pow(i,2);

        for(j=0;j<p;j++)
        {
            tmpRandom = std::uniform_int_distribution<int>(j*(size/p),
                                                          (j+1)*(size/p));
            randomIntY.push_back(tmpRandom);
        }
    }
}

void MainWindow::genNewPixmap()
{

}
