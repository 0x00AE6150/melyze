#pragma once

namespace elf {
    using Elf32_Addr = uint32_t;
    using Elf32_Off = uint32_t;
    using Elf32_Half = uint16_t;
    using Elf32_Word = uint32_t;
    using Elf32_Sword = int32_t;

    using Elf64_Addr = uint64_t;
    using Elf64_Off = uint64_t;
    using Elf64_Half = uint16_t;
    using Elf64_Word = uint32_t;
    using Elf64_Sword = int32_t;
    using Elf64_Xword = uint64_t;
    using Elf64_Sxword = int64_t;

    static const char s_elf_magic[] = {0x7f, 'E', 'L', 'F', '\0'};

    typedef struct Elf32_Ehdr {
        unsigned char e_ident[EI_NIDENT]; // ELF Identification bytes
        Elf32_Half e_type;                // Type of file (see ET_* below)
        Elf32_Half e_machine;   // Required architecture for this file (see EM_*)
        Elf32_Word e_version;   // Must be equal to 1
        Elf32_Addr e_entry;     // Address to jump to in order to start program
        Elf32_Off e_phoff;      // Program header table's file offset, in bytes
        Elf32_Off e_shoff;      // Section header table's file offset, in bytes
        Elf32_Word e_flags;     // Processor-specific flags
        Elf32_Half e_ehsize;    // Size of ELF header, in bytes
        Elf32_Half e_phentsize; // Size of an entry in the program header table
        Elf32_Half e_phnum;     // Number of entries in the program header table
        Elf32_Half e_shentsize; // Size of an entry in the section header table
        Elf32_Half e_shnum;     // Number of entries in the section header table
        Elf32_Half e_shstrndx;  // Sect hdr table index of sect name string table

        bool checkMagic() const {
            return (memcmp(e_ident, ElfMagic, strlen(ElfMagic))) == 0;
        }
    } Elf32_Ehdr;

    typedef struct Elf64_Ehdr {
        unsigned char e_ident[EI_NIDENT];
        Elf64_Half e_type;
        Elf64_Half e_machine;
        Elf64_Word e_version;
        Elf64_Addr e_entry;
        Elf64_Off e_phoff;
        Elf64_Off e_shoff;
        Elf64_Word e_flags;
        Elf64_Half e_ehsize;
        Elf64_Half e_phentsize;
        Elf64_Half e_phnum;
        Elf64_Half e_shentsize;
        Elf64_Half e_shnum;
        Elf64_Half e_shstrndx;

        bool checkMagic() const {
            return (memcmp(e_ident, ElfMagic, strlen(ElfMagic))) == 0;
        }
    } Elf64_Ehdr;

    // Section header.
    typedef struct Elf32_Shdr {
        Elf32_Word sh_name;      // Section name (index into string table)
        Elf32_Word sh_type;      // Section type (SHT_*)
        Elf32_Word sh_flags;     // Section flags (SHF_*)
        Elf32_Addr sh_addr;      // Address where section is to be loaded
        Elf32_Off sh_offset;     // File offset of section data, in bytes
        Elf32_Word sh_size;      // Size of section, in bytes
        Elf32_Word sh_link;      // Section type-specific header table index link
        Elf32_Word sh_info;      // Section type-specific extra information
        Elf32_Word sh_addralign; // Section address alignment
        Elf32_Word sh_entsize;   // Size of records contained within the section
    } Elf32_Shdr;

    // Section header for ELF64 - same fields as ELF32, different types.
    typedef struct Elf64_Shdr {
        Elf64_Word sh_name;
        Elf64_Word sh_type;
        Elf64_Xword sh_flags;
        Elf64_Addr sh_addr;
        Elf64_Off sh_offset;
        Elf64_Xword sh_size;
        Elf64_Word sh_link;
        Elf64_Word sh_info;
        Elf64_Xword sh_addralign;
        Elf64_Xword sh_entsize;
    } Elf64_Shdr;

