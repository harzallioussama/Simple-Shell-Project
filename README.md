# SSP - Simple Shell Project
## Overview
SSP is a basic shell implemented in C, designed just for fun and educational purposes. It supports a few built-in commands along with the ability to execute external programs.
## Features
- Change Directory: `cd`
- List Directory Contents: `ls`
- Rename Files and Directories: `rename`
- Display Help Information: `help`
- Quit the Shell: `quit`
## Getting Started
1. Clone the repository:
   ```bash
   git clone git@github.com:harzallioussama/Simple-Shell-Project.git
   gcc -o ssp ssp.c
   ./ssp
   ```
2. Usage:
  ```bash
  cd <directory_path>
  ls
  rename <old_name> <new_name>
  help
  quit
  ```
##External Programs
You can also execute external programs by entering their names along with any required arguments.

```bash
g++ -o prog prog.cpp
```

In the shell prompt :
```bash
./prog Hello
```
