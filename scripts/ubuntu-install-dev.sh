#!/bin/bash
sudo add-apt-repository ppa:pistache+team/unstable &&
wget https://dev.mysql.com/get/mysql-apt-config_0.8.18-1_all.deb &&
sudo dpkg -i mysql-apt-config_0.8.18-1_all.deb &&
sudo apt update &&
sudo apt install libpistache-dev libmysqlcppconn-dev libsodium-dev &&
wget -O ../3rdparty/includes/json.hpp https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp