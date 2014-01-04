#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

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
#include <QAction>

#include <QFile>
#include <QMessageBox>
#include <QStringListModel>

#include "inter_class_data.h"

class controlPanel : public QDialog
{
    Q_OBJECT
public:
    controlPanel(QWidget*,image_info*);
    ~controlPanel();
    QSlider   *hor_slider;
    QSlider *sweep_slider;

    QStackedWidget *stack_of_widgets;
    void        create_image_info(image_info*);
    void    update_control_panel (image_info*);
    void         update_list_view(image_info*);
    void      update_picture_data(image_info*);
    void     update_picture_lists(image_info*);

public slots:
    void                change_next_page(bool);
    void                change_last_page(bool);
    void            combobox_indexchanged(int);
    void         combobox_preview_changed(int);
    void double_click_on_fileview(QModelIndex);
protected:
    void         closeEvent(QCloseEvent *);

private:

  QLabel                    *Title;
  image_info                 *info;

  QListView             *list_view;
  QStringListModel *hor_list_model;
  QStringListModel *ver_list_model;

signals:
  void    closing_panel();
  void display_new_file();
};

#endif // CONTROL_PANEL_H
