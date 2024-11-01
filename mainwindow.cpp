//STL

//Native

//Qt
#include <QToolButton>

//Project
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "popupmenu.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIcon tmpIco(":/img/s2.png");

    popupMenu = new PopupMenu(ui->toolBar);

    for (int i = 0; i < 5; i++)
    {
        QToolButton *button = new QToolButton();
        if (i & 1)
            button->setIcon(tmpIco);

        popupMenu->addToolButton(button, [](){});
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
