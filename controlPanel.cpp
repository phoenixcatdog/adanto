#include "controlPanel.h"

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

controlPanel::controlPanel(QWidget *parent, image_info *info_p) :
    QDialog(parent = 0)
{
  //Camera parameters
  //Projector parameters
  //Set parameters
  //Preview project
  //Preview projector analysis
  //Calculate 3d view

  //Create all necessary data

  info = info_p;
  hor_list_model = NULL;
  ver_list_model = NULL;

  create_image_info(info);
  update_picture_data(info);
  update_picture_lists(info);

  //Create the UI

  Title = new QLabel("Calibration", this);
  Title->setAlignment(Qt::AlignCenter);

  QToolButton *left_arrow  = new QToolButton();
  left_arrow->setAutoRaise(true);
  left_arrow->setToolButtonStyle(Qt::ToolButtonIconOnly);
  left_arrow->setPopupMode(QToolButton::InstantPopup);
  //left_arrow->setIcon(QIcon("://icons/backward.png"));

  QString fileName("://icons/backward.png");
  if(QFile(fileName).exists() == false)
      QMessageBox::warning(this, "Application", "Wrong file name : " + fileName);

  QToolButton *right_arrow = new QToolButton();
  right_arrow->setAutoRaise(true);
  right_arrow->setToolButtonStyle(Qt::ToolButtonIconOnly);
  right_arrow->setPopupMode(QToolButton::InstantPopup);
  //right_arrow->setIcon(QIcon("://icons/forward.png"));

  QAction *left_arrow_action = new QAction(this);
  left_arrow_action->setIcon(QIcon("://icons/backward.png"));
  left_arrow_action->setText("Last Page");
  left_arrow_action->setStatusTip(tr("go to last step"));
  connect(left_arrow_action, SIGNAL(triggered(bool)), this, SLOT(change_last_page(bool)));

  QAction *right_arrow_action = new QAction(this);
  right_arrow_action->setIcon(QIcon("://icons/forward.png"));
  right_arrow_action->setText("Next Page");
  right_arrow_action->setStatusTip(tr("go to next step"));
  connect(right_arrow_action, SIGNAL(triggered(bool)), this, SLOT(change_next_page(bool)));

  left_arrow->setDefaultAction(left_arrow_action);
  right_arrow->setDefaultAction(right_arrow_action);

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
  connect(hor_ver_selector, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox_preview_changed(int)));

  ima_pre_selector = new QComboBox();
  ima_pre_selector->addItem("Preview");
  ima_pre_selector->addItem("Threshold");
  connect(ima_pre_selector, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox_indexchanged(int)));

  list_view = new QListView();
  list_view->setModel(hor_list_model);
  list_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(list_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(double_click_on_fileview(QModelIndex)));


  sweep_hor_ver_selector = new QComboBox();
  sweep_hor_ver_selector->addItem("Horizontal");
  sweep_hor_ver_selector->addItem("Vertical");

  QVBoxLayout *mlayout = new QVBoxLayout(this);
  QHBoxLayout *hlayout = new QHBoxLayout();

  //This is the top part that displays the current step in the entire process
  hlayout->addWidget(left_arrow);
  hlayout->addWidget(Title);
  hlayout->addWidget(right_arrow);

  mlayout->addLayout(hlayout);

  //We are going to create a stack of widgets pages,so we can the cycle between pages
  QWidget *first_page  = new QWidget;
  QWidget *second_page = new QWidget;

  stack_of_widgets = new QStackedWidget;
  stack_of_widgets->addWidget(first_page);
  stack_of_widgets->addWidget(second_page);

  //First page populating
  QVBoxLayout *m2layout = new QVBoxLayout(first_page);
  first_page->setLayout(m2layout);
  m2layout->addWidget(hor_slider);
  m2layout->addWidget(hor_ver_selector);
  m2layout->addWidget(ima_pre_selector);
  m2layout->addWidget(list_view);

  //First page populating
  QVBoxLayout *p2layout = new QVBoxLayout(second_page);
  first_page->setLayout(m2layout);
  p2layout->addWidget(pushy);
  p2layout->addWidget(sweep_slider);
  p2layout->addWidget(sweep_hor_ver_selector);
  p2layout->addWidget(pushy1);
  p2layout->addWidget(pushy2);
  p2layout->addWidget(pushy3);

  stack_of_widgets->setCurrentIndex(0);
  mlayout->addWidget(stack_of_widgets);


  setLayout(mlayout);
  setWindowTitle("control panel");
  show();
}

