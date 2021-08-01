#!/bin/bash
export DEBIAN_FRONTEND=noninteractive

echo "select version();" | mysql -u root -h 127.0.0.1

sudo service mysql stop

sudo apt install mysql-server
sudo service mysql start
sleep 14
sudo service mysql status


echo "create database db;" | mysql -u root -h 127.0.0.1