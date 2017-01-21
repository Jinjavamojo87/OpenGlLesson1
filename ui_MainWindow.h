/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTableWidget>

#include <string>
#include <fstream>
#include <streambuf>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *logoImage;
    QLabel *logoLabel;
    QSpacerItem *logoSpacer;
    QToolButton *chooseCameraButton;
    QPushButton *closeButton;
    QWidget *body_widget;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widgetVideo;
    QWidget *rightPanel;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *rightPanelLayout;
    QTextBrowser *titleHtml;
    QToolBox *infoPanel;
    QWidget *page1;
    QVBoxLayout *verticalLayout_3;
    QTextBrowser *descriptionHtml;
    QWidget *page2;
    QVBoxLayout *verticalLayout_4;
    QTextBrowser *constructDetailsHtml;
    QWidget *page3;
    QVBoxLayout *verticalLayout_5;
    QTextBrowser *technicalFeaturesHtml;

    void setVisibleRightPanelComponents(bool v) {
        for (auto child : rightPanel->children()) {
            if (child->isWidgetType()) {
                QWidget *w = (QWidget *) child;
                w->setVisible(v);
            }
        }
    }

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1359, 725);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow\n"
"{\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                stop: 0 #363b5d, stop: 0.4 #3e4d59,\n"
"                                stop: 0.5 #537867, stop: 1.0 #76a27a);\n"
"}\n"
"\n"
"QToolBox::tab {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                stop: 0 #3c4b62, stop: 0.4 #3c4b62,\n"
"                                stop: 0.5 #3c4b62, stop: 1.0 #3c4b62);\n"
"    border-radius: 5px;\n"
"    color: white;\n"
"}\n"
"\n"
"QToolBox::tab:selected { /* italicize selected tabs */\n"
"    color: white;\n"
"}"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(0, 96));
        widget->setMaximumSize(QSize(16777215, 96));
        widget->setStyleSheet(QString::fromUtf8("background:white;"));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 6, 0);
        logoImage = new QLabel(widget);
        logoImage->setObjectName(QString::fromUtf8("logoImage"));
        logoImage->setPixmap(QPixmap(QString::fromUtf8(":/new/picture/RussiansFligt.png")));

        horizontalLayout_3->addWidget(logoImage);

        logoLabel = new QLabel(widget);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(logoLabel->sizePolicy().hasHeightForWidth());
        logoLabel->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(16);
        logoLabel->setFont(font);

        horizontalLayout_3->addWidget(logoLabel);

        logoSpacer = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(logoSpacer);

        chooseCameraButton = new QToolButton(widget);
        chooseCameraButton->setObjectName(QString::fromUtf8("chooseCameraButton"));

        horizontalLayout_3->addWidget(chooseCameraButton);

        closeButton = new QPushButton(widget);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));

        horizontalLayout_3->addWidget(closeButton);


        verticalLayout_2->addWidget(widget);

        body_widget = new QWidget(centralWidget);
        body_widget->setObjectName(QString::fromUtf8("body_widget"));
        horizontalLayout_4 = new QHBoxLayout(body_widget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        widgetVideo = new QWidget(body_widget);
        widgetVideo->setObjectName(QString::fromUtf8("widgetVideo"));
        widgetVideo->setStyleSheet(QString::fromUtf8("background:rgb(0, 0, 0)"));

        horizontalLayout_4->addWidget(widgetVideo);

        rightPanel = new QWidget(body_widget);
        rightPanel->setObjectName(QString::fromUtf8("rightPanel"));
        rightPanel->setMinimumSize(QSize(500, 0));
        rightPanel->setMaximumSize(QSize(500, 16777215));
        QPalette palette;
        QBrush brush(QColor(77, 85, 108, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        rightPanel->setPalette(palette);
        rightPanel->setStyleSheet(QString::fromUtf8("background:rgb(77, 85, 108)"));
        horizontalLayout = new QHBoxLayout(rightPanel);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        rightPanelLayout = new QVBoxLayout();
        rightPanelLayout->setSpacing(15);
        rightPanelLayout->setObjectName(QString::fromUtf8("rightPanelLayout"));
        rightPanelLayout->setContentsMargins(3, 3, 3, 3);
        titleHtml = new QTextBrowser(rightPanel);
        titleHtml->setObjectName(QString::fromUtf8("titleHtml"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(titleHtml->sizePolicy().hasHeightForWidth());
        titleHtml->setSizePolicy(sizePolicy2);
        titleHtml->setMinimumSize(QSize(0, 0));
        titleHtml->setMaximumSize(QSize(16777215, 150));
        titleHtml->setAcceptRichText(true);

        rightPanelLayout->addWidget(titleHtml);

        infoPanel = new QToolBox(rightPanel);
        infoPanel->setObjectName(QString::fromUtf8("infoPanel"));
        infoPanel->setMaximumSize(QSize(16777215, 16777215));
        page1 = new QWidget();
        page1->setObjectName(QString::fromUtf8("page1"));
        page1->setGeometry(QRect(0, 0, 476, 317));
        verticalLayout_3 = new QVBoxLayout(page1);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 0, -1, -1);
        descriptionHtml = new QTextBrowser(page1);
        descriptionHtml->setObjectName(QString::fromUtf8("descriptionHtml"));

        verticalLayout_3->addWidget(descriptionHtml);

        infoPanel->addItem(page1, QString::fromUtf8("\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265"));
        page2 = new QWidget();
        page2->setObjectName(QString::fromUtf8("page2"));
        page2->setGeometry(QRect(0, 0, 476, 317));
        verticalLayout_4 = new QVBoxLayout(page2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(-1, 0, -1, -1);
        constructDetailsHtml = new QTextBrowser(page2);
        constructDetailsHtml->setObjectName(QString::fromUtf8("constructDetailsHtml"));

        verticalLayout_4->addWidget(constructDetailsHtml);

        infoPanel->addItem(page2, QString::fromUtf8("\320\232\320\276\320\275\321\201\321\202\321\200\321\203\320\272\321\202\320\270\320\262\320\275\321\213\320\265 \320\276\321\201\320\276\320\261\320\265\320\275\320\275\320\276\321\201\321\202\320\270"));
        page3 = new QWidget();
        page3->setObjectName(QString::fromUtf8("page3"));
        page3->setGeometry(QRect(0, 0, 476, 317));
        verticalLayout_5 = new QVBoxLayout(page3);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(-1, 0, -1, -1);
        technicalFeaturesHtml = new QTextBrowser(page3);
        technicalFeaturesHtml->setObjectName(QString::fromUtf8("technicalFeaturesHtml"));

        verticalLayout_5->addWidget(technicalFeaturesHtml);

        infoPanel->addItem(page3, QString::fromUtf8("\320\242\320\265\321\205\320\275\320\270\321\207\320\265\321\201\320\272\320\270\320\265 \321\205\320\260\321\200\320\260\320\272\321\202\320\265\321\200\320\270\321\201\321\202\320\270\320\272\320\270"));

        rightPanelLayout->addWidget(infoPanel);


        horizontalLayout->addLayout(rightPanelLayout);


        horizontalLayout_4->addWidget(rightPanel);


        verticalLayout_2->addWidget(body_widget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);
        QObject::connect(closeButton, SIGNAL(clicked()), MainWindow, SLOT(close()));

        infoPanel->setCurrentIndex(2);
        infoPanel->layout()->setSpacing(10);


        QMetaObject::connectSlotsByName(MainWindow);
        setVisibleRightPanelComponents(false);
    } // setupUi

    QString readFromFile(const std::string fileName) {
        std::ifstream t(fileName);
        std::string result((std::istreambuf_iterator<char>(t)),
                                        std::istreambuf_iterator<char>());
        return QString(result.c_str());
    }

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, 0));
        logoImage->setText(QString());
        logoLabel->setText(QApplication::translate("MainWindow", "\320\241\320\230\320\241\320\242\320\225\320\234\320\220 \320\237\320\236\320\224\320\224\320\225\320\240\320\226\320\232\320\230 \320\240\320\225\320\234\320\236\320\235\320\242\320\235\320\253\320\245 \320\240\320\220\320\221\320\236\320\242", 0, 0));
        chooseCameraButton->setText(QApplication::translate("MainWindow", "...", 0, 0));
        closeButton->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", 0, 0));
        infoPanel->setItemText(infoPanel->indexOf(page1), QApplication::translate("MainWindow", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265", 0, 0));
        constructDetailsHtml->setHtml(readFromFile("html/constructDetails.html"));
        infoPanel->setItemText(infoPanel->indexOf(page2), QApplication::translate("MainWindow", "\320\232\320\276\320\275\321\201\321\202\321\200\321\203\320\272\321\202\320\270\320\262\320\275\321\213\320\265 \320\276\321\201\320\276\320\261\320\265\320\275\320\275\320\276\321\201\321\202\320\270", 0, 0));
        technicalFeaturesHtml->setHtml(readFromFile("html/technicalFeatures.html"));
        infoPanel->setItemText(infoPanel->indexOf(page3), QApplication::translate("MainWindow", "\320\242\320\265\321\205\320\275\320\270\321\207\320\265\321\201\320\272\320\270\320\265 \321\205\320\260\321\200\320\260\320\272\321\202\320\265\321\200\320\270\321\201\321\202\320\270\320\272\320\270", 0, 0));
        descriptionHtml->setHtml(readFromFile("html/description.html"));
        titleHtml->setHtml(readFromFile("html/modelNameTitle.html"));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
