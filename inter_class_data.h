#ifndef INTER_CLASS_DATA_H
#define INTER_CLASS_DATA_H

#include <QString>

typedef struct
{
  int                 level_h_max;//Maximun horizontal level
  int                 level_v_max;//Maximun vertical level

  int          selected_list_item;
  int         default_orientation;//Horizontal = 0; Vertical = 1

  QString                 *folder;
}image_info;

#endif // INTER_CLASS_DATA_H
