#include "scannerfiledialog.h"

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

int scannerFileDialog::add_sem_error(QPixmap *semaphore_light,QString *error_message_val)
{
    int   num;

    num = error_num;
    error_semaphore[num]=   semaphore_light;
    error_message[num]  = error_message_val;
    num++;
    error_num = num;

    return(0);
}

int scannerFileDialog::get_sem_error(int num, QPixmap **semaphore_light,QString **error_message_val)
{
    if (num >= error_num)
    {return(-1);}
    *semaphore_light     = error_semaphore[num];
    *error_message_val   =   error_message[num];

    return(0);
}

int scannerFileDialog::fill_digits(int num, QChar **digit_1,QChar **digit_2)
{
    int   d1;
    int   d2;

    if (num> 99)
    {return(-1);}

    d1 = num / 10;
    d2 = num - 10*d1;

    *digit_1 = &(all_digits[d1]);
    *digit_2 = &(all_digits[d2]);

    return(0);
}

int scannerFileDialog::create_all_digits(void)
{
   int    i;
   char dig;

   for( i = 0 ; i < 10; i++ )
   {
     dig = i + '0';
     all_digits[i] = QChar(dig);
   }

    return(0);
}

scannerFileDialog::scannerFileDialog( QWidget *parent) :
    QDialog(parent)
{
    project_folder = NULL;

    help_message   = QString ("Please select the folder that contains the project.");
    green_message  = QString ("Looks like there is a valid project in this folder. Double Click on any file inside the folder to open it.");
    yellow_message = QString ("This folder does not contain a valid project.");
    red_message    = QString ("The number of pictures in different sets is different");
    red_message1   = QString ("H0000 exist but V0000 does not");
    red_message2   = QString ("V0000 exist but H0000 does not");

    create_all_digits();

    error_num = 0;
    add_sem_error( &semaphore_green, &green_message);// Everything is ok
    add_sem_error(&semaphore_yellow,&yellow_message);// A folder without project
    add_sem_error(   &semaphore_red,   &red_message);// Various error codes
    add_sem_error(   &semaphore_red,   &red_message2);// Various error codes
    add_sem_error(   &semaphore_red,   &red_message2);// Various error codes

    big_forest = new QVBoxLayout(this);

    splitter   = new QSplitter(this);
    tree_one   = new QHBoxLayout(this);
    splitter->setOrientation(Qt::Horizontal);

    folders_view = new QTreeView();
    files_view   = new QListView();

    folder_model = new QFileSystemModel;
    folder_model->setRootPath(QDir::homePath());
    folder_model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    folders_view->setModel(folder_model);
    folders_view->hideColumn(1);
    folders_view->hideColumn(2);
    folders_view->hideColumn(3);

    files_model = new QFileSystemModel;
    files_model->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    files_model->setRootPath(QDir::homePath());

    files_view->setModel(files_model);

    connect(folders_view, SIGNAL(clicked(QModelIndex)), this, SLOT(click_on_treeview(QModelIndex)));

    tree_one->addWidget(splitter);
    splitter->addWidget(folders_view);
    splitter->addWidget(files_view);

    tree_two   = new QHBoxLayout(this);
    tree_two->setSizeConstraint(QLayout::SetMinimumSize);

    semaphore = QPixmap("://icons/semaphore.png");
    semaphore_red    = QPixmap("://icons/semaphore_red.png");
    semaphore_yellow = QPixmap("://icons/semaphore_yellow.png");
    semaphore_green  = QPixmap("://icons/semaphore_green.png");

    semaphore_label = createPixmapLabel();
    semaphore_label->setPixmap(semaphore);
    semaphore_label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    message_widget = new QLabel();
    message_widget->setText(help_message);
    message_widget->setAlignment(Qt::AlignCenter);
    message_widget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    tree_two->addWidget(semaphore_label);
    tree_two->addWidget(message_widget);

    big_forest->addLayout(tree_one);
    big_forest->addLayout(tree_two);

    setLayout(big_forest);
    connect(files_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(double_click_on_listview(QModelIndex)));

    setWindowTitle("Project explorer");
    show();
}

