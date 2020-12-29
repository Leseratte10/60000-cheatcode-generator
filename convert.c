 /*   
    Copyright (C) 2020  Florian Bach ("Leseratte")

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, version 3.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/



#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#ifdef WINDOWS
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif
#include <stdlib.h>
#include <string.h>

void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);

#ifdef WINDOWS

void *
memmem (const void *haystack, size_t haystack_len, const void *needle,
	size_t needle_len)
{
  const char *begin;
  const char *const last_possible
    = (const char *) haystack + haystack_len - needle_len;

  if (needle_len == 0)
    /* The first occurrence of the empty string is deemed to occur at
       the beginning of the string.  */
    return (void *) haystack;

  /* Sanity check, otherwise the loop might search through the whole
     memory.  */
  if (__builtin_expect (haystack_len < needle_len, 0))
    return NULL;

  for (begin = (const char *) haystack; begin <= last_possible; ++begin)
    if (begin[0] == ((const char *) needle)[0] &&
	!memcmp ((const void *) &begin[1],
		 (const void *) ((const char *) needle + 1),
		 needle_len - 1))
      return (void *) begin;

  return NULL;
}

#endif

int main(int argc, char * argv[]) {
    
    
        if (argc != 2) {
            printf("Error 60000 fix generator (v2) by Leseratte\n");
            printf("This will (try to) generate an anti-error-60000 code\n");
            printf("for the game whose main.dol you enter as parameter.\n");
            printf("Usage: %s <game.dol>\n", argv[0]);
            return 0;
        }
        
        FILE * fd = fopen(argv[1], "r");

        if (!fd) {
            printf("can't open file '%s'\n", argv[1]);
            return 1;
        }
        
        // seek to T1 file offset: 
        fseek(fd, 0x04, SEEK_SET);
        int t1_file_offset = 0; 
        
        // read T1 file offset
        if (fread(&t1_file_offset, 4, 1, fd) < 0) {
            printf("error reading");
            return 1;
        };   
        
        t1_file_offset = ntohl(t1_file_offset);
        //printf("T1 is at 0x%08x (file)\n", t1_file_offset);
        
        
        // seek to T1 mem offset: 
        fseek(fd, 0x4c, SEEK_SET);
        int t1_mem_offset = 0; 
        
        // read T1 mem offset
        if (fread(&t1_mem_offset, 4, 1, fd) < 0) {
            printf("error reading");
            return 1;
        };   
        
        t1_mem_offset = ntohl(t1_mem_offset);
        //printf("T1 is at 0x%08x (mem)\n", t1_mem_offset);
        
        
        // seek to T1 size
        fseek(fd, 0x94, SEEK_SET);
        int t1_size = 0; 
        
        // read T1 size
        if (fread(&t1_size, 4, 1, fd) < 0) {
            printf("error reading");
            return 1;
        };   
        
        t1_size = ntohl(t1_size);
        //printf("T1 size: 0x%08x\n", t1_size);
        
        
        
        // Okay, now we got all the crap data. 
        // t1_file_offset is the T1 offset into the DOL file
        // t1_mem_offset is the T1 offset in memory
        // t1_size is (duh) the size of T1
        
        // Now get some memory and read T1.
        
        char * T1_section = malloc(t1_size); 
        
        if (T1_section == 0) {
            printf("can't malloc\n");
            return 1;
        }
        
        // seek to T1 size
        fseek(fd, t1_file_offset, SEEK_SET);
        
        // read T1 size
        if (fread(T1_section, 1, t1_size, fd) < 0) {
            printf("error reading");
            return 1;
        };   
        
        
        // Okay, so now T1_section should contain the T1 section. 
        // Let's start searching for crap. 
        
        
        
        char search_GetUserId_v1[12] =    {0x80, 0x64, 0x01, 0xb0, 
                                        0x80, 0x84, 0x01, 0xb4, 
                                        0x4e, 0x80, 0x00, 0x20};
                                        
                                        
        char search_GetUserId_v2[24] =  {0x54, 0x60, 0x00, 0x29, 
                                        0x40, 0x82, 0x00, 0x14, 
                                        0x80, 0x1e, 0x00, 0x04, 
                                        0x54, 0x00, 0x00, 0x28, 
                                        0x7c, 0x00, 0x1b, 0x78, 
                                        0x90, 0x1e, 0x00, 0x04};
                                  
                                     
        char search_loginStatePtr_v1[36] = { 0x2c, 0x03, 0x00, 0x00, 
                                            0x41, 0x82, 0x00, 0x18, 
                                            0x80, 0x03, 0x00, 0x04, 
                                            0x2c, 0x00, 0x00, 0x05, 
                                            0x40, 0x82, 0x00, 0x0c, 
                                            0x38, 0x60, 0x00, 0x01, 
                                            0x4e, 0x80, 0x00, 0x20, 
                                            0x38, 0x60, 0x00, 0x00, 
                                            0x4e, 0x80, 0x00, 0x20}; 
        
        char search_loginStatePtr_v2[18] = {0x38, 0xa0, 0x00, 0x01,
                                         0x38, 0x00, 0x00, 0x00, 
                                         0x38, 0x60, 0x00, 0x01, 
                                         0x90, 0xa4, 0x00, 0x04, 
                                         0x80, 0x8d};
                                         
        char search_loginStatePtr_v3[9] = {0x38, 0x64, 0x00, 0x4c, 
                                            0x38, 0x84, 0x01, 0x4c, 
                                            0x48};

 

                                         
        char search_InvalidGSID_v1[21] = {0x2c, 0x00, 0x00, 0x00, 
                                      0x41, 0x82, 0x00, 0x94, 
                                      0x3c, 0x80, 0xff, 0xff, 
                                      0x38, 0x60, 0x00, 0x06, 
                                      0x38, 0x84, 0x15, 0xa0, 
                                      0x4b};
                                      
        char search_InvalidGSID_v2[21] = {0x2c, 0x00, 0x00, 0x00, 
                                      0x41, 0x82, 0x00, 0xa0, 
                                      0x3c, 0x80, 0xff, 0xff, 
                                      0x38, 0x60, 0x00, 0x06, 
                                      0x38, 0x84, 0x15, 0xa0, 
                                      0x4b};
                                      
        char search_InvalidGSID_v3[21] = {0x2c, 0x00, 0x00, 0x00, 
                                     0x41, 0x82, 0x00, 0x9c, 
                                     0x3c, 0x80, 0xff, 0xff, 
                                     0x38, 0x60, 0x00, 0x06, 
                                     0x38, 0x84, 0x15, 0xa0, 
                                     0x4b};
                                     
        int found_GetUserId = 0; 
        int found_loginStatePtr = 0; 
        int found_InvalidGSID = 0; 
        
        
        // find GetUserId: 

        int is_r13_reference = 0;           // 0: unknown, 1: r13 ref, 2: absolute addr.
        int final_stpLoginCnt = 0; 
        
        char * GetUserID = memmem(T1_section, t1_size, search_GetUserId_v1, 12);
        char * patchAtAddress = memmem(T1_section, t1_size, search_InvalidGSID_v1, 21);
        char * lg_state_ptr = memmem(T1_section, t1_size, search_loginStatePtr_v1, 36); // replace

        if (lg_state_ptr != 0) {
            unsigned int pre1 = *(int*)((char*)lg_state_ptr - 8);
            unsigned int pre2 = *(int *)((char*)lg_state_ptr - 4);

            pre1 = ntohl(pre1);
            pre2 = ntohl(pre2);

            if (pre1 >> 16 == 0x3c60 && pre2 >> 16 == 0x8063) {
                // This game does not use r13. 
                final_stpLoginCnt = (pre1 & 0xffff) << 16;

                int next_part = (pre2 & 0xffff);

                if (next_part >= 0x8000)
                    final_stpLoginCnt -= 0x10000; 

                final_stpLoginCnt += next_part;   
                is_r13_reference = 2;
            }
            else if (pre2 >> 16 == 0x806d) {
                // This game does use r13
                is_r13_reference = 1;
                final_stpLoginCnt = pre2;
            }
            else {
                is_r13_reference = 0;
                lg_state_ptr = 0;
            }

        }
        
        
        int branch_back_code = 0x48000070;
        
        if (GetUserID == 0) {
            // try heuristics
            GetUserID = memmem(T1_section, t1_size, search_GetUserId_v2, 24);
            if (GetUserID != 0) {
                found_GetUserId = 2; 
                // We found the call site. Now let's follow the branch: 
                int branch_asm = ntohl(*(int *)(GetUserID - 4)); 
                branch_asm &= 0xffff; 
                branch_asm -= 1; 
                GetUserID += branch_asm + 4;
            }
        }
        else found_GetUserId = 1; 
       
        if (is_r13_reference == 0) { 
            // try heuristics: 
            lg_state_ptr = memmem(T1_section, t1_size, search_loginStatePtr_v2, 18);
            if (lg_state_ptr == 0) {
                lg_state_ptr = memmem(T1_section, t1_size, search_loginStatePtr_v3, 9);
                if (lg_state_ptr != 0) {
                    found_loginStatePtr = 3;
                    is_r13_reference = 3;
                }
            }
            else {
                found_loginStatePtr = 2; 
                is_r13_reference = 3;
            }
        }
        else found_loginStatePtr = 1; 
        
        if (patchAtAddress == 0) {
            // try heuristics: 
            patchAtAddress = memmem(T1_section, t1_size, search_InvalidGSID_v2, 21);
            if (patchAtAddress) {
                patchAtAddress -= 32;
                branch_back_code = 0x4800009c; 
                found_InvalidGSID = 2; 
            }
            else {
                patchAtAddress = memmem(T1_section, t1_size, search_InvalidGSID_v3, 21);
                if (patchAtAddress) {
                    patchAtAddress -= 32;
                    branch_back_code = 0x48000098; 
                    found_InvalidGSID = 3; 
                }
            }
        }
        else found_InvalidGSID = 1; 

        if (GetUserID == 0) {
            printf("Can't find GetUserID\n"); 
            printf("Either this game doesn't have an online mode, or it is\n");
            printf("not yet supported by this tool. If this game is supported\n");
            printf("on Wiimmfi, please contact Leseratte.\n");
            return 4;
        }
        
        if (lg_state_ptr == 0) {
            printf("Can't find login state ptr\n"); 
            printf("Either this game doesn't have an online mode, or it is\n");
            printf("not yet supported by this tool. If this game is supported\n");
            printf("on Wiimmfi, please contact Leseratte.\n");
            return 4;
        }
        
        if (patchAtAddress == 0) {
            printf("Can't find GSID code\n"); 
            printf("Either this game doesn't have an online mode, or it is\n");
            printf("not yet supported by this tool. If this game is supported\n");
            printf("on Wiimmfi, please contact Leseratte.\n");
            return 4;
        }
        
        printf("Found GetUserID v%d, loginStatePtr v%d.%d, InvalidGSID v%d\n", 
                found_GetUserId, found_loginStatePtr, is_r13_reference, found_InvalidGSID);
        
        GetUserID -= 8;     // this now points to the function start
        patchAtAddress -= 20;

          
        /*  
        hexDump("GetUserID", GetUserID, 0x20); 
        hexDump("accessLoginStatePtr", accessLoginStatePtr, 0x20); 
        hexDump("patchAtAddress", patchAtAddress, 0x20); 
        */
        
        int wii_GetUserID = 0; 
        int wii_patchAtAddress = 0; 
        
        wii_GetUserID = (GetUserID - T1_section) + t1_mem_offset; 
        
        wii_patchAtAddress = (patchAtAddress - T1_section) + t1_mem_offset; 

        printf("Wii offsets: %08x (0) %08x\n", wii_GetUserID, wii_patchAtAddress);
        printf("is_r13: %d, data: %x\n", is_r13_reference, final_stpLoginCnt);
        
        // Determine branch to DWCi_Auth_GetConsoleUserId (bl):            
        int branch_value = 0x48000001 + (((wii_GetUserID) - (wii_patchAtAddress)) & 0x3ffffff);
        
        
        // 0: unknown, 1: r13 ref, 2: absolute addr.
        int access1, access2;
        if (is_r13_reference == 1) {
            // r13 ref.
            access1 = 0x80ed0000 | (final_stpLoginCnt & 0xffff); 
            access2 = 0x60000000;
        }
        else if (is_r13_reference == 2) {
            // absolute addr.
            access1 = 0x3ce00000 + ((unsigned int)final_stpLoginCnt >> 16);   // lis r7, (final_stpLoginCnt >> 16)

            int next_off = final_stpLoginCnt & 0xffff;
            if (next_off >= 0x8000) {
                access1++;
            }
            
            access2 = 0x80e70000 + next_off;    // lwz r7, X(r7);

        }
        else if (is_r13_reference == 3) {
            // r13 ref, but still need to convert the addr. 
            char * accessLoginStatePtr = (char*)(lg_state_ptr - 4); 
            int wii_accessLoginStatePtr = 0; 
            wii_accessLoginStatePtr = (accessLoginStatePtr - T1_section) + t1_mem_offset; 
            int access = htonl(*(int *)accessLoginStatePtr); 
                
            // Patch that code to register 7: 
            access1 = 0x80ed0000 | (access & 0xffff); 
            access2 = 0x60000000;

        }
        else {
            printf("err: r13 ref = %d\n", is_r13_reference);
            return 2;
        }


        // Get the code to access the login state ptr:
 
        

        printf("=====================================\n");
        printf("Analysis done. \n");



        // Final: Print patch code: 
        printf("[60000 fix - Leseratte]\n");
        // Print the branch to GetConsoleUserID: 
        printf("04%06x %08x\n", wii_patchAtAddress & 0xffffff, branch_value);
        // Print the C2 header: 
        printf("C2%06x %08x\n", (wii_patchAtAddress + 4) & 0xffffff, 7);
        // Print code: 
        printf("%08x %08x\n", access1, access2); 
        printf("%08x %08x\n", 0x60000000, 0x38000003); 
        printf("%08x %08x\n", 0x90070004, 0x60630800); 
        printf("%08x %08x\n", 0x90670040, 0x90870044); 
        printf("%08x %08x\n", 0x8087001c, 0x8064000c); 
        printf("%08x %08x\n", 0x90670048, 0x38600001); 
        printf("%08x %08x\n", 0x90640020, 0); 
        
        printf("04%06x %08x\n", (wii_patchAtAddress + 8) & 0xffffff, branch_back_code);
        
        
        fclose(fd);
}