# sish

Just a <b>si</b>mple <b>sh</b>ell developed as a class project. Originally developed to work in MINIX. The code was based of templates given to us in class, lex.c was provided, and shell.c was only an input loop that called the lexer. The design document was worked on with my group, and the instructions.pdf was provided by the professor, Nathan Whitehead.

## Building

A simple `make` (same as `make build`) should do the trick. This will create a "bin" and a "build" directory. The "bin" directory contains the shell file, called sish, and the "build" directory contains various temporary build files. `make clean` will remove build, and `make spotless` will remove all generated files.

## Extra Features

I only implemented a few extra features. The prompt has the present working directory and colors, and I implemented a simple version of `cd` for changing directories. `cd` only takes one argument (no zero argument version), but should check for bad paths.
