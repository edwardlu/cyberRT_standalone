# cyberRT_standalone
  Apollo cyber is a small but strong framework aims at IPC & RPC. <br>
  Hope this could be a middle-ware to do better than AutoSar standard. <br>
  1,Based on Cyber-RT framework from apollo <br>
  2,CMakelists.txt is from the https://blog.csdn.net/qq_35066464 <br>
  3,Add glog gflag uuid into this repositry <br>
  4,Most of the Cyber-RT standalone project generates static library,but this CMakelists.txt make different, it generates libcyber.so <br>
  5,CO-Routin in userspace <br>
  6,Thread allocate to arrange the priority <br>

# Build and Install
  follow the instruction in the 3rd <br>
  than: <br>
  mkdir build <br>
  cd build <br>
  cmake .. <br>
  make -j32 <br>
  
# Changes
  1,remove poco libs for dynamic libs open and load <br>
