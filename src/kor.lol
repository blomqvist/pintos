gmake -j8 -C examples/;
gmake -j8 -C userprog/;
cd userprog/build/;
pintos -p ../../examples/longrun_nowait -a longr -v -k --fs-disk=2 -- -f -q run 'longr 5 5';
cd ../..
