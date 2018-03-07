#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    randomEngine.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_generate_clicked()
{
    int i;

    setSettings();

    QString suffixpng = '_' + QString::number(ui->spinBox_size->value()) + 'x'
                         + QString::number(ui->spinBox_size->value()) + '_'
                         + QString::number(ui->spinBox_depth->value()) + ".png";
    QString suffixtxt= '_' + QString::number(ui->spinBox_size->value()) + 'x'
                         + QString::number(ui->spinBox_size->value()) + '_'
                         + QString::number(ui->spinBox_depth->value()) + ".txt";

    ui->progressBar->setRange(0,ui->spinBox_gen->value()-1);

    for(i=0;i<ui->spinBox_gen->value();++i)
    {
        genNewPixmap();

        QFile file(QString(QString::number(i+1)+suffixtxt));
        file.open(QIODevice::WriteOnly);
        QTextStream outstream(&file);
        outstream << qpixmap_to_qstring();
        file.close();

        pixmap->save(QString(QString::number(i+1)+suffixpng));
        ui->progressBar->setValue(i);
    }
}

void MainWindow::on_pushButton_preview_clicked()
{
    setSettings();

    genNewPixmap();

    ui->textEdit->setText(qpixmap_to_qstring());

    if(item != NULL)
        delete item;
    if(scene != NULL)
        delete scene;

    item = new QGraphicsPixmapItem(*pixmap);
    scene = new QGraphicsScene();
    scene->addItem(item);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->show();
}

void MainWindow::setSettings()
{
    int i, j, p;
    int size, depth;

    randomIntX.clear();
    randomIntY.clear();

    size    = ui->spinBox_size->value();
    depth   = ui->spinBox_depth->value();

    if(pen != NULL)
        delete pen;
    if(painter != NULL)
        delete painter;
    if(pixmap != NULL)
        delete pixmap;

    pixmap  = new QPixmap(size,size);
    painter = new QPainter(pixmap);
    pen     = new QPen(Qt::white);

    painter->setPen(*pen);

    std::uniform_int_distribution<int> tmpRandom;
    std::vector<std::uniform_int_distribution<int>> tmpRandomVector;

    for(i=0;i<=depth;++i)
    {
        tmpRandom = std::uniform_int_distribution<int>(i*(size/(depth+1)),
                                                      (i+1)*(size/(depth+1))-1);
        randomIntX.push_back(tmpRandom);

        if(i==0)
            p = 1;
        else if(i==1)
            p = 2;
        else
            p *= 2;

        tmpRandomVector.clear();

        for(j=0;j<p;j++)
        {
            tmpRandom = std::uniform_int_distribution<int>(j*(size/p),
                                                          (j+1)*(size/p)-1);
            tmpRandomVector.push_back(tmpRandom);
        }
        randomIntY.push_back(tmpRandomVector);
    }
}

void MainWindow::genNewPixmap()
{
    pixmap->fill(Qt::black);

    genNextLevel(QPoint((randomIntX.at(0)(randomEngine)),(randomIntY.at(0).at(0)(randomEngine))),
                 1,0);
}

void MainWindow::genNextLevel(QPoint p, int n, int m)
{
    // set line witdh
    pen->setWidth(2*((ui->spinBox_depth->value()-n+1)/ui->spinBox_depth->value()));
    painter->setPen(*pen);

    QPoint  p1((randomIntX[n](randomEngine)),randomIntY[n][m](randomEngine)),
            p2((randomIntX[n](randomEngine)),randomIntY[n][m+1](randomEngine));

    painter->drawLine(p,p1);
    painter->drawLine(p,p2);

    if(n < ui->spinBox_depth->value())
    {
        genNextLevel(p1,n+1,m*2);
        genNextLevel(p2,n+1,(m+1)*2);
    }
}

QString MainWindow::qpixmap_to_qstring()
{
    QString string;
    QImage tmpImg = pixmap->toImage();

    for(int j = 0; j < tmpImg.height(); j++)
    {
        for(int i = 0; i < tmpImg.width(); i++)
        {
            qGray(tmpImg.pixel(i,j)) == 0 ?
            string += '0' : string += '1';
        }
        string += '\n';
    }
    return string;
}
