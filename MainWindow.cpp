#include <QtWidgets/QHBoxLayout>
#include <Viewer/OsgViewerFromWindow.h>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "WidgetOpenGL/myOpenGL.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OsgViewerFromWindow::create();
    OsgViewerFromWindow::get().addListenerOfEventIntersector([=](const OsgViewerFromWindow::MODE_VIEWER &mode) {
        ui->setVisibleRightPanelComponents(OsgViewerFromWindow::MODE_OBJECT_VIEW == mode);
    });
    //setWindowFlags(0);
    //setWindowState(Qt::WindowState::WindowFullScreen);

    auto l = new QHBoxLayout();
    l->setMargin(0);
    ui->widgetVideo->setLayout(l);
    auto cam = new myOpenGL;
    l->addWidget(cam);

}

MainWindow::~MainWindow()
{
    delete ui;
}
