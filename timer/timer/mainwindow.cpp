#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <time.h>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>

void cambiarA0(int ** matriz)
{
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {

            if (matriz[i][j] == -1) matriz[i][j] =0;

        }
    }
}

int countAll0(int ** matriz)
{
    int acu = 0;
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {

            if (matriz[i][j] == 0) acu++;

        }
    }

    return acu;
}

int count0(int x,int y, int ** matriz)
{
    if (x >= 0 && x < 50 && y >= 0 && y < 50) {
        if(matriz[x][y] == 0)
        {
            matriz[x][y] = -1;
            return 1 + count0(x+1,y, matriz)
                    + count0(x-1,y, matriz)
                    + count0(x,y+1, matriz)
                    + count0(x,y-1, matriz);
        }
    }
    return 0;
}

int construirPuertaX(int proximaPuertaX, int proximaPuertaY, int ** matriz, bool &derecha){
    if (derecha) {
        while (proximaPuertaX < 50) {
            if (matriz[proximaPuertaX][proximaPuertaY] == 1) {
                matriz[proximaPuertaX][proximaPuertaY] = 0;
                return proximaPuertaX;
            }
            proximaPuertaX++;
        }
        derecha = false;
        return proximaPuertaX -1;
    } else {
        while (proximaPuertaX >= 0) {
            if (matriz[proximaPuertaX][proximaPuertaY] == 1) {
                matriz[proximaPuertaX][proximaPuertaY] = 0;
                return proximaPuertaX;
            }
            proximaPuertaX--;
        }
        derecha = true;
        return proximaPuertaX + 1;
    }
    return 0;
}

int construirPuertaY(int proximaPuertaX, int proximaPuertaY, int ** matriz, bool &abajo){
    if (abajo) {
        while (proximaPuertaY < 50) {
            if (matriz[proximaPuertaX][proximaPuertaY] == 1) {
                matriz[proximaPuertaX][proximaPuertaY] = 0;
                return proximaPuertaY;
            }
            proximaPuertaY++;
        }
        abajo = false;
        return proximaPuertaY -1;
    } else {
        while (proximaPuertaY >= 0) {
            if (matriz[proximaPuertaX][proximaPuertaY] == 1) {
                matriz[proximaPuertaX][proximaPuertaY] = 0;
                return proximaPuertaY;
            }
            proximaPuertaY--;
        }
        abajo = true;
        return proximaPuertaY + 1;
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int ** matriz = new int*[50];
    for (int i = 0; i < 50; i++) {
        matriz[i] = new int[50];
        for (int j = 0; j < 50; j++) {

            matriz[i][j] = 0;

        }
    }

    int i = 0;

    srand(time(NULL));
    while (i <= 8) {
        int r = rand() % 4  + 1 + 5 * i;
        int c = rand() % 4 + 1 + 5 * i;

        for (int j = 0; j < 50; j++){
            matriz[r][j] = 1;
        }

        for (int j = 0; j < 50; j++){
            matriz[j][c] = 1;
        }
        i++;
    }

    int zerosArea = count0(0, 0, matriz);
    cambiarA0(matriz);
    int Allzeros = countAll0(matriz);

    int proximaPuertaX = 0;
    int proximaPuertaY = 0;

    bool derecha = true;
    bool abajo = true;

    bool enX = rand() % 2;

    while (zerosArea < (Allzeros)) {

        if (enX) {
            proximaPuertaX = construirPuertaX(proximaPuertaX, proximaPuertaY, matriz, derecha);
            enX = rand() % 2;

            if (derecha) {
                if (proximaPuertaX < 48) proximaPuertaX++;
            } else {
                if (proximaPuertaX > 0) proximaPuertaX--;
            }

        } else {
            proximaPuertaY = construirPuertaY(proximaPuertaX, proximaPuertaY, matriz, abajo);
            enX = rand() % 2;

            if (derecha) {
                if (proximaPuertaY < 48) proximaPuertaY++;
            } else {
                if (proximaPuertaY > 0) proximaPuertaY--;
            }
        }

        zerosArea = count0(0, 0, matriz);
        cambiarA0(matriz);
        Allzeros = countAll0(matriz);
    }

    this->labels = new QLabel**[50];
    for (int i = 0; i < 50; i++) {
        this->labels[i] = new QLabel*[50];
        for (int j = 0; j < 50; j++) {

            if (matriz[i][j] == 0) {
                this->labels[i][j] = new QLabel();
                this->labels[i][j]->setBackgroundRole(QPalette::Dark);
                this->labels[i][j]->setScaledContents(true);

//                QPixmap pix("/home/emanuel/Escritorio/piso.png");
//                myimage->setPixmap(pix);

                this->ui->gridLayout->addWidget(labels[i][j] ,i,j);
            } else {
                this->labels[i][j] = new QLabel();
                this->labels[i][j]->setBackgroundRole(QPalette::Dark);
                this->labels[i][j]->setScaledContents(true);

                QPixmap pix("/home/emanuel/Escritorio/wall2.png");
                this->labels[i][j]->setPixmap(pix);

                this->ui->gridLayout->addWidget(this->labels[i][j] ,i,j);
            }

        }

    }

    //this->showFullScreen();
    QLabel *myimage = this->labels[0][0];
    QPixmap pix("/home/emanuel/Escritorio/personaje.png");
    myimage->setPixmap(pix);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(dibujarArtefacto()));
    timer->start(10000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dibujarArtefacto() {
    QMessageBox::warning(this, tr("Hola!!"),
                                       "Hola!!",
                                       QMessageBox::Ok  ,
                                       QMessageBox::Ok);


}
