# cyberRT_standalone
Apollo cyber is a small but strong framework aims at IPC & RPC.
Hope this could be a middle-ware to do better than AutoSar standard.
1, Based on Cyber-RT framework from apollo
2, CMakelists.txt is from the https://blog.csdn.net/qq_35066464
3, Add glog gflag uuid into this repositry
4, Most of the Cyber-RT standalone project generates static library,but this CMakelists.txt make different, it generates libcyber.so
5, CO-Routin in userspace
6, Thread allocate to arrange the priority

# Build and Install
follow the instruction in the 3rd
than : 
mkdir build
cd build
cmake ..
make -j32

