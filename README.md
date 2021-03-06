# Thaddeus' PCCTS

This project is based on a fork of the Purdue Compiler Construction Toolset ([PCCTS 1.33](http://www.polhode.com/pccts.html)), first developed by Terence Parr.

## What is this?

pccts is made of various programs :

 * antlr
 * dlg
 * sorcerer (not build by default)

For more details on this programs, see "Documentation section".

## Installation

1. Clone this repository :

        $ git clone https://github.com/thaddeus002/pccts.git

1. Go to the main pccts directory :

        $ cd pccts

1. Build the tools :

        $ make

1. install :

        $ make install

1. To get an up-to-date list of program options execute the
   program with no command line options. To get up-to-date
   documentation read `CHANGES_FROM_133*.txt` and
   `KNOWN_PROBLEMS.txt`

1. If you wish to create a library from elements of pccts/h:

   If the first letter of the filename is lowercase (uppercase) it is
   related to the code generated using the pccts C mode (C++ mode).
   Some of the .c and .cpp files in the h directory are not meant to
   be placed in a library and will not compile because they are meant
   to be #include in pccts generated files which are grammar specific.

   For C++ users place the following elements in the library:

    * AParser.cpp
    * ASTBase.cpp
    * ATokenBuffer.cpp
    * BufFileInput.cpp (optional)
    * DLexerBase.cpp
    * PCCTSAST.cpp
    * SList.cpp

## License

*pccts* is under public domain. See [LICENSE](LICENSE).

## Documentation

New users should visit [the project 's page](https://thaddeus002.github.io/pccts) in
order to get the following document :

    "Notes For New Users of PCCTS"

This is a Postscript file of about 40 pages which is extremely
useful for someone starting out. It is a based on 1.33mr21

When you have a little more experience, be sure to review the
following documents in the distribution kit:

    CHANGES_FROM_133.txt
    CHANGES_FROM_133_BEFORE_MR13.txt
    KNOWN_PROBLEMS.txt
