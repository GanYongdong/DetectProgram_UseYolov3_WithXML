#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "jj_obj_detect.h"

namespace Ui {
class MainWindow;
}

std::vector<std::string> s_split(const std::string& in, const std::string& delim);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MixDetector detector;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
