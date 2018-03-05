#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>

#include <algorithm>
#include <random>

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

    /**
     * @brief setSettings load the settings from the user
     * interface and allocate the drawing objects
     */
    void setSettings();

    /**
     * @brief genNewPixmap generate a new image in pixmap
     */
    void genNewPixmap();

    /**
     * @brief genNextLevel generate the next tree level
     * @param p previous point
     * @param n depth level
     * @param m offset
     */
    void genNextLevel(QPoint p, int n, int m);

    // Random
    std::default_random_engine                      randomEngine;
    std::vector<std::uniform_int_distribution<int>> randomIntX;
    std::vector<std::vector<
    std::uniform_int_distribution<int>>>            randomIntY;

    // Draw
    QPixmap     *pixmap     = NULL;
    QPainter    *painter    = NULL;
    QPen        *pen        = NULL;

    // QGraphicsView
    QGraphicsPixmapItem *item   = NULL;
    QGraphicsScene *scene       = NULL;
};

#endif // MAINWINDOW_H
