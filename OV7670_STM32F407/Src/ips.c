/*
 * ips.c
 *
 *  Created on: 2019/12/16
 *      Author: 0000400921
 */
#include "ips.h"
#include "print.h"

//unsigned char graph_index[8] = {0, 4, 8, 12, 16, 20, 24, 27};

//unsigned char image_ori[IMAGE_SIZE][IMAGE_SIZE] = {
//    {149, 151, 153, 155, 158, 159, 161, 163, 164, 165, 165, 167, 167, 167, 169,
//     168, 169, 168, 169, 170, 169, 169, 168, 167, 166, 164, 162, 161},
//    {149, 152, 154, 157, 159, 162, 162, 164, 165, 166, 168, 168, 169, 169, 169,
//     171, 170, 171, 171, 171, 171, 171, 170, 169, 168, 167, 167, 166},
//    {98, 152, 155, 157, 161, 162, 164, 165, 167, 168, 170, 170, 171, 171, 172,
//     171, 173, 173, 173, 172, 172, 172, 172, 170, 170, 169, 168, 168},
//    {42, 44, 153, 160, 162, 163, 165, 168, 168, 170, 171, 172, 172, 173, 173,
//     173, 174, 175, 175, 174, 174, 174, 173, 172, 171, 171, 170, 169},
//    {44, 42, 42, 159, 163, 165, 166, 169, 170, 171, 172, 173, 174, 176, 175,
//     176, 176, 176, 176, 176, 176, 175, 174, 175, 173, 172, 172, 170},
//    {47, 39, 41, 44, 160, 165, 167, 169, 171, 172, 173, 174, 174, 174, 177, 176,
//     176, 177, 177, 177, 177, 176, 176, 175, 175, 174, 173, 171},
//    {40, 40, 40, 43, 44, 165, 167, 169, 171, 173, 175, 175, 176, 176, 177, 178,
//     178, 177, 177, 177, 177, 177, 177, 176, 176, 174, 174, 173},
//    {41, 40, 43, 43, 42, 47, 167, 170, 172, 174, 175, 176, 176, 178, 178, 179,
//     179, 179, 178, 176, 177, 177, 176, 177, 176, 176, 174, 173},
//    {41, 41, 41, 42, 43, 44, 43, 168, 172, 174, 176, 177, 177, 178, 179, 180,
//     180, 180, 180, 179, 179, 177, 176, 175, 176, 174, 175, 174},
//    {42, 41, 41, 43, 43, 42, 44, 58, 172, 174, 176, 177, 177, 179, 180, 180,
//     181, 181, 181, 179, 180, 179, 180, 179, 178, 175, 174, 173},
//    {42, 40, 41, 41, 42, 43, 44, 45, 98, 174, 176, 178, 177, 179, 179, 180, 181,
//     181, 181, 181, 180, 179, 180, 179, 179, 178, 178, 176},
//    {41, 40, 41, 42, 40, 42, 43, 44, 44, 148, 176, 177, 180, 179, 180, 181, 181,
//     181, 181, 181, 180, 181, 180, 180, 179, 178, 178, 176},
//    {41, 40, 39, 41, 41, 42, 43, 42, 43, 47, 174, 178, 179, 179, 180, 182, 180,
//     180, 181, 180, 180, 180, 180, 180, 179, 178, 177, 176},
//    {40, 39, 41, 39, 41, 42, 41, 43, 43, 43, 42, 176, 178, 179, 181, 182, 181,
//     183, 182, 181, 180, 180, 179, 179, 179, 177, 177, 176},
//    {40, 40, 41, 41, 41, 40, 42, 41, 42, 44, 45, 94, 176, 177, 179, 179, 180,
//     181, 180, 181, 179, 178, 178, 178, 179, 178, 177, 175},
//    {141, 42, 40, 40, 40, 41, 41, 41, 40, 41, 43, 45, 153, 173, 174, 176, 177,
//     178, 178, 178, 177, 178, 176, 177, 176, 175, 174, 175},
//    {153, 42, 40, 39, 39, 39, 39, 40, 41, 41, 42, 44, 45, 171, 173, 175, 177,
//     178, 177, 179, 179, 177, 178, 177, 177, 175, 173, 173},
//    {154, 149, 41, 40, 38, 39, 40, 39, 40, 40, 40, 42, 42, 92, 172, 175, 176,
//     176, 177, 178, 177, 176, 176, 177, 176, 176, 175, 173},
//    {153, 154, 45, 40, 39, 38, 39, 39, 39, 41, 39, 42, 42, 46, 161, 171, 173,
//     174, 175, 176, 176, 175, 175, 175, 175, 174, 173, 172},
//    {154, 154, 148, 40, 38, 38, 38, 38, 38, 39, 39, 40, 41, 43, 49, 170, 170,
//     172, 173, 174, 174, 174, 174, 174, 173, 173, 173, 170},
//    {152, 153, 154, 44, 38, 37, 37, 38, 38, 39, 38, 38, 39, 40, 42, 137, 168,
//     171, 170, 171, 171, 171, 172, 172, 172, 171, 171, 169},
//    {152, 153, 154, 141, 40, 38, 37, 38, 39, 38, 38, 39, 39, 40, 40, 43, 164,
//     168, 169, 170, 171, 171, 170, 171, 170, 169, 169, 167},
//    {151, 152, 153, 153, 38, 38, 39, 37, 37, 38, 38, 38, 37, 38, 39, 39, 103,
//     165, 165, 168, 167, 167, 169, 167, 168, 166, 164, 166},
//    {149, 152, 152, 154, 118, 38, 37, 38, 39, 37, 37, 37, 39, 38, 38, 39, 41,
//     162, 166, 166, 166, 165, 167, 165, 167, 163, 163, 161},
//    {149, 150, 152, 153, 152, 39, 37, 36, 37, 37, 37, 37, 37, 37, 39, 38, 39,
//     85, 164, 165, 165, 163, 165, 165, 165, 164, 163, 162},
//    {148, 150, 151, 152, 153, 60, 38, 37, 36, 36, 36, 36, 37, 36, 36, 39, 37,
//     41, 157, 163, 163, 164, 165, 163, 163, 161, 162, 159},
//    {147, 149, 149, 152, 151, 139, 39, 38, 37, 37, 35, 37, 35, 36, 37, 35, 38,
//     38, 82, 161, 160, 162, 162, 162, 161, 161, 160, 159},
//    {147, 147, 149, 150, 150, 149, 36, 38, 37, 34, 35, 36, 37, 35, 37, 36, 38,
//     37, 39, 156, 157, 160, 160, 160, 159, 159, 159, 156}};

