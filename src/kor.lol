gmake -j8 -C examples/;
gmake -j8 -C userprog/;
cd userprog/build/;
pintos -p ../../examples/sumargv -a sumargv -v -k --fs-disk=2 -- -f -q run 'sumargv 1 2 3 4 5';
cd ../..