    // Section types.
    enum : unsigned {
        SHT_NULL = 0,                         // No associated section (inactive entry).
        SHT_PROGBITS = 1,                     // Program-defined contents.
        SHT_SYMTAB = 2,                       // Symbol table.
        SHT_STRTAB = 3,                       // String table.
        SHT_RELA = 4,                         // Relocation entries; explicit addends.
        SHT_HASH = 5,                         // Symbol hash table.
        SHT_DYNAMIC = 6,                      // Information for dynamic linking.
        SHT_NOTE = 7,                         // Information about the file.
        SHT_NOBITS = 8,                       // Data occupies no space in the file.
        SHT_REL = 9,                          // Relocation entries; no explicit addends.
        SHT_SHLIB = 10,                       // Reserved.
        SHT_DYNSYM = 11,                      // Symbol table.
        SHT_INIT_ARRAY = 14,                  // Pointers to initialization functions.
        SHT_FINI_ARRAY = 15,                  // Pointers to termination functions.
        SHT_PREINIT_ARRAY = 16,               // Pointers to pre-init functions.
        SHT_GROUP = 17,                       // Section group.
        SHT_SYMTAB_SHNDX = 18,                // Indices for SHN_XINDEX entries.
        // Experimental support for SHT_RELR sections. For details, see proposal
        // at https://groups.google.com/forum/#!topic/generic-abi/bX460iggiKg
        SHT_RELR = 19,                        // Relocation entries; only offsets.
        SHT_LOOS = 0x60000000,                // Lowest operating system-specific type.
        // Android packed relocation section types.
        // https://android.googlesource.com/platform/bionic/+/6f12bfece5dcc01325e0abba56a46b1bcf991c69/tools/relocation_packer/src/elf_file.cc#37
        SHT_ANDROID_REL = 0x60000001,
        SHT_ANDROID_RELA = 0x60000002,
        SHT_LLVM_ODRTAB = 0x6fff4c00,         // LLVM ODR table.
        SHT_LLVM_LINKER_OPTIONS = 0x6fff4c01, // LLVM Linker Options.
        SHT_LLVM_CALL_GRAPH_PROFILE = 0x6fff4c02, // LLVM Call Graph Profile.
        SHT_LLVM_ADDRSIG = 0x6fff4c03,        // List of address-significant symbols
        // for safe ICF.
        SHT_LLVM_DEPENDENT_LIBRARIES = 0x6fff4c04, // LLVM Dependent Library Specifiers.
        SHT_LLVM_SYMPART = 0x6fff4c05,        // Symbol partition specification.
        SHT_LLVM_PART_EHDR = 0x6fff4c06,      // ELF header for loadable partition.
        SHT_LLVM_PART_PHDR = 0x6fff4c07,      // Phdrs for loadable partition.
        // Android's experimental support for SHT_RELR sections.
        // https://android.googlesource.com/platform/bionic/+/b7feec74547f84559a1467aca02708ff61346d2a/libc/include/elf.h#512
        SHT_ANDROID_RELR = 0x6fffff00,        // Relocation entries; only offsets.
        SHT_GNU_ATTRIBUTES = 0x6ffffff5,      // Object attributes.
        SHT_GNU_HASH = 0x6ffffff6,            // GNU-style hash table.
        SHT_GNU_verdef = 0x6ffffffd,          // GNU version definitions.
        SHT_GNU_verneed = 0x6ffffffe,         // GNU version references.
        SHT_GNU_versym = 0x6fffffff,          // GNU symbol versions table.
        SHT_HIOS = 0x6fffffff,                // Highest operating system-specific type.
        SHT_LOPROC = 0x70000000,              // Lowest processor arch-specific type.
        // Exception Index table
        SHT_ARM_EXIDX = 0x70000001U,
        // BPABI DLL dynamic linking pre-emption map
        SHT_ARM_PREEMPTMAP = 0x70000002U,
        //  Object file compatibility attributes
        SHT_ARM_ATTRIBUTES = 0x70000003U,
        SHT_ARM_DEBUGOVERLAY = 0x70000004U,
        SHT_ARM_OVERLAYSECTION = 0x70000005U,
        SHT_HEX_ORDERED = 0x70000000,         // Link editor is to sort the entries in
        // this section based on their sizes
        SHT_X86_64_UNWIND = 0x70000001,       // Unwind information

        SHT_MIPS_REGINFO = 0x70000006,        // Register usage information
        SHT_MIPS_OPTIONS = 0x7000000d,        // General options
        SHT_MIPS_DWARF = 0x7000001e,          // DWARF debugging section.
        SHT_MIPS_ABIFLAGS = 0x7000002a,       // ABI information.

        SHT_MSP430_ATTRIBUTES = 0x70000003U,

        SHT_HIPROC = 0x7fffffff,              // Highest processor arch-specific type.
        SHT_LOUSER = 0x80000000,              // Lowest type reserved for applications.
        SHT_HIUSER = 0xffffffff               // Highest type reserved for applications.
    };

