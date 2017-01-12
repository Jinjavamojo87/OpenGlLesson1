#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "WidgetOpenGL/OpenGLWrapper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setWindowFlags(0);
    //setWindowState(Qt::WindowState::WindowFullScreen);

    auto l = new QHBoxLayout();
    l->setMargin(0);
    ui->widgetVideo->setLayout(l);
    //auto cam = new Camera(/*ui->toolBox*/);
    auto cam = new OpenGLWrapper;
    l->addWidget(cam);
}

MainWindow::~MainWindow()
{
    delete ui;
}
