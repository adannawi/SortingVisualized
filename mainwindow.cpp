#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Setup connects
    connect(ui->sortButton, SIGNAL(clicked()), this, SLOT(doSort()));
    connect(ui->algorithmList, SIGNAL(clicked(QModelIndex)), this, SLOT(updateAlgorithm()));
    connect(ui->columnAmount, SIGNAL(valueChanged(int)), this, SLOT(updateColumns()));

    //Initialize widget states
    ui->sortButton->setEnabled(0);
    ui->algorithmLabel->setText("None");

    //Let's set up the scene
    scene = new QGraphicsScene(this);
    ui->sortingBackground->setScene(scene);
    ui->sortingBackground->verticalScrollBar()->blockSignals(true);
    ui->sortingBackground->horizontalScrollBar()->blockSignals(true);

    //Let's find out the scene height and width for use later
    sceneHeight = ui->sortingBackground->height();
    sceneWidth = ui->sortingBackground->width();

    //Let's set up the columns!
    setupColumns(100);
    numColumns = 100;
}

void MainWindow::setupColumns(int n){
    // Set up column array
    columnWidth = sceneWidth/n;
    columns.resize(n);

    // Set up height list
    double colHeightDiv = sceneHeight/n;
    for(auto i = colHeightDiv; i < sceneHeight; i += colHeightDiv){
        columnHeights.push_back(i);
    }

    // Shuffle said height list
    std::random_shuffle(columnHeights.begin(), columnHeights.end());


    auto j = 0;
    auto k = 0.0;
    for (auto &p : columns){
        p = new QGraphicsRectItem;
        p->setRect(k, sceneHeight - columnHeights[j], columnWidth, columnHeights[j]);
        p->setBrush(QBrush(QColor(255, 0, 68, 255)));
        scene->addItem(p);
        j++;
        k+=columnWidth;
    }
}

void MainWindow::updateColumns(){
    for(auto &p : columns)
            scene->removeItem(p);
    columnHeights.clear();
    numColumns = ui->columnAmount->value();
    setupColumns(ui->columnAmount->value());
}

void MainWindow::doSort(){
    QListWidgetItem * item = ui->algorithmList->currentItem();
    if (item->text() == "Bubble Sort"){
        BubbleSort();
    }else if (item->text() == "Quick Sort"){
        QuickSort(0, numColumns-1);
    }else{
        ui->algorithmLabel->setText("Invalid Selection");
    }
     Sorted(numColumns);
}


/* SORTING ALGORITHMS */

void MainWindow::Sorted(int n){
    for(int i = 0; i < n; i++){
         columns[i]->setBrush(QBrush(QColor(0,200,100)));
         QCoreApplication::processEvents();
         QThread::msleep(1);
    }
}

void MainWindow::BubbleSort(){
    for (int c = 0; c < numColumns; c++){
        for(int d = 0; d < numColumns - c - 1; d++){
            if (columnHeights[d] > columnHeights[d+1]){
                // swap
                swapColumns(d, d+1);

                // Next line is needed because we're not threaded - makes sure to update graph as we go
                QCoreApplication::processEvents();
            }
        }
    }
}

void MainWindow::MergeSort(){

}


void MainWindow::QuickSort(int arrayBegin, int arrayEnd){
    if (arrayBegin < arrayEnd){
        int pi = partition(arrayBegin, arrayEnd);
        QuickSort(arrayBegin, pi - 1);
        QuickSort(pi + 1, arrayEnd);
    }
}

int MainWindow::partition(int arrayBegin, int arrayEnd){
    int pivot = columnHeights[arrayEnd];
    int i = (arrayBegin - 1);
    for (int j = arrayBegin; j <= arrayEnd - 1; j++){
        if (columnHeights[j] <= pivot){
            i++;
            swapColumns(i, j);
           QCoreApplication::processEvents();
        }
    }
    swapColumns(i+1, arrayEnd);
    QCoreApplication::processEvents();
    return (i + 1);
}


void MainWindow::updateAlgorithm(){
    ui->sortButton->setEnabled(1);
    ui->algorithmLabel->setText(ui->algorithmList->currentItem()->text());
}

void MainWindow::swapColumns(int n,int k){
    auto nRect = columns[n]->rect();
    auto kRect = columns[k]->rect();
    auto nColumnPos = nRect.left();
    auto kColumnPos = kRect.left();

    nRect.moveLeft(kColumnPos);
    kRect.moveLeft(nColumnPos);

    columns[n]->setRect(nRect);
    columns[k]->setRect(kRect);

    std::swap(columns[n], columns[k]);
    std::swap(columnHeights[n], columnHeights[k]);
    Beep((25000/numColumns)*n, 1);
    QThread::msleep(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
