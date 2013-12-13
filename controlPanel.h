#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <QWidget>
#include <QDialog>
#include <Qlabel>
#include <QSlider>

class controlPanel : public QDialog
{
    Q_OBJECT
public:
    controlPanel(QWidget *parent = 0);
    ~controlPanel();
    QSlider   *hor_slider;
    QSlider *sweep_slider;
private:
    QLabel       *Title;
};

#endif // CONTROL_PANEL_H
