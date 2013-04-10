gmake -j8 -C examples/;
gmake -j8 -C userprog/;
cd userprog/build/;
pintos -p ../../examples/file_syscall_tests -a systest -v -k --fs-disk=2 -- -f -q run 'systest 1 2 3 4 5';
cd ../..
