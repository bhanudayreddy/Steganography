#include<stdio.h>   
#include<string.h>
#include<stdlib.h>  
#include "decode.h"
#include "common.h"
#include "types.h" 
#include<unistd.h>

Status open_files_to_decode(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "INFO: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }

  
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);
        return e_failure;
    }
    
    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (!argv[2] || !strstr(argv[2], ".bmp"))
        return e_failure;

    decInfo->stego_image_fname = argv[2];
    printf("Source image name: %s\n", decInfo->stego_image_fname);

    if (argv[3])
    {
        decInfo->output_fname = argv[3];
        printf("Output file name: %s\n", decInfo->output_fname);
    }
    else
    {
        static char output_name[50];  
        strcpy(output_name, "decoded_secret");
        strcat(output_name, decInfo->extn_secret_file);
        decInfo->output_fname = output_name;
        printf("INFO: NO OUTPUT FILE NAME PROVIDED, Automatic name: %s\n", decInfo->output_fname);
    }
    

    return e_success;           
}

Status do_decoding(DecodeInfo *decInfo)
{
    
  
    printf(BLUE"INFO: DECODING STARTED \n"RESET);
    printf("INFO: Opening files\n");
    if (open_files_to_decode(decInfo) == e_failure)
    {
        printf(RED"ERROR: FILES DID NOT OPEN\n"RESET);
        return e_failure;
    }
    sleep(1);
    printf("INFO: Decoding magic string\n");
    if (decode_magic_string(decInfo) == e_success)
    {
        printf(GREEN"INFO: Magic string decoded!\n"RESET);
    }
    if (validate_magic_string(decInfo) == e_success)
    {
        printf(GREEN"INFO: Magic string matched!\n"RESET);
    }
    else
    {
        return e_failure;
    }
    printf("INFO: Decoding secret file extension size\n");
    if (decode_secret_file_extn_size(decInfo) == e_success)
    {
        sleep(1);
        printf(GREEN"INFO: Secret file extension size decoded\n"RESET);
    }
    printf("INFO: Decoding secret file extension\n");
    if (decode_secret_file_extn(decInfo) == e_success)
    {
            sleep(1);

        printf(GREEN"INFO: Secret file extension decoded \n"RESET);
    }
    printf("INFO: Decoding secret file size\n");
    if (decode_secret_file_size(decInfo) == e_success)
    {
            sleep(1);

        printf(GREEN"INFO: Secret file size decoded\n"RESET);
    }
    printf("INFO: Decoding secret file data\n");
    if (decode_secret_file_data(decInfo) == e_success)
    {
            sleep(1);

        printf(GREEN"INFO: Secret file data successfully decoded\n"RESET);
    }

    return e_success;
}

char lsb_to_byte(char *buffer)
{
    unsigned char ch = 0;
    for (int i = 0; i < 8; i++)
    {
        ch = ch << 1 | (buffer[i] & 1);

    }
    return ch;
}

int lsb_to_size(char *buffer)
{
    int size = 0;

    for (int i = 0; i < 32; i++)  
    {
        int bit = (buffer[i] & 1);  
        size = (size << 1) | bit;  
    }


    return size;
}

Status validate_magic_string(DecodeInfo *decInfo)
{
   printf("INFO: Validating the entered magic string\n");
   if(strcmp(decInfo->entered_magic_string,decInfo->decoded_magic_string)!=0)
   {
      printf(RED"INFO: Magic string didnot match, Decoding Aborted\n"RESET);
         return e_failure;
   }
  
    return e_success;
   
    

}


Status decode_magic_string(DecodeInfo *decInfo)
{
    printf("Enter the magic string to continue decoding: ");
    scanf(" %[^\n]",decInfo->entered_magic_string);
    
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    char buffer[8];
    char magic[3];
    for (int i = 0; i < 2; i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        magic[i] = lsb_to_byte(buffer);
    }
     magic[2] = '\0';
     printf("%s\n",magic);
    strcpy(decInfo->decoded_magic_string,magic);
        



    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer, 1, 32, decInfo->fptr_stego_image);
    decInfo->extn_size = lsb_to_size(buffer);

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];


    for (int i = 0; i < decInfo->extn_size; i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        decInfo->extn_secret_file[i] = lsb_to_byte(buffer);
    }
    decInfo->extn_secret_file[decInfo->extn_size] = '\0';

    
    return e_success;
}


Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer, 1, 32, decInfo->fptr_stego_image);  
    decInfo->size_secret_file = lsb_to_size(buffer);

    return e_success;
}


Status decode_secret_file_data(DecodeInfo *decInfo)
{
    int len = decInfo->size_secret_file;
    char buffer[8];
    
    for (int i = 0; i < len; i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        char decoded_char = lsb_to_byte(buffer); 
        fwrite(&decoded_char, 1, 1, decInfo->fptr_output); 
    }

    return e_success;
}






































