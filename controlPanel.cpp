#include "controlPanel.h"

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

controlPanel::controlPanel(QWidget *parent, image_info *info_p) :
    QDialog(parent = 0)
{
  //Create all necessary data

  info = info_p;
  hor_list_model = NULL;
  ver_list_model = NULL;

  info->steps_num = 6;
  info->adanto_steps[0] = new QString("Camera parameters");
  info->adanto_steps[1] = new QString("Projector parameters");
  info->adanto_steps[2] = new QString("Set parameters");
  info->adanto_steps[3] = new QString("Preview project");
  info->adanto_steps[4] = new QString("Preview projector analysis");
  info->adanto_steps[5] = new QString("Calculate 3d view");

  create_image_info(info);
  update_picture_data(info);
  update_picture_lists(info);

  //Create the UI
  //General title
  info->current_step = 0;
  Title = new QLabel(*(info->adanto_steps[(info->current_step)]), this);
  Title->setAlignment(Qt::AlignCenter);

  //Widgets of different pages
  //Page 0 (camera parameters)
  QComboBox   *cam_model_selector;
  cam_model_selector = new QComboBox();
  cam_model_selector->addItem("Sony");
  cam_model_selector->addItem("Iphone");
  //connect(cam_model_selector, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox_preview_changed(int)));

  //Page 1(Projector parameters)
  QComboBox   *proj_model_selector;
  proj_model_selector = new QComboBox();
  proj_model_selector->addItem("Sony");
  proj_model_selector->addItem("Iphone");
  //connect(proj_model_selector, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox_preview_changed(int)));


  //Page 2(Set parameters)
  QTreeView        *params_editor;
  params_editor = new QTreeView();


  //Page 3(Preview Project)
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

  //Page 4(preview projector analysis)
  //Page 5(calculate 3d view)
  QPushButton *calculator_3d       = new QPushButton("calculate 3D");

  //This is the top part that displays the current step in the entire process
  hlayout->addWidget(left_arrow);
  hlayout->addWidget(Title);
  hlayout->addWidget(right_arrow);

  mlayout->addLayout(hlayout);

  //We are going to create a stack of widgets pages,so we can the cycle between pages
  QWidget           *page_n[6];
  page_n[0]      = new QWidget;
  page_n[1]      = new QWidget;
  page_n[2]      = new QWidget;
  page_n[3]      = new QWidget;
  page_n[4]      = new QWidget;
  page_n[5]      = new QWidget;
  QVBoxLayout   *v_layout_n[6];
  QVBoxLayout *selected_layout;

  stack_of_widgets = new QStackedWidget;
  stack_of_widgets->addWidget(page_n[0]);
  stack_of_widgets->addWidget(page_n[1]);
  stack_of_widgets->addWidget(page_n[2]);
  stack_of_widgets->addWidget(page_n[3]);
  stack_of_widgets->addWidget(page_n[4]);
  stack_of_widgets->addWidget(page_n[5]);

  //First page populating(camera parameters)
  selected_layout = v_layout_n[0];
  selected_layout = new QVBoxLayout(page_n[0]);
  (page_n[0])->setLayout(selected_layout);
  selected_layout->addWidget(cam_model_selector);

  //Second page populating(projector parameters)
  selected_layout = v_layout_n[1];
  selected_layout = new QVBoxLayout(page_n[1]);
  (page_n[1])->setLayout(selected_layout);
  selected_layout->addWidget(proj_model_selector);

  //thrird page populating (set parameters)
  selected_layout = v_layout_n[2];
  selected_layout = new QVBoxLayout(page_n[2]);
  (page_n[2])->setLayout(selected_layout);
  selected_layout->addWidget(params_editor);

  //page 4 populating(Preview project)
  selected_layout = v_layout_n[3];
  selected_layout = new QVBoxLayout(page_n[3]);
  (page_n[3])->setLayout(selected_layout);
  selected_layout->addWidget(hor_slider);
  selected_layout->addWidget(hor_ver_selector);
  selected_layout->addWidget(ima_pre_selector);
  selected_layout->addWidget(list_view);

  // page 5 populating(Previrew projector analysis)
  selected_layout = v_layout_n[4];
  selected_layout = new QVBoxLayout(page_n[4]);
  (page_n[4])->setLayout(selected_layout);
  selected_layout->addWidget(pushy);
  selected_layout->addWidget(sweep_slider);
  selected_layout->addWidget(sweep_hor_ver_selector);
  selected_layout->addWidget(pushy1);
  selected_layout->addWidget(pushy2);
  selected_layout->addWidget(pushy3);

  // page 6 populating(calculate 3d view)
  selected_layout = v_layout_n[5];
  selected_layout = new QVBoxLayout(page_n[5]);
  (page_n[5])->setLayout(selected_layout);
  selected_layout->addWidget(calculator_3d);

  stack_of_widgets->setCurrentIndex((info->current_step));
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

  if (index < ((info->steps_num)-1))
  {
    index++;
    stack_of_widgets->setCurrentIndex(index);
  }

  info->current_step = index;
  Title->setText(*(info->adanto_steps[index]));
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
  info->current_step = index;
  Title->setText(*(info->adanto_steps[index]));
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
