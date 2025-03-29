#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include<unistd.h>
/* Function Definitions */

/* Get image sizes
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th bytes
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if (!argv[2] || !strstr(argv[2], ".bmp"))
        return e_failure;

    encInfo->src_image_fname = argv[2];
    printf("Source image name: %s\n", encInfo->src_image_fname);

    if (!argv[3] || !strchr(argv[3], '.'))
        return e_failure;

    encInfo->secret_fname = argv[3];
    printf("Secret file name: %s\n", encInfo->secret_fname);

    encInfo->secret_extension = strstr(argv[3], ".");  
    encInfo->extn_size = strlen(encInfo->secret_extension);


    if (argv[4] && strstr(argv[4], ".bmp"))
    {
        encInfo->stego_image_fname = argv[4];
        printf("Stego image name: %s\n", encInfo->stego_image_fname);
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
        printf("Stego image name: %s\n", encInfo->stego_image_fname);
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    printf(BLUE"INFO: Encoding started\n"RESET);
    sleep(1);
    printf("INFO: Opening files!\n");
    if((open_files(encInfo)==e_failure))
    {
            sleep(1);

        printf(RED"ERROR: Cannot open file!\n"RESET);
        return e_failure;
    }

    printf("INFO: Checking capacity\n");
    if((check_capacity(encInfo)==e_failure))
    {

        printf(RED"ERROR: Capacity is not enough!!\n"RESET);
    }
                sleep(1);

    printf(GREEN"INFO: Capacity checking done\n"RESET);
   
    printf("INFO: Copying BMP header: \n");
    if((copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure))
    {

        printf("ERROR: BMP copying not done!\n");
        return e_failure;
    }


    sleep(1);

    printf(GREEN"INFO: BMP file copy done\n"RESET);

    printf("INFO: Encoding magic string\n");
    if(encode_magic_string("#*",encInfo)==e_success)
    {
                    sleep(1);

      printf(GREEN"INFO: Magic string encoded\n"RESET);
    }

    printf("INFO: Encoding secret file extension size \n");

    if(encode_secret_file_extn_size(encInfo->extn_size,encInfo)==e_success)
    {
        sleep(1);

        printf(GREEN"INFO: Encoded secret file extension size!!\n"RESET);
    }

    printf("INFO: Encoding secret file extension\n");
    
    if (encode_secret_file_extn(encInfo->secret_extension, encInfo) == e_success)
    {            sleep(1);

        printf(GREEN"INFO: Encoded secret file extension\n"RESET);
    }

    printf("INFO: Encoding secret file size!!\n");
    fseek(encInfo->fptr_secret,0,SEEK_END);
    encInfo->size_secret_file = ftell(encInfo->fptr_secret);
    rewind(encInfo->fptr_secret);

    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
    {
                    sleep(1);

        printf(GREEN"INFO: Encoded secret file size!!!\n"RESET);
    }

    printf("INFO: Encoding secret file data\n");
    if(encode_secret_file_data(encInfo)==e_success)
    {
        printf(GREEN"INFO: Secret file data encoded\n"RESET);
    }

    printf("INFO: Copying image data\n");
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo)==e_success)
    {
                    sleep(1);

        printf(GREEN"INFO: Image data encoded\n"RESET);
    }


    return e_success;
}   
     
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    uint size = ftell(fptr);
    return size;
}
Status check_capacity(EncodeInfo *encInfo)
{ 
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file= get_file_size(encInfo->fptr_secret);

     int magic_len = strlen("#*");
     int magic_extension = 4;
     int secret_extension = (int)strlen(encInfo->secret_extension);

     int full_size = 54+(8 * (magic_len +magic_extension+secret_extension+encInfo->size_secret_file));
     if(encInfo->image_capacity>full_size)
        return e_success;
     else
        return e_failure;
     
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
        char buffer[54];
        fseek(fptr_src_image,0,SEEK_SET);

        printf("INFO: Reading BMP header...\n");
        
        if (fread(buffer, 54, 1, fptr_src_image) != 1)
        {
                        sleep(1);

            printf(RED"ERROR: fread() failed\n"RESET);
            return e_failure;
        }
        printf(GREEN"INFO: Image reading successful\n"RESET);
        
        if (fwrite(buffer, 54, 1, fptr_dest_image) != 1)
        {
            printf("ERROR: fwrite() failed\n");
            return e_failure;
        }
        printf(GREEN"INFO: BMP header copy successfull\n"RESET);
        return e_success;
        
}  
   /*
         1. call open_files()
            (WE SHOULD PRINT MESSAGES FOR EACH OPERATION)
            INFO: FOR POSITIVE OR WORK DONE MESSAGES
            ERROR: FOR FAIILURE CASES          
    */

    void byte_to_lsb(char data, char *buffer)
    {
        int pos = 7;
        for(int i = 0; i < 8; i++)
        {
            unsigned char mask = 1 << (pos - i);
            unsigned char res = (data & mask) >> (pos - i);
            buffer[i] &= 0xFE;
            buffer[i] |= res;
    
        }
    }
    void size_to_lsb(int data, char *buffer)
    {
        int pos = 31;
        for (int i = 0; i < 32; i++)
        {
            unsigned int mask = 1 << (pos - i);
            unsigned int res = (data & mask) >> (pos - i);
            buffer[i] &= 0xFE; 
            buffer[i] |= res;
        }
    }

    
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int len = strlen(magic_string);
    char buffer[8];
    fseek(encInfo->fptr_src_image, 54, SEEK_SET);
    fseek(encInfo->fptr_stego_image, 54, SEEK_SET);


    for (int i = 0; i < len; i++)
    {     
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        byte_to_lsb(magic_string[i], buffer);
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success; 
}
   

Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{    
    char buffer[32];
    fread(buffer, 1, 32, encInfo->fptr_src_image);
    size_to_lsb(extn_size, buffer);
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
    return e_success;
}


Status encode_secret_file_extn(const char *secret_extension, EncodeInfo *encInfo)
{
    int len = encInfo->extn_size;
    char buffer[8];


    for(int i = 0; i < len; i++)
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        byte_to_lsb(secret_extension[i], buffer);
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }

    return e_success;
}
 
   

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
      char buffer[32];
      fread(buffer,1,32,encInfo->fptr_src_image);
      size_to_lsb(encInfo->size_secret_file,buffer);
      fwrite(buffer,1,32,encInfo->fptr_stego_image);

      return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int len = encInfo->size_secret_file;
    char buffer[8];
    char data;

    for (int i = 0; i < len; i++)
    {
        fread(&data, 1, 1, encInfo->fptr_secret);
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        

        byte_to_lsb(data, buffer);
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }

    return e_success;
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest, EncodeInfo *encInfo)
{
    int ch;
    while ((ch = fgetc(fptr_src)) != EOF)  
    {
        fputc(ch, fptr_dest);
    }
    return e_success;

}






