#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

int main(void) {
	// Program headers
	Elf64_Phdr programheaders[] =
	{
	};
	// Sections stuff
	const char section_names[] = "" "\0" ".text" "\0" ".data" "\0" ".symtab" "\0" ".strtab" "\0" ".shstrtab" "\0" ".rela.text" "\0";
	// Symbol table
	const char symbols_names[] = "" "\0" "foo" "\0" "lib.s" "\0";

	Elf64_Sym symbol_tabel[] =
	{
		{ // null
			.st_name = 0,
			.st_info = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE),
			.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT),
			.st_shndx = STN_UNDEF,
			.st_value = 0,
			.st_size = 0
		},
		{ // File
			.st_name = 5,
			.st_info = ELF64_ST_INFO(STB_LOCAL, STT_FILE),
			.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT),
			.st_shndx = SHN_ABS,
			.st_value = 0,
			.st_size = 0
		},
		{ // .text section
			.st_name = 0,
			.st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION),
			.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT),
			.st_shndx = 1, // .text
			.st_value = 0,
			.st_size = 0
		},
		{ // .data section
			.st_name = 0,
			.st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION),
			.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT),
			.st_shndx = 2, // .data
			.st_value = 0,
			.st_size = 0
		},
		{ // foo
			.st_name = 1,
			.st_info = ELF64_ST_INFO(STB_GLOBAL, STT_FUNC),
			.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT),
			.st_shndx = 1, // .text
			.st_value = 0, // function offset in section
			.st_size = 0
		}
	};

	Elf64_Rela rela[] =
	{
		{
			.r_offset 	= 0x00000000000a,
			.r_info		= ELF64_R_INFO(3, R_X86_64_64),
			.r_addend	= 0
		}
	};

	// Section data
	const char text[] = {
		// foo:
		"\xb8\x01\x00\x00\x00"			// mov    eax,0x1
		"\x48\x89\xc7"					// mov    rdi,rax
		"\x48\xbe\x00\x00\x00\x00\x00"  // movabs rsi,0x0 (mov rsi, nessage)
		"\x00\x00\x00"
		"\xba\x0d\x00\x00\x00"			// mov    edx,13
		"\x0f\x05"						// syscall
		"\xc3"							// ret
	};
	const char data[] = {
		// message:
		"Hello World!\n"
	};
	// Sections
	Elf64_Shdr sections[] =
	{
		{ // NULL
			.sh_name	  = 0,
			.sh_type	  = SHT_NULL,
			.sh_flags	  = 0,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 00000000,
			.sh_size	  = 0,
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 0,
			.sh_entsize	  = 0x0000000000000000
		},
		{ // .text
			.sh_name	  = 1,
			.sh_type	  = SHT_PROGBITS,
			.sh_flags	  = SHF_ALLOC | SHF_EXECINSTR,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 0x00000200,
			.sh_size	  = sizeof(text),
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 16,
			.sh_entsize	  = 0x0000000000000000
		},
		{ // .data
			.sh_name 	  = 7,
			.sh_type	  = SHT_PROGBITS,
			.sh_flags	  = SHF_ALLOC | SHF_WRITE,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 0x00000220,
			.sh_size	  = sizeof(data),
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 4,
			.sh_entsize	  = 0x0000000000000000
		},
		{ // .shstrtab
			.sh_name	  = 29,
			.sh_type	  = SHT_STRTAB,
			.sh_flags	  = 0,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 0x00000230,
			.sh_size	  = sizeof(section_names),
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 1,
			.sh_entsize	  = 0x0000000000000000
		},
		{ // .symtab
			.sh_name	  = 13,
			.sh_type	  = SHT_SYMTAB,
			.sh_flags	  = 0,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 0x00000270,
			.sh_size	  = sizeof(symbol_tabel),
			.sh_link	  = 5,
			.sh_info	  = sizeof(symbol_tabel)/sizeof(Elf64_Sym)-1, // the size of the table minus 1. Why you ask, idk
			.sh_addralign = 8,
			.sh_entsize	  = sizeof(Elf64_Sym)
		},
		{ // .strtab
			.sh_name	  = 21,
			.sh_type	  = SHT_STRTAB,
			.sh_flags	  = 0,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 0x00000300,
			.sh_size	  = sizeof(symbols_names),
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 1,
			.sh_entsize	  = 0x0000000000000000
		},
		{ // .rela.text
			.sh_name	  = 39,
			.sh_type	  = SHT_RELA,
			.sh_flags	  = 0,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 0x00000320,
			.sh_size	  = sizeof(rela),
			.sh_link	  = 4,
			.sh_info	  = sizeof(rela)/sizeof(Elf64_Rela),
			.sh_addralign = 8,
			.sh_entsize	  = sizeof(Elf64_Rela)
		}
	};

	// Elf header
	Elf64_Ehdr header = {
		.e_ident = {
			0x7F, 'E', 'L', 'F', // ELF MAGIC
			0x02, // CLASS: 64bit
			0x01, // DATA: little endian
			0x01, // ELF Version: the one and only
			0x00, // OS ABI: System V
			0x00, // EI_ABIVERSION (ignored by the linux kernel)
			// the rest is currently unused
		},
		.e_type = ET_REL,
		.e_machine = EM_X86_64,
		.e_version = EV_CURRENT,
		.e_entry = 0,
		.e_phoff = sizeof(programheaders) ? sizeof(Elf64_Ehdr) : 0,
		.e_shoff = 0x40,
		.e_flags = 0,
		.e_ehsize = sizeof(Elf64_Ehdr),
		.e_phentsize = sizeof(Elf64_Phdr),
		.e_phnum = sizeof(programheaders)/sizeof(Elf64_Phdr),
		.e_shentsize = sizeof(Elf64_Shdr),
		.e_shnum = sizeof(sections)/sizeof(Elf64_Shdr),
		.e_shstrndx = 3
	};

	char program[0x340] = {};
	// write ELF header
	memcpy(program, &header, sizeof(header));
	// write program headers
	memcpy(program+header.e_phoff, programheaders, sizeof(programheaders));
	// write sections
	memcpy(program+header.e_shoff, sections, sizeof(sections));
	
	// write sections data
	// write .text
	memcpy(program+sections[1/*.text*/].sh_offset, text, sizeof(text));
	// write .data
	memcpy(program+sections[2/*.data*/].sh_offset, data, sizeof(data));
	// write section names
	memcpy(program+sections[3/*.shstrtab*/].sh_offset, section_names, sizeof(section_names));
	// write symbol table
	memcpy(program+sections[4/*.symtab*/].sh_offset, symbol_tabel, sizeof(symbol_tabel));
	// write symbol string table
	memcpy(program+sections[5/*.strtab*/].sh_offset, symbols_names, sizeof(symbols_names));
	// write rela
	memcpy(program+sections[6/*.rela.text*/].sh_offset, rela, sizeof(rela));

	// write program to file
	FILE* out = fopen("build/out.o", "wb");
	fwrite(program, 1, sizeof(program), out);
	fclose(out);
	return 0;
}