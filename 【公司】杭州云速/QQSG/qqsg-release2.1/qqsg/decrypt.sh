rm decrypt
gcc -o decrypt decrypt.c profile.c lock.c md5.c
./decrypt -f packet