controlPanel::~controlPanel()
{
}

void controlPanel::create_image_info (image_info* info)
{
  info->info_flag = 0;

  info->level_h_max = 0;
  info->level_v_max = 0;

  info->selected_orientation = 0;
  info->preview_mode         = 0;
  info->folder = NULL;
  info->hor_pic_list = NULL;
  info->ver_pic_list = NULL;
  info->selected_list_item = 0;

  info->ima_loaded = NULL;

  return;
}

void controlPanel::update_picture_data (image_info* info)
{
  int                    i;
  int                max_h;
  int                max_v;
  QStringList *string_list;

  max_h = info->level_h_max;
  max_v = info->level_v_max;

  if ((info->hor_pic_list)!= NULL)
  {
    delete (info->hor_pic_list);
  }
  info->hor_pic_list = new QStringList();
  string_list = info->hor_pic_list;
  for ( i = 0; i < max_h ; i++ )
  {
    string_list->append(QString ("Horizontal image %1").arg(QString::number(i)));
  }

  if ((info->ver_pic_list)!= NULL)
  {
      delete (info->ver_pic_list);
  }
  info->ver_pic_list = new QStringList();
  string_list = info->ver_pic_list;
  for ( i = 0; i < max_h ; i++ )
  {
    string_list->append(QString ("Vertical image %1").arg(QString::number(i)));
  }

  return;
}

void controlPanel::update_picture_lists (image_info *info)
{
  QStringList *string_list;

  string_list = info->hor_pic_list;
  if (hor_list_model != NULL)
  {  delete (hor_list_model);}
  hor_list_model = new QStringListModel(*string_list);

  string_list = info->ver_pic_list;
  if (ver_list_model != NULL)
  {  delete (ver_list_model);}
  ver_list_model = new QStringListModel(*string_list);
}

void controlPanel::update_control_panel (image_info* info)
{

}

void controlPanel::update_list_view(image_info *info)
{
  update_picture_data(info);
  update_picture_lists(info);

  if (info->selected_orientation == 0)
  {
    list_view->setModel(hor_list_model);
  }
  else
  {
    list_view->setModel(ver_list_model);
  }
}



void controlPanel::change_next_page(bool checked)
{
  int index;

  index = stack_of_widgets->currentIndex();

  if (index < (3-1))
  {
    index++;
    stack_of_widgets->setCurrentIndex(index);
  }
  stack_of_widgets->show();
  return;
}

void controlPanel::change_last_page(bool checked)
{
  int index;

  index = stack_of_widgets->currentIndex();

  if (index > (0))
  {
    index--;
    stack_of_widgets->setCurrentIndex(index);
  }
  stack_of_widgets->show();
  return;
}

void controlPanel::combobox_indexchanged(int index)
{
  info->preview_mode = index;
  return;
}

void controlPanel::combobox_preview_changed(int index)
{
  if (info->no_textures)
  {return;}
  info->selected_orientation = index;
  if (index == 0)
  {
    if(info->selected_list_item > info->level_h_max)
    {
        info->selected_list_item = info->level_h_max;
    }
    list_view->setModel(hor_list_model);
  }
  else
  {
     if(info->selected_list_item > info->level_v_max)
    {
        info->selected_list_item = info->level_v_max;
    }
    list_view->setModel(ver_list_model);
  }
  emit display_new_file();

  return;
}

void controlPanel::double_click_on_fileview(QModelIndex index)
{
  if (info->no_textures)
  {return;}
  info->selected_list_item = index.row();
  emit display_new_file();

  return;
}

void controlPanel::closeEvent(QCloseEvent *event)
{
  emit closing_panel();

  return;
}
