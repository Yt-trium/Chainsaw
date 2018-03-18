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
    QString mode;

    // User settings
    getUserInterfaceSettings();

    // Reset the Paint Devices
    set_PaintDevices();

    // Generation
    switch (user_mode)
    {
    case BT2D:
        mode = "BT2D";
        init_BT2D();
        break;
    case MST2D:
        mode = "MST2D";
        init_MST2D();
        break;
    default:
        break;
    }

    QString suffixpng = '_' + mode + '_'
            + QString::number(ui->spinBox_size_x->value()) + 'x'
            + QString::number(ui->spinBox_size_y->value()) + '_'
            + QString::number(ui->spinBox_depth->value()) + ".png";

    ui->progressBar->setRange(0,ui->spinBox_gen->value()-1);

    for(i=0;i<ui->spinBox_gen->value();++i)
    {
        switch (user_mode)
        {
        case BT2D:
            genBT2D();
            break;
        case MST2D:
            genMST2D();
            break;
        default:
            break;
        }

        pixmap->save(QString(QString::number(i+1)+suffixpng));
        ui->progressBar->setValue(i);
    }
}

void MainWindow::on_pushButton_preview_clicked()
{
    // User settings
    getUserInterfaceSettings();

    // Reset the Paint Devices
    set_PaintDevices();

    // Generation
    switch (user_mode)
    {
    case BT2D:
        init_BT2D();
        genBT2D();
        break;
    case MST2D:
        init_MST2D();
        genMST2D();
        break;
    default:
        break;
    }

    // Reset the Graphics
    set_Graphics();
}

void MainWindow::on_comboBox_model_currentIndexChanged(int index)
{
    switch ((Mode)index)
    {
    case BT2D:
        ui->spinBox_depth->setEnabled(true);
        ui->spinBox_number_point->setEnabled(false);
        ui->spinBox_size_z->setEnabled(false);
        ui->comboBox_points_distribution->setEnabled(false);
        break;
    case MST2D:
        ui->spinBox_depth->setEnabled(false);
        ui->spinBox_number_point->setEnabled(true);
        ui->spinBox_size_z->setEnabled(false);
        ui->comboBox_points_distribution->setEnabled(true);
        break;
    default:
        break;
    }
}

void MainWindow::getUserInterfaceSettings()
{
    user_mode       = (Mode)ui->comboBox_model->currentIndex();
    size_x          = ui->spinBox_size_x->value();
    size_y          = ui->spinBox_size_y->value();
    size_z          = ui->spinBox_size_z->value();
    tree_depth      = ui->spinBox_depth->value();
    number_point    = ui->spinBox_number_point->value();
    generation      = ui->spinBox_gen->value();
    user_distribution = (Distribution)ui->comboBox_points_distribution->currentIndex();
}

void MainWindow::set_PaintDevices()
{
    if(pen != NULL)
        delete pen;
    if(painter != NULL)
        delete painter;
    if(pixmap != NULL)
        delete pixmap;

    pixmap  = new QPixmap(size_x,size_y);
    painter = new QPainter(pixmap);
    pen     = new QPen(Qt::white);

    painter->setPen(*pen);
}

void MainWindow::set_Graphics()
{
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

void MainWindow::init_BT2D()
{
    int i, j, p;

    BT2D_randomIntX.clear();
    BT2D_randomIntY.clear();

    std::uniform_int_distribution<int> tmpRandom;
    std::vector<std::uniform_int_distribution<int>> tmpRandomVector;

    for(i=0;i<=tree_depth;++i)
    {
        tmpRandom = std::uniform_int_distribution<int>(i*(size_x/(tree_depth+1)),
                                                      (i+1)*(size_x/(tree_depth+1))-1);
        BT2D_randomIntX.push_back(tmpRandom);

        if(i==0)
            p = 1;
        else if(i==1)
            p = 2;
        else
            p *= 2;

        tmpRandomVector.clear();

        for(j=0;j<p;j++)
        {
            tmpRandom = std::uniform_int_distribution<int>(j*(size_y/p),
                                                          (j+1)*(size_y/p)-1);
            tmpRandomVector.push_back(tmpRandom);
        }
        BT2D_randomIntY.push_back(tmpRandomVector);
    }
}

void MainWindow::genBT2D()
{
    pixmap->fill(Qt::black);

    genBT2DNextLevel(QPoint((BT2D_randomIntX.at(0)(randomEngine)),(BT2D_randomIntY.at(0).at(0)(randomEngine))),
                 1,0);
}

void MainWindow::genBT2DNextLevel(QPoint p, int n, int m)
{
    // set line witdh
    pen->setWidth(2*((ui->spinBox_depth->value()-n+1)/ui->spinBox_depth->value()));
    painter->setPen(*pen);

    QPoint  p1((BT2D_randomIntX[n](randomEngine)),BT2D_randomIntY[n][m](randomEngine)),
            p2((BT2D_randomIntX[n](randomEngine)),BT2D_randomIntY[n][m+1](randomEngine));

    painter->drawLine(p,p1);
    painter->drawLine(p,p2);

    if(n < ui->spinBox_depth->value())
    {
        genBT2DNextLevel(p1,n+1,m*2);
        genBT2DNextLevel(p2,n+1,(m+1)*2);
    }
}

void MainWindow::init_MST2D()
{
    MST2D_randomIntX = std::uniform_int_distribution<int>(0,size_x-1);
    MST2D_randomIntY = std::uniform_int_distribution<int>(0,size_y-1);

    MST2D_normal_randomIntX = std::normal_distribution<>(size_x*0.5,(size_x*0.125));
    MST2D_normal_randomIntY = std::normal_distribution<>(size_y*0.5,(size_y*0.125));
}

void MainWindow::genMST2D()
{
    int i,x,y;

    pixmap->fill(Qt::black);

    // Generate random points
    std::vector<QPoint> points;

    for(i=0;i<number_point;++i)
    {
        switch((Distribution)user_distribution)
        {
        case Uniform:
            points.push_back(QPoint(MST2D_randomIntX(randomEngine),MST2D_randomIntY(randomEngine)));
            break;
        case Normal:
            x = MST2D_normal_randomIntX(randomEngine);
            y = MST2D_normal_randomIntY(randomEngine);
            points.push_back(QPoint(std::max(std::min(x,size_x-1),0),
                                    std::max(std::min(y,size_y-1),0)));
            break;
        default:
            break;
        }
    }

    Kruskal k(points);

    std::vector<QLine> tmp = k.kruskalMST();

    for(std::size_t i = 0; i < tmp.size();i++)
    {
        pen->setWidth(0.1*((size_x+size_y)/2)/(number_point*0.1));
        painter->setPen(*pen);

        painter->drawLine(tmp.at(i));
    }

    /* Experimental code for pen width (not working)
     *
    QPoint center(size_x/2,size_y/2);
    int average_size = (size_x+size_y)/2;

    std::function<int(QPoint,QLine)> distance = [](QPoint p, QLine l)
    {
        // QT 5.8
        return sqrt(pow(p.x()-l.center().x(),2)+pow(p.y()-l.center().y(),2));

        return (sqrt(pow(p.x()-l.x1(),2)+pow(p.y()-l.y1(),2))
               +sqrt(pow(p.x()-l.x2(),2)+pow(p.y()-l.y1(),2)))/2;
    };

    std::function<int(int,int)> min = [](int a, int b)
    {
        return (a > b) ? b : a;
    };
    qDebug() << log(1+min((0.1*(average_size)),
                ((0.1*(average_size))+1)/(0.1*distance(center, tmp.at(i)))));
    */
}
