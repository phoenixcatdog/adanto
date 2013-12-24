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
#include <QAction>

#include <QFile>
#include <QMessageBox>

#include "inter_class_data.h"

class controlPanel : public QDialog
{
    Q_OBJECT
public:
    controlPanel(QWidget *parent = 0);
    ~controlPanel();
    QSlider   *hor_slider;
    QSlider *sweep_slider;

    QStackedWidget *stack_of_widgets;
    void update_control_panel (image_info*);
    void     update_list_view(image_info *);

public slots:
    void change_next_page(bool);
    void change_last_page(bool);

protected:
    void         closeEvent(QCloseEvent *);

private:
    QLabel       *Title;

signals:
    void closing_panel();
};

#endif // CONTROL_PANEL_H
