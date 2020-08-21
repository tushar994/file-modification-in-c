# Assignment 1 for OS and N

## Question 1
**assumptions made**

If the right number of arguments are not given, then the program tells you to correct that and exits

If the Assignments folder is already made, nothing is done to it (even if the permission for the folder is not the permission deesired, it is not changed), and creating the appropriate file in it is attempted

If the file ./Assignments/<filename> already exists, then it is overwritten, but the permissions are left unchanged

Every character (\n and such) are treated as normal characters, so for example:-
if the sentence to be reversed is "okay bro\n"
it will be reverse to "\norb yako".


## Question 2
If the right number of arguments are not given, then the program tells you to correct that and exits without doing any processing.

If a file given doesn't exist, then its permissions are simply not printed, it does not check if one is the reverse of the other.
It prints which files/directory does not exist/can't be opened.

