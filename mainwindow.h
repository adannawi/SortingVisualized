#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets>
#include <QThread>
#include <cstdlib>
#include "windows.h"
#include <math.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    void setupColumns(int);
    void BubbleSort();
    void MergeSort();
    void QuickSort(int, int);
    void swapColumns(int,int);
    void Sorted(int);
    int partition(int, int);
    double sceneWidth;
    double sceneHeight;
    double columnWidth;
    int numColumns;
    std::vector<QGraphicsRectItem*> columns;
    std::vector<double> columnHeights;
private slots:
    void doSort();
    void updateAlgorithm();
    void updateColumns();
};

#endif // MAINWINDOW_H
