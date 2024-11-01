#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//STL

//Native

//Qt
#include <QMainWindow>

//Project

class PopupMenu;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    PopupMenu* popupMenu;
};

#endif // MAINWINDOW_H