    // Symbol table entries for ELF32.
    typedef struct Elf32_Sym {
        Elf32_Word st_name;     // Symbol name (index into string table)
        Elf32_Addr st_value;    // Value or address associated with the symbol
        Elf32_Word st_size;     // Size of the symbol
        unsigned char st_info;  // Symbol's type and binding attributes
        unsigned char st_other; // Must be zero; reserved
        Elf32_Half st_shndx;    // Which section (header table index) it's defined in
    } Elf32_Sym;

    // Symbol table entries for ELF64.
    typedef struct Elf64_Sym {
        Elf64_Word st_name;     // Symbol name (index into string table)
        unsigned char st_info;  // Symbol's type and binding attributes
        unsigned char st_other; // Must be zero; reserved
        Elf64_Half st_shndx;    // Which section (header tbl index) it's defined in
        Elf64_Addr st_value;    // Value or address associated with the symbol
        Elf64_Xword st_size;    // Size of the symbol
    } Elf64_Sym;

    // Program header for ELF32.
    typedef struct Elf32_Phdr {
        Elf32_Word p_type;   // Type of segment
        Elf32_Off p_offset;  // File offset where segment is located, in bytes
        Elf32_Addr p_vaddr;  // Virtual address of beginning of segment
        Elf32_Addr p_paddr;  // Physical address of beginning of segment (OS-specific)
        Elf32_Word p_filesz; // Num. of bytes in file image of segment (may be zero)
        Elf32_Word p_memsz;  // Num. of bytes in mem image of segment (may be zero)
        Elf32_Word p_flags;  // Segment flags
        Elf32_Word p_align;  // Segment alignment constraint
    } Elf32_Phdr;

    // Program header for ELF64.
    typedef struct Elf64_Phdr {
        Elf64_Word p_type;    // Type of segment
        Elf64_Word p_flags;   // Segment flags
        Elf64_Off p_offset;   // File offset where segment is located, in bytes
        Elf64_Addr p_vaddr;   // Virtual address of beginning of segment
        Elf64_Addr p_paddr;   // Physical addr of beginning of segment (OS-specific)
        Elf64_Xword p_filesz; // Num. of bytes in file image of segment (may be zero)
        Elf64_Xword p_memsz;  // Num. of bytes in mem image of segment (may be zero)
        Elf64_Xword p_align;  // Segment alignment constraint
    } Elf64_Phdr;

    // Segment types.
    enum {
        PT_NULL = 0,            // Unused segment.
        PT_LOAD = 1,            // Loadable segment.
        PT_DYNAMIC = 2,         // Dynamic linking information.
        PT_INTERP = 3,          // Interpreter pathname.
        PT_NOTE = 4,            // Auxiliary information.
        PT_SHLIB = 5,           // Reserved.
        PT_PHDR = 6,            // The program header table itself.
        PT_TLS = 7,             // The thread-local storage template.
        PT_LOOS = 0x60000000,   // Lowest operating system-specific pt entry type.
        PT_HIOS = 0x6fffffff,   // Highest operating system-specific pt entry type.
        PT_LOPROC = 0x70000000, // Lowest processor-specific program hdr entry type.
        PT_HIPROC = 0x7fffffff, // Highest processor-specific program hdr entry type.

        // x86-64 program header types.
        // These all contain stack unwind tables.
        PT_GNU_EH_FRAME = 0x6474e550,
        PT_SUNW_EH_FRAME = 0x6474e550,
        PT_SUNW_UNWIND = 0x6464e550,

        PT_GNU_STACK = 0x6474e551, // Indicates stack executability.
        PT_GNU_RELRO = 0x6474e552, // Read-only after relocation.

        PT_OPENBSD_RANDOMIZE = 0x65a3dbe6, // Fill with random data.
        PT_OPENBSD_WXNEEDED = 0x65a3dbe7,  // Program does W^X violations.
        PT_OPENBSD_BOOTDATA = 0x65a41be6,  // Section for boot arguments.

        // ARM program header types.
        PT_ARM_ARCHEXT = 0x70000000, // Platform architecture compatibility info
        // These all contain stack unwind tables.
        PT_ARM_EXIDX = 0x70000001,
        PT_ARM_UNWIND = 0x70000001,

        // MIPS program header types.
        PT_MIPS_REGINFO = 0x70000000,  // Register usage information.
        PT_MIPS_RTPROC = 0x70000001,   // Runtime procedure table.
        PT_MIPS_OPTIONS = 0x70000002,  // Options segment.
        PT_MIPS_ABIFLAGS = 0x70000003, // Abiflags segment.
    };
}