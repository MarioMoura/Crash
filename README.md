# Crash - The Crashable Shell

Crash is a humble unix shell tailored to be nothing more than educative.

Feel free to use the code as you wish and ,if you think you can improve it, go for it!

The test folder contains a number of scripts that may be a simple introduction to common syscalls. Using the command:
```
man 2 <syscall>
```
shall give you a very good description (for the most part) of the syscall, and sometimes include a C code exemple too.

# Compilation

It will work on unix systems, you can use the Makefile:

```
make
```

or you can use another compiler as you wish.

# Usage

Crash is inspired by a normal shell. The main difference is that pipes takes precedence. Like:
```
echo testing > file | wc -c
```
In a normal shell wc will produce an output of zero, but in Crash the file will be created but empty.

# Options

I decided to remove debug options from version 1.2. Now debug operations are done by the compiler.

## make options

Now with the manual PATH management implemented, protsh will use the environment variable as its PATH, this cannot be changed from inside the shell yet, but it can be altered by writing in the Makefile.

Debug mode that disables compiler optimizations, and produce debug information, to compile that way run:
```
make debug
```
Another option is the install one:
```
sudo make install
```
this is going to install the binary in the $PATH, still it is not recommended as protsh is not an useful shell.
