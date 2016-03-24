/* chrisky draft code to do a single frame decode from S920 format
with 640x480 dimension. This was the results of hours of experiments 
and hexdump. Also thanks Fredrik Pihl from stackoverflow and his reference
to "Sonix SN9C20X decoder
 * Vasily Khoruzhick, (C) 2008-2009"
 
 this function converts a 640x480 frame size of S920 to YUY2
 
 This file is GNU LGPL licensed
 */
 
#include<stdio.h>

int organize_192(char* input, char* output, int row_length){
    int i = 0;
    int col_length;
    //y
    int relative_i = 0;
    for(i=0;i<64;i++){
        output[(i/8)*32+(i%8)*2]=input[i];
    }
    for(i=64;i<128;i++){
        output[((i-64)/8)*32+(i%8)*2+16]=input[i];
    }
    //u
    for(i=128;i<160;i++){
        relative_i = i-128;
        output[relative_i%(row_length/2)*4+1 +2*row_length*2*(relative_i/(row_length/2))]=input[i];
        output[relative_i%(row_length/2)*4+1+2*2*row_length*(relative_i/(row_length/2))+row_length*2]=input[i];
    }
    //v
    for(i=160;i<192;i++){
        relative_i = i-160;
        output[(relative_i%(row_length/2))*4+3+2*row_length*2*(relative_i/(row_length/2))]=input[i];
        output[(relative_i%(row_length/2))*4+3+2*2*row_length*(relative_i/(row_length/2))+row_length*2]=input[i];
    }
}

int s920_to_yuy2_640x480(char *input_all, char *output_all){
    unsigned int total_input_size =460800;
    unsigned int total_output_size = 614400;
    unsigned int total_blocks = 460800/192;
    unsigned int block_row_length = 16;
    unsigned int block_col_length;
    unsigned int blocks_per_row;
    unsigned int i,ii,iii;
    char temp_block_output[256];
    block_col_length = 128/block_row_length;
    blocks_per_row=640/block_row_length;
    for(i=0;i<total_blocks;i++){
        organize_192(input_all+i*192,temp_block_output,block_row_length);
        for(ii=0;ii<block_col_length;ii++){
            for(iii=0;iii<block_row_length*2;iii++){
                output_all[((i/blocks_per_row)*block_col_length)*640*2+(i%blocks_per_row)*block_row_length*2+ii*640*2+iii]=temp_block_output[ii*block_row_length*2+iii];
            }
        }
    }
}
