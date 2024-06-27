[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# Description

Part A is implemented completely considering all the things mentioned in the project description.

`make` makes an .out file that must be executed to run the shell.

# Assumptions

1. All the paths output is absolute.

2. Comments are very minimal.

3. invalid flags fro peek gives invalid path.

4. pastevents considers "A ;B" same sa "A ; B" and also stores invalid commands that returned abnormally.

5. Any command that contains tries to execute "pastevents" isn't included in pastevents file except pastevents execute which is added after replacing it with the command it executes .

6. Pastevents file is stored in home directory of the shell. So home directory of shell must have edit permission and also if the home of shell is changed, pastevents isnt carried over. We can change this behaviour by changing EVENT_FILE_PATH in execute.h with
   "/home/%s/Documents/Events.txt, pwd->userName" or any other absolute path.

7. Background process makes a temp file in home directory of the shell. Hence it prints output of a background process after u enter another command id the bacground process takes too long to execute. This behaviour can be prevented and made to work like bash terminal by defining IGNORE_REDIRECT in pwd_.h `#define IGNORE_REDIRECT`. When temfiles are created, ```exit``` command deletes all temp files, however they remain if shell is exited by other means such as keyboard interupt

8. Executable path in proclore is absolute not relative.

9. seek does not accept flags like -fd -fe etc i.e. one hyphen can be only followed by one flag.

10. seek gives invalid use if no target to search is given is kept empty.

11. 4095 is the limit of length of almost all strings. Paths cannot exceed this limit. Output of seek can go upto about 50000 characters. All these can be changed by cahnging the OUTPUT_LIMIT and INPUT_LIMIT in .h files.

12. sudo permissions might be required to run some of the commands, permissions are only checked where it is asked to be checked in the project description.

13. & will not work with any of the user-defined commands.

14. vim & doesnt work properly.

15. exit is not stored in pastevents.

16. Ctrl+C, Ctrl+D doesnt work in raw mode of neonate.

17. iMan only outputs NAME DESCRIPTION and SYNOPSIS and parsing is basic.

18. using ctrlZ stops the process and pushes it to background queue (backgroundProcesses).

19. Assumed /proc/loadvvg file contains the latest pid.