#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    havePages = 0;
    QCoreApplication::setOrganizationName("Laudenberg");
    QCoreApplication::setOrganizationDomain("laudenberg.org");
    QCoreApplication::setApplicationName("Pages Per Day");
    setWindowTitle("Pages Per Day");

    if (!settings.value("deadline").isNull()) {
        ui->deadlineEdit->setDate(settings.value("deadline").toDate());
    } else {
        ui->deadlineEdit->setDate(QDate::currentDate());
    }

    if (!settings.value("needPages").isNull()) {
        ui->needPagesEdit->setValue(settings.value("needPages").toInt());
    }

    if (!settings.value("fileName").isNull()) {
        ui->fileNameEdit->setText(settings.value("fileName").toString());
        countPages();
    }

    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    connect(ui->deadlineEdit, SIGNAL(dateChanged(QDate)), this, SLOT(calculate()));
    connect(ui->needPagesEdit, SIGNAL(valueChanged(int)), this, SLOT(calculate()));
    connect(&reloadPdfTimer, SIGNAL(timeout()), this, SLOT(countPages()));
    reloadPdfTimer.start(1000 * 60);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("PDF-Datei"), QString(), tr("PDF (*.pdf)"));
    ui->fileNameEdit->setText(fileName);
    settings.setValue("fileName", fileName);
    countPages();
}

void MainWindow::countPages()
{
    QFile file(ui->fileNameEdit->text());
    havePages = 0;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();
        while (!line.isNull()) {
            havePages += line.count(QRegExp("/Type\\s+/Page($|[^s])"));
            line = in.readLine();
        }
        ui->fileNameEdit->setStyleSheet("color: black");
    } else {
        ui->fileNameEdit->setStyleSheet("color: red");
    }

    calculate();
}

void MainWindow::calculate()
{
    ui->havePagesLabel->setText(QString::number(havePages));
    settings.setValue("deadline", ui->deadlineEdit->date());
    settings.setValue("needPages", ui->needPagesEdit->value());

    int days = QDate::currentDate().daysTo(ui->deadlineEdit->date());
    int pages = ui->needPagesEdit->value() - havePages;

    if (days == 0)
        days = 1;

    ui->resultLabel->setText(QString::number((double) pages / days, 'g', 2));
}

