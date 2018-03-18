#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QFile>
#include <QTextStream>

#include <QtDebug>

#include <algorithm>
#include <random>
#include <functional>
#include <cmath>

#include "kruskal.h"

/**
 * @brief The Mode enum
 */
enum Mode { BT2D = 0,
            MST2D = 1
          };

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
    /**
     * @brief on_pushButton_generate_clicked
     * Generate a new dataset
     */
    void on_pushButton_generate_clicked();
    /**
     * @brief on_pushButton_preview_clicked
     * Generate a preview
     */
    void on_pushButton_preview_clicked();
    /**
     * @brief on_comboBox_model_currentIndexChanged
     * @param index
     * Used to update the enabled and disabled widget
     */
    void on_comboBox_model_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    /**
     * @brief getUserInterfaceSettings get the
     * settings from the user interface
     */
    void getUserInterfaceSettings();

    /**
     * @brief set_PaintDevices
     */
    void set_PaintDevices();
    /**
     * @brief set_Graphics
     */
    void set_Graphics();

    // BT2D
    /**
     * @brief init_BT2D
     */
    void init_BT2D();
    /**
     * @brief genBT2D generate a new binary tree
     * in pixmap
     */
    void genBT2D();
    /**
     * @brief genBT2DNextLevel generate the next tree level
     * @param p previous point
     * @param n depth level
     * @param m offset
     */
    void genBT2DNextLevel(QPoint p, int n, int m);

    /**
     * @brief init_MST2D
     */
    void init_MST2D();
    /**
     * @brief genMST2D generate a new minimum
     * spanning tree in pixmap
     */
    void genMST2D();

    // Random
    std::default_random_engine                      randomEngine;
    // BT2D
    std::vector<std::uniform_int_distribution<int>> BT2D_randomIntX;
    std::vector<std::vector<
    std::uniform_int_distribution<int>>>            BT2D_randomIntY;
    // MST2D
    // Uniform
    std::uniform_int_distribution<int> MST2D_randomIntX;
    std::uniform_int_distribution<int> MST2D_randomIntY;

    // Paint Devices
    QPixmap     *pixmap     = NULL;
    QPainter    *painter    = NULL;
    QPen        *pen        = NULL;

    // Graphics
    QGraphicsPixmapItem *item   = NULL;
    QGraphicsScene *scene       = NULL;

    // User Settings (GUI)
    Mode user_mode      = BT2D;
    int size_x          = 32;
    int size_y          = 32;
    int size_z          = 32;
    int tree_depth      = 2;
    int number_point    = 16;
    int generation      = 10;
};

#endif // MAINWINDOW_H
