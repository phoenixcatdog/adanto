#ifndef SCANNERFILEDIALOG_H
#define SCANNERFILEDIALOG_H

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QStackedWidget>
#include <QSlider>
#include <QComboBox>
#include <QListWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QAction>
#include <QSplitter>
#include <QString>

#include "inter_class_data.h"

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

#include <QFile>
#include <QMessageBox>


class scannerFileDialog : public QDialog
{
    Q_OBJECT
public:
    scannerFileDialog (QWidget *, image_info *);
    ~scannerFileDialog();

    int checkFolder(QString folder_path);

    QVBoxLayout   *big_forest;//Forest contains the trees
    QSplitter      *splitter;
    QHBoxLayout    *tree_one;
    QHBoxLayout    *tree_two;

    QString     help_message;
    QString    green_message;
    QString   yellow_message;
    QString      red_message;
    QString     red_message1;
    QString     red_message2;

    int    candidate_ver_level;//Candidate vertical level from file sources
    int    candidate_hor_level;
    QString    *project_folder;
public slots:
    void click_on_treeview(QModelIndex);
    void double_click_on_listview(QModelIndex);

protected:
    void         closeEvent(QCloseEvent *);

private:
    image_info               *info;

    QLabel                       *createPixmapLabel();
    int       get_sem_error(int,QPixmap **,QString**);
    int              add_sem_error(QPixmap*,QString*);
    //int  fill_digits(image_info*,int,QChar**,QChar**);
    int                create_all_digits(image_info*);

    QTreeView        *folders_view;
    QListView          *files_view;

    QFileSystemModel *folder_model;
    QFileSystemModel  *files_model;

    QPixmap            semaphore;
    QPixmap      semaphore_green;
    QPixmap     semaphore_yellow;
    QPixmap        semaphore_red;
    QLabel      *semaphore_label;//This is the label we use to display semaphore pixmaps
    QLabel       *message_widget;// We are going to communicate all messages using this

    int                error_num;
    QPixmap *error_semaphore[20];
    QString   *error_message[20];

signals:
    void load_new_project(int, int, QString *);
    void closing_panel();
};

#endif // SCANNERFILEDIALOG_H
