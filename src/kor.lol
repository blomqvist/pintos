gmake -j8 -C examples/;
gmake -j8 -C userprog/;
cd userprog/build/;
pintos -v -k --fs-disk=2 --qemu -p ../../examples/longrun -a longrun -p ../../examples/dummy -a dummy -- -f -q run 'longrun 1 1'
cd ../..
