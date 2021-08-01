#!/bin/bash
export DEBIAN_FRONTEND=noninteractive
sudo service mysql stop

apt install mysql-server
sudo service mysql start
sleep 14
sudo service mysql status


echo "create database db;" | mysql -u root db