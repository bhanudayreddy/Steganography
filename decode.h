#ifndef DECODE_H
#define DECODE_H
#include<stdio.h>

#include "types.h" // Contains user defined types
/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"



typedef struct _DecodeInfo
{
    // /* Source Image info */
    // char *;
    // FILE *fptr_src_image;
    // uint image_capacity;
    // uint bits_per_pixel;
    // char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *output_fname;
    char entered_magic_string[3];
    char decoded_magic_string[3];
    FILE *fptr_output;
    char *secret_extension;
    int extn_size;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} DecodeInfo;

Status open_files_to_decode(DecodeInfo *decInfo);

/* Decoding function prototype */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/*Start decoding*/
Status do_decoding(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

Status validate_magic_string(DecodeInfo *decInfo);


/* Store secret extension size*/
Status decode_secret_file_extn_size( DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn( DecodeInfo *decInfo);

/* Encode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/*Encoding bytes of a character*/
char lsb_to_byte(char * buffer);
/*Encoding bytes of an integer*/
int lsb_to_size(char * buffer);



#endif
