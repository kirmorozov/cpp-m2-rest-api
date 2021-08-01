#!/bin/bash
export DEBIAN_FRONTEND=noninteractive

echo "select version();" | mysql -u root -h 127.0.0.1

sudo service mysql stop

sudo apt install mysql-server
sudo service mysql start
mysqladmin ping -u root -proot -h 127.0.0.1 --wait=90

mysqladmin -u root -proot -h 127.0.0.1 password '123123qa'

echo "create database db;" | mysql -u root -p'123123qa' -h 127.0.0.1