#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"


int main(int argc,char * argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;


    int ret_arg;
    int ret = check_operation_type(argv,argc);
    if(ret==e_encode)
    {    
        if(read_and_validate_encode_args(argv,&encInfo)== e_success)
        {
            if(do_encoding(&encInfo)==e_success)
            {
                printf(BLUE"INFO: ENCODING DONE!!\n"RESET);
            }
            else if(do_encoding(&encInfo)==e_failure)
            {
                printf("RED: ENCODING FAILED\n"RESET);
            }
        }
        else
        {
        printf(RED"ERROR: Invalid arguements passed."RESET);
        }
    }   
    else if(ret == e_decode)
    {
        if(read_and_validate_decode_args(argv,&decInfo)== e_success)
        {
            if(do_decoding(&decInfo)==e_success)
            {
               printf(BLUE"INFO: DECODING DONE!!!\n"RESET);
            }
             else if(do_decoding(&decInfo)==e_failure)
            {
                printf(RED"DECDING FAILED\n"RESET);
            }
        }
    }
    else
    {
        printf(RED"ERROR: Unsupported arguement(s) passed"RESET);
    }
}
OperationType check_operation_type(char *argv[],int argc)

{   
    if(argc<2)
    {
        return e_unsupported;
    }
        if(strcmp(argv[1],"-e")== 0)
        {
            return e_encode;
        }
        else if(strcmp(argv[1],"-d")== 0)
        {
            return e_decode;
        }
        else
        {
            return e_unsupported;
        }
    
    return 0;





}   
    































