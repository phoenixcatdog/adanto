#ifndef INTER_CLASS_DATA_H
#define INTER_CLASS_DATA_H

#include <QString>
#include <QStringList>

#define INFO_FLAG_DRAW_IMAGE  0x1

typedef struct
{
  unsigned int          info_flag;

  int               update_screen;//0 when screen is updated, 1 when needs redrawing
  int                 no_textures;//1 when no textures are loaded

  int                   display_w;//Main Window size in pixels
  int                   display_h;

  int                 level_h_max;//Maximun horizontal level
  int                 level_v_max;//Maximun vertical level

  int          selected_list_item;
  int        selected_orientation;//Horizontal = 0;     Vertical = 1
  int                preview_mode;//Picture = 0; threshold image = 1

  QImage              *ima_loaded;

  QString                 *folder;
  QStringList       *hor_pic_list;
  QStringList       *ver_pic_list;

  QChar         all_digits[10];//The decimal numbers digits
}image_info;

int fill_digits(image_info *,int,QChar**,QChar**);

#endif // INTER_CLASS_DATA_H
