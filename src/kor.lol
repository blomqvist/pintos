gmake -j8 -C examples/;
gmake -j8 -C userprog/;
cd userprog/build/;
pintos -p ../../examples/file_syscall_tests -a sys_test -v -k --fs-disk=2 -- -f -q run 'sys_test testing one two three';
cd ../..
