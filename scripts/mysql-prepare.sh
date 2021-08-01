#!/bin/bash
export DEBIAN_FRONTEND=noninteractive

sudo service mysql stop

sudo apt install mysql-server
sudo service mysql start
export MYSQL_PWD=root
mysqladmin ping -u root -h 127.0.0.1 --wait=90
mysqladmin -u root -h 127.0.0.1 password '123123qa'

export MYSQL_PWD=123123qa

echo "create database db;" | mysql -u root -h 127.0.0.1
gzcat scripts/db.sql.gz | mysql -u root -h 127.0.0.1 db
