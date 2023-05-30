# SimpleBashUtils
Development of Bash text utilities: cat, grep.


The programs are developed in C language of C11 standard using gcc compiler.
The program code of the cat and grep are located on the develop branch in the src/cat/ and src/grep/ folders, respectively
Writing code is followed the Google style.
The programs executable files with command line arguments
The programs are built with Makefile with appropriate targets: s21_cat, s21_grep
Integration tests covers of all flags, including their pair combinations (on;y for grep e.g. -iv, -in) variants and input values, based on a comparison with the behavior of real Bash utilities


## Cat Options: 
-b (GNU: --number-nonblank)
numbers only non-empty lines

-e implies -v (GNU only: -E the same, but without implying -v)
but also display end-of-line characters as $

-n (GNU: --number)
number all output lines

-s (GNU: --squeeze-blank)
squeeze multiple adjacent blank lines

-t implies -v (GNU: -T the same, but without implying -v)
but also display tabs as ^I

## Grep Options

-e
pattern

-i
Ignore uppercase vs. lowercase.

-v
Invert match.

-c
Output count of matching lines only.

-l
Output matching files only.

-n
Precede each matching line with a line number.

-h
Output matching lines without preceding them by file names.

-s
Suppress error messages about nonexistent or unreadable files.

-f file
Take regexes from a file.

-o
Output the matched parts of a matching line.

