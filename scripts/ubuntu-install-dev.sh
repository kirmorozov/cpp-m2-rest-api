#!/bin/bash
export DEBIAN_FRONTEND=noninteractive
sudo add-apt-repository ppa:pistache+team/unstable
wget https://dev.mysql.com/get/mysql-apt-config_0.8.18-1_all.deb
sudo dpkg -i mysql-apt-config_0.8.18-1_all.deb
sudo apt update
sudo apt install libpistache-dev libmysqlcppconn-dev libsodium-dev libhiredis-dev redis-server
sudo service redis-server start