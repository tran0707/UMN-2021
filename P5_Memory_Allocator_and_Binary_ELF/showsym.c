// Template for parsing an ELF file to print its symbol table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

int DEBUG = 0;                  // controls whether to print debug messages

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s [-d] <file>\n",argv[0]);
    return 0;
  }

  char *objfile_name = argv[1];

  // check for debug mode
  if(argc >=3){
    if(strcmp("-d",argv[1])==0){
      DEBUG = 1;                // set debug flag if -d is passed on command line
      objfile_name = argv[2];
    }
    else{
      printf("incorrect usage\n");
      return 1;
    }
  }

  // Open file descriptor and set up memory map for objfile_name
  int fd = open(objfile_name,O_RDONLY);
  struct stat stat_buf;
  fstat(fd,&stat_buf);
  int size = stat_buf.st_size;
  char *file_bytes = mmap(NULL,size,PROT_READ,MAP_SHARED,fd,0);
  // CREATE A POINTER to the intial bytes of the file which are an ELF64_Ehdr struct
  //Elf64_Ehdr *ehdr = NULL;
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *) file_bytes;
  // CHECK e_ident field's bytes 0 to for for the sequence {0x7f,'E','L','F'}.
  // Exit the program with code 1 if the bytes do not match
  int e_ident_matches =
      ehdr->e_ident[0] == 0x7f &&
      ehdr->e_ident[1] == 'E'  &&
      ehdr->e_ident[2] == 'L'  &&
      ehdr->e_ident[3] == 'F'   ;
  if(!e_ident_matches){
      printf("Magic bytes wrong, this is not an ELF file");
      exit(1);
  }
  // PROVIDED: check for a 64-bit file
  if(ehdr->e_ident[EI_CLASS] != ELFCLASS64){
    printf("Not a 64-bit file ELF file\n");
    return 1;
  }

  // PROVIDED: check for x86-64 architecture
  if(ehdr->e_machine != EM_X86_64){
    printf("Not an x86-64 file\n");
    return 1;
  }

  // DETERMINE THE OFFSET of the Section Header Array (e_shoff), the
  // number of sections (e_shnum), and the index of the Section Header
  // String table (e_shstrndx). These fields are from the ELF File
  // Header.
     int e_shoff_offset = ehdr->e_shoff;
     int e_shnum_offset = ehdr->e_shnum;
     int e_shstrndx_offset = ehdr->e_shstrndx;

  // Set up a pointer to the array of section headers. Use the section
  // header string table index to find its byte position in the file
  // and set up a pointer to it.
  Elf64_Shdr *shdr = (Elf64_Shdr *)(file_bytes + e_shoff_offset);

  // Search the Section Header Array for the secion with name .symtab
  // (symbol table) and .strtab (string table).  Note their positions
  // in the file (sh_offset field).  Also note the size in bytes
  // (sh_size) and and the size of each entry (sh_entsize) for .symtab
  // so its number of entries can be computed.
  int found_symtab = 0;
  int found_strtab = 0;
  size_t sym_offset = -1;
  size_t sym_size = -1;
  size_t sym_entsize = -1;
  size_t str_offset = -1;
  for(int i=0; i< (e_shnum_offset); i++){
    char *str_compare = (char*)(file_bytes
                        + shdr[e_shstrndx_offset].sh_offset + shdr[i].sh_name);
    if(strcmp(str_compare,".symtab") == 0){
        found_symtab = 1;
        sym_offset = (shdr[i].sh_offset);
        sym_size = (shdr[i].sh_size);
        sym_entsize = (shdr[i].sh_entsize);
    }
    if(strcmp(str_compare,".strtab") == 0){
      found_strtab = 1;
      str_offset = (shdr[i].sh_offset);
      }
  }
  if(found_symtab == 0){
    printf("Couldn't find symbol table\n");
    return 1;
  }
  if(found_strtab == 0){
    printf("Couldn't find string table\n");
    return 1;
  }
  size_t num_entry = sym_size/sym_entsize;
  // PRINT byte information about where the symbol table was found and
  // its sizes. The number of entries in the symbol table can be
  // determined by dividing its total size in bytes by the size of
  // each entry.
  printf("Symbol Table\n");
  printf("- %lu bytes offset from start of file\n",sym_offset);
  printf("- %lu bytes total size\n",sym_size);
  printf("- %lu bytes per entry\n",sym_entsize);
  printf("- %lu entries\n",num_entry);


  // Set up pointers to the Symbol Table and associated String Table
  // using offsets found earlier.
  Elf64_Sym *sym = (Elf64_Sym*)(file_bytes + sym_offset);
    // Print column IDs for info on each symbol
  printf("[%3s]  %8s %4s %s\n",
         "idx","TYPE","SIZE","NAME");
  // Iterate over the symbol table entries
  char *found_name;
  char *found_type;
  size_t str_size_return;
  for(int i=0; i < num_entry; i++){
    // Determine size of symbol and name. Use <NONE> name has zero
    // length.
    char *found_name_offset = (char*)(file_bytes + str_offset + sym[i].st_name);
    str_size_return = sym[i].st_size;
    if(strlen(found_name_offset)==0){
      found_name = "<NONE>";
    }
    else{
      found_name = found_name_offset;
    }
    // Determine type of symbol. See assignment specification for
    // fields, macros, and definitions related to this.
    unsigned char typec = ELF64_ST_TYPE(sym[i].st_info);
    switch (typec) {
      case STT_NOTYPE:
        found_type = "NOTYPE";break;
      case STT_OBJECT:
        found_type = "OBJECT";break;
      case STT_FUNC:
        found_type = "FUNC";break;
      case STT_FILE:
        found_type = "FILE";break;
      case STT_SECTION:
        found_type = "SECTION";break;
    }
      // Print symbol information
    printf("[%3d]: %8s %4lu %s\n",i,found_type,str_size_return,found_name);
  }
  // Unmap file from memory and close associated file descriptor
  munmap(file_bytes, size);
  close(fd);
  return 0;
}