// printf("%2d, ", image[i][j]);
void binary_simple(unsigned char (*image_in)[IMAGE_SIZE],
    unsigned char (*image_out)[IMAGE_SIZE])
{
  unsigned int i, j;
  for (i = 0; i < IMAGE_SIZE; i++)
  {
    for (j = 0; j < IMAGE_SIZE; j++)
    {
      if (image_in[i][j] > THRESHOLD)
      {
        image_out[i][j] = 1;
      }
      else
      {
        image_out[i][j] = 0;
      }
    }
  }
}

void find_center_line(unsigned char (*image_bin)[IMAGE_SIZE],
    unsigned char *line_flag_list, unsigned char *line_pos_list)
{
  unsigned int i, j;
  unsigned int l_edge = 0, r_edge = IMAGE_SIZE;
  int delta = 0, mul_flag = 1;  // default: no line
  for (i = 0; i < IMAGE_SIZE; i++)
  {
    mul_flag = 1;
    for (j = 0; j < IMAGE_SIZE - 1; j++)
    {
      delta = image_bin[i][j] - image_bin[i][j + 1];
      switch (delta)
      {
      case 0:
        ;
        break;  // printf("flat\n")
      case 1:
        l_edge = j;
        break;
      case -1:
        r_edge = j;
        break;

      default:
        print_str("find_center_line Error", 1);
      }
      mul_flag *= image_bin[i][j];
    }
    // store flag
    if (mul_flag)
    {
      line_flag_list[i] = 0;
    }
    else
    {
      line_flag_list[i] = 1;
    }
    // store position
    line_pos_list[i] = (int) ((l_edge + r_edge) / 2);
  }
}

// print list
void print_list(unsigned char *list)
{
  unsigned int i;
  for (i = 0; i < IMAGE_SIZE; i++)
  {
    print_int(list[i], 0);
    print_str(" ", 0);
  }
  print_str("\r\n", 0);
}

// print image
void print_image(unsigned char (*image)[IMAGE_SIZE])
{
  unsigned int i, j;
  for (i = 0; i < IMAGE_SIZE; i++)
  {
    for (j = 0; j < IMAGE_SIZE; j++)
    {
      print_int(image[i][j], 0);
      print_str(" ", 0);
    }
    print_str("\r\n", 0);
  }
}

// show image
void show_image_bin(unsigned char (*image)[IMAGE_SIZE])
{
  unsigned int i, j;
  for (i = 0; i < IMAGE_SIZE; i++)
  {
    print_int(i, 0);
    if(i<10)
    {print_str(" ", 0);}
    for (j = 0; j < IMAGE_SIZE; j++)
    {
      if (image[i][j] > 0)
      {
        print_str("++", 0);
      }
      else
      {
        print_str("  ", 0);
      }
    }
    print_str("\r\n", 0);
  }
}

// generate data[8bits x 8] for display
void generate_dis_data(unsigned char *line_pos_list,
    unsigned char *disp_data_list)
{
  unsigned int i;

  for (i = 0; i < IMAGE_SIZE; i++)
  {  // row
    if (line_pos_list[i] > 24)
    {
      disp_data_list[i] = 0b00000001;
    }
    else if (line_pos_list[i] > 21)
    {
      disp_data_list[i] = 0b00000010;
    }
    else if (line_pos_list[i] > 17)
    {
      disp_data_list[i] = 0b00000100;
    }
    else if (line_pos_list[i] > 14)
    {
      disp_data_list[i] = 0b00001000;
    }
    else if (line_pos_list[i] > 10)
    {
      disp_data_list[i] = 0b00010000;
    }
    else if (line_pos_list[i] > 7)
    {
      disp_data_list[i] = 0b00100000;
    }
    else if (line_pos_list[i] > 3)
    {
      disp_data_list[i] = 0b01000000;
    }
    else
    {
      disp_data_list[i] = 0b10000000;
    }
  }
}

//int main()
//{
//    printf("original image:\r\n");
//    print_image(image_ori);
//    binary_simple(image_ori, image_bin);
//    printf("binary image:\r\n");
//    print_image(image_bin);
//    printf("show image:\r\n");
//    show_image_bin(image_bin);
//
//    unsigned char line_flag_list[IMAGE_SIZE];
//    unsigned char line_pos_list[IMAGE_SIZE];
//    find_center_line(image_bin, line_flag_list, line_pos_list);
//
//    printf("show line flags:\r\n");
//    print_list(line_flag_list);
//    printf("show line positions:\r\n");
//    print_list(line_pos_list);
//
//    return 0;
//}
