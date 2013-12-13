#include "controlPanel.h"

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QStackedWidget>
#include <QSlider>
#include <QComboBox>
#include <QListWidget>

#include <QFile>
#include <QMessageBox>

controlPanel::controlPanel(QWidget *parent) :
    QDialog(parent)
{
  Title = new QLabel("Calibration", this);
  Title->setAlignment(Qt::AlignCenter);

  QToolButton *left_arrow  = new QToolButton();
  left_arrow->setAutoRaise(true);
  left_arrow->setToolButtonStyle(Qt::ToolButtonIconOnly);
  left_arrow->setIcon(QIcon("://icons/backward.png"));

  QString fileName("://icons/backward.png");
  if(QFile(fileName).exists() == false)
      QMessageBox::warning(this, "Application", "Wrong file name : " + fileName);

  QToolButton *right_arrow = new QToolButton();
  right_arrow->setAutoRaise(true);
  right_arrow->setToolButtonStyle(Qt::ToolButtonIconOnly);
  right_arrow->setIcon(QIcon("://icons/forward.png"));

  QPushButton *pushy       = new QPushButton("hello");

  QPushButton *pushy1      = new QPushButton("hello1");
  QPushButton *pushy2      = new QPushButton("hello2");
  QPushButton *pushy3      = new QPushButton("hello3");

  QComboBox   *hor_ver_selector;
  QComboBox   *ima_pre_selector;

  QComboBox   *sweep_hor_ver_selector;

  hor_slider = new QSlider(Qt::Horizontal);
  hor_slider->setRange(0,1024);
  hor_slider->setValue(528);

  sweep_slider = new QSlider(Qt::Horizontal);
  sweep_slider->setRange(0,65536);
  sweep_slider->setValue(0);

  hor_ver_selector = new QComboBox();
  hor_ver_selector->addItem("Horizontal");
  hor_ver_selector->addItem("Vertical");

  ima_pre_selector = new QComboBox();
  ima_pre_selector->addItem("Original");
  ima_pre_selector->addItem("Prreview");

  QListWidget *list_widget = new QListWidget();
  list_widget->addItem("H00");
  list_widget->addItem("H01");
  list_widget->addItem("H02");
  list_widget->addItem("H03");
  list_widget->addItem("H04");
  list_widget->addItem("H05");
  list_widget->addItem("H06");
  list_widget->addItem("H07");
  list_widget->addItem("H08");
  list_widget->addItem("H09");

  sweep_hor_ver_selector = new QComboBox();
  sweep_hor_ver_selector->addItem("Horizontal");
  sweep_hor_ver_selector->addItem("Vertical");

  QVBoxLayout *mlayout = new QVBoxLayout(this);
  QHBoxLayout *hlayout = new QHBoxLayout(this);

  //This is the top part that displays the current step in the entire process
  hlayout->addWidget(left_arrow);
  hlayout->addWidget(Title);
  hlayout->addWidget(right_arrow);

  mlayout->addLayout(hlayout);

  //We are going to create a stack of widgets pages,so we can the cycle between pages
  QWidget *first_page  = new QWidget;
  QWidget *second_page = new QWidget;

  QStackedWidget *stack_of_widgets = new QStackedWidget;
  stack_of_widgets->addWidget(first_page);
  stack_of_widgets->addWidget(second_page);

  //First page populating
  QVBoxLayout *m2layout = new QVBoxLayout(first_page);
  first_page->setLayout(m2layout);
  m2layout->addWidget(hor_slider);
  m2layout->addWidget(hor_ver_selector);
  m2layout->addWidget(ima_pre_selector);
  m2layout->addWidget(list_widget);

  //First page populating
  QVBoxLayout *p2layout = new QVBoxLayout(second_page);
  first_page->setLayout(m2layout);
  p2layout->addWidget(pushy);
  p2layout->addWidget(sweep_slider);
  p2layout->addWidget(sweep_hor_ver_selector);
  p2layout->addWidget(pushy1);
  p2layout->addWidget(pushy2);
  p2layout->addWidget(pushy3);

  mlayout->addWidget(stack_of_widgets);

  setLayout(mlayout);
  setWindowTitle("control panel");
  show();
}

controlPanel::~controlPanel()
{
}
