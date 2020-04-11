# Protsh

Protish is a humble unix shell aimed to be nothing more than educative.

Feel free to use the code as you wish and ,if you think you can improve it, go for it!

The test folder contains a number of scripts that may be a simple introduction to common syscalls. Using the command:
```
man 2 <syscall>
```
shall give you a very good description (for the most part) of the syscall, and some include a C code exemple too.

# Installation

It will work on unix systems, you can use the Makefile:

```
make
```

or you can use another compiler as you wish.

# Options

Some new options were added in version 1.2:

## make options

Make now includes two new options:

A debug version of the shell that does only a single execution of:
```
ls -lah
```
or reads from a file like:
```
protsh cmd
```
to compile run:
```
make debug
```
Another option is the install one:
```
sudo make install
```
this is going to install the binary in the $PATH, still it is not recommended as protsh is not an useful shell

## protsh options

It was mentioned above but for the sake of completeness, protsh if compiled with debug mode can read its commands from a file
```
protsh cmdfile
```
