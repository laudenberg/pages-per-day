#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTimer>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void openFileDialog();
    void calculate();
    void countPages();

private:
    Ui::MainWindow *ui;
    int havePages;
    QSettings settings;
    QTimer reloadPdfTimer;
};

#endif // MAINWINDOW_H
