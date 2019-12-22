/*
 * ips.h
 *
 *  Created on: 2019/12/16
 *      Author: 0000400921
 */

#ifndef IPS_H_
#define IPS_H_

#define IMAGE_SIZE 28
#define THRESHOLD 105

void binary_simple(unsigned char (*image_in)[IMAGE_SIZE], unsigned char (*image_out)[IMAGE_SIZE]);
void find_center_line(unsigned char (*image_bin)[IMAGE_SIZE],
                      unsigned char *line_flag_list,
                      unsigned char *line_pos_list);
void print_list(unsigned char *list);
void print_image(unsigned char (*image)[IMAGE_SIZE]);
void show_image_bin(unsigned char (*image)[IMAGE_SIZE]);
void generate_dis_data(unsigned char *line_pos_list,
                       unsigned char *disp_data_list);

#endif /* IPS_H_ */
