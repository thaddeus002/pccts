
                          Parr Research Corporation
                                    with
                  Purdue University Electrical Engineering
                                    and
                       University of Minnesota, AHPCRC

                                Terence Parr
                                Russell Quong
                                 Will Cohen
                                 Hank Dietz


A central place for information about PCCTS 1.33 is :

   [http://www.polhode.com/pccts.html](http://www.polhode.com/pccts.html)

The maintenance release is available from :

   [http://www.polhode.com/pccts133mr.zip](http://www.polhode.com/pccts133mr.zip)

There is a ready-to-run version for win32 for Microsoft Visual Studio
at the same site. It is available from :

   [http://www.polhode.com/win32.zip](http://www.polhode.com/win32.zip)

New users should visit http://www.polhode.com/pccts.html in
order to get the following document :

        "Notes For New Users of PCCTS"

This is a Postscript file of about 40 pages which is extremely 
useful for someone starting out. It is a based on 1.33mr21

When you have a little more experience, be sure to review the
following documents in the distribution kit:

        CHANGES_FROM_133.txt
        CHANGES_FROM_133_BEFORE_MR13.txt
        KNOWN_PROBLEMS.txt


## INSTALLATION (Unix)

1. Clone this repository : 

   > git clone https://github.com/thaddeus002/pccts.git

1. Go to the main pccts directory :

   > cd pccts

1. Build the tools :

   > make

   This will create in _bin_ directory :

    * antlr
    * dlg
    * sorcerer (not build by default)
    * genmk (not build by default)

1. Copy the tools to `/usr/bin` or `/usr/local/bin` if you like. If you
   don't wish to then add `pccts/bin` to your path.

1. To get an up-to-date list of program options execute the
   program with no command line options. To get up-to-date
   documentation read `CHANGES_FROM_133*.txt` and 
   `KNOWN_PROBLEMS.txt`
   
1. You need not create a library. The makefile created by genmk
   assumes that the files are not part of a library. 

   If you wish to create a library from elements of pccts/h:

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

## LICENSE

*pccts* is under public domain. See [LICENSE](LICENSE).