scannerFileDialog::~scannerFileDialog()
{
}

QLabel *scannerFileDialog::createPixmapLabel()
{
    QLabel *label = new QLabel;
    label->setEnabled(false);
    label->setAlignment(Qt::AlignCenter);
    label->setFrameShape(QFrame::NoFrame);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label->setBackgroundRole(QPalette::Base);
    label->setAutoFillBackground(true);
    label->setMinimumSize(64, 64);
    return label;
}

int scannerFileDialog::checkFolder(QString folder_path)
{
  int                 i;
  int           out_val;
  int           level_h;
  int           level_v;
  int         max_level;
  int  rewriting_size_h;
  int  rewriting_size_v;

  max_level = 99;
  level_h   =  0;
  level_v   =  0;

  QString  slash = "/";
  QString extension = QString(".jpg");
  QChar           *digit1;
  QChar           *digit2;


  QString hor_id     = "H";
  QString ver_id     = "V";
  QString number_l1  = "00";
  QString number_l2  = "00";

  QString name_h = folder_path;
  rewriting_size_h = folder_path.size() + slash.size() + hor_id.size() + number_l1.size();

  name_h.reserve( rewriting_size_h + number_l2.size() + extension.size() );
  name_h += slash;
  name_h += hor_id;
  name_h += number_l1;
  name_h += number_l2;
  name_h += extension;

  QString name_v = folder_path;
  rewriting_size_v = folder_path.size() + slash.size() + ver_id.size() + number_l1.size();
  name_v.reserve( rewriting_size_v + number_l2.size() + extension.size() );
  name_v += slash;
  name_v += ver_id;
  name_v += number_l1;
  name_v += number_l2;
  name_v += extension;

  out_val = 0;

  QFile pictureh(name_h);
  QFile picturev(name_v);

  if (!(pictureh.exists()))
  {
      if (!(picturev.exists()))
      {out_val = 1;}
      else
      {out_val = 3;}
  }
  else
  {
      if (!(picturev.exists()))
      {
        out_val = 4;
      }
      else// Both files exis, now we need to see how many files are there
      {
        out_val = 0;
        for( i = 0; i < max_level; i++)//Find all horizontal levels
        {
          fill_digits(i, &digit1, &digit2);
          name_h.replace(rewriting_size_h  ,1,*digit1);
          name_h.replace(rewriting_size_h+1,1,*digit2);

          QFile var_file(name_h);
          if(!(var_file.exists()))
          {
            break;
          }
          level_h++;
        }

        for( i = 0; i < max_level; i++)//Find all vertical levels
        {
          fill_digits(i, &digit1, &digit2);
          name_v.replace(rewriting_size_v  ,1,*digit1);
          name_v.replace(rewriting_size_v+1,1,*digit2);

          QFile var_file(name_v);
          if(!(var_file.exists()))
          {
            break;
          }
          level_v++;
        }
        candidate_hor_level = level_h;
        candidate_ver_level = level_v;
      }
  }


  return out_val;
}


void scannerFileDialog::click_on_treeview(QModelIndex index)
{
  int                 out;
  QPixmap          *pix_p;
  QString       *string_p;

  if (project_folder != NULL)
  {
    delete project_folder;
    project_folder = NULL;
  }

  QString folder_path = folder_model->fileInfo(index).absoluteFilePath();
  files_view->setRootIndex(files_model->setRootPath(folder_path));

  project_folder = new QString(folder_path);//Create a new instance of QString

  out = checkFolder(folder_path);

  if(get_sem_error(out, &pix_p,&string_p)>=0)
  {
    semaphore_label->setPixmap(*pix_p);
    message_widget->setText(*string_p);
    semaphore_label->show();
    message_widget->show();
  }

  return;
}

void scannerFileDialog::double_click_on_listview(QModelIndex index)
{
  int                 out;

  out = checkFolder(*project_folder);

  if(out == 0)//Load new project in imagedisplay
  {
    emit load_new_project(candidate_hor_level, candidate_ver_level,project_folder);
  }


  return;
}

void scannerFileDialog::closeEvent(QCloseEvent *event)
{
  emit closing_panel();

  return;
}
