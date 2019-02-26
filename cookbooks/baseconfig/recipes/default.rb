#
# Cookbook:: baseconfig
# Recipe:: default
#
# Copyright:: 2019, The Authors, All Rights Reserved.

execute 'install_build-essential' do
  command 'sudo apt-get install -y build-essential'
end

execute 'install_Ncurses' do
  command 'sudo apt-get install -y libncurses5-dev'
end

apt_repository 'get_gcc-7' do
  uri 'ppa:ubuntu-toolchain-r/test'
end

execute 'install_gcc-7' do
  command 'sudo apt-get update'
  command 'sudo apt-get install -y gcc-7 g++-7'
end

execute 'set_gcc-7_as_default' do
  command 'sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 70 --slave /usr/bin/g++ g++ /usr/bin/g++-7'
end

bash 'install_cmake' do
  user 'root'
  cwd '/tmp'
  code <<-EOH
  wget -nv https://github.com/Kitware/CMake/releases/download/v3.13.3/cmake-3.13.3-Linux-x86_64.tar.gz
  tar -zxf cmake-3.13.3-Linux-x86_64.tar.gz
  cd cmake-3.13.3-Linux-x86_64
  sudo cp -r bin /usr/local
  sudo cp -r share /usr/local
  sudo cp -r doc /usr/local/share/
  sudo cp -r man /usr/local/share/
  EOH
end

bash 'install_boost' do
  user 'root'
  cwd '/tmp'
  code <<-EOH
  wget -nv https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz
  tar -zxf boost_1_69_0.tar.gz
  cd boost_1_69_0
  sudo ./bootstrap.sh --with-libraries=system,filesystem --libdir=/usr/local/lib --includedir=/usr/include
  sudo ./b2 install toolset=gcc-7
  EOH
end

execute 'create_build_directory' do
  cwd '/home/vagrant/'
  command 'mkdir build'
end

execute 'run_cmake' do
  cwd '/home/vagrant/build'
  command 'cmake ../adventure2019'
end

execute 'run_make' do
  cwd '/home/vagrant/build'
  command 'make'
end
