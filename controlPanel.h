#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

//Copyright: Jose Hevia jose.francisco.hevia (at) gmail
//License :GPLv2

#include <QWidget>
#include <QDialog>
#include <Qlabel>
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

class controlPanel : public QDialog
{
    Q_OBJECT
public:
    controlPanel(QWidget *parent = 0);
    ~controlPanel();
    QSlider   *hor_slider;
    QSlider *sweep_slider;

    QStackedWidget *stack_of_widgets;
public slots:
    void change_next_page(bool);
    void change_last_page(bool);

private:
    QLabel       *Title;
};

#endif // CONTROL_PANEL_H
