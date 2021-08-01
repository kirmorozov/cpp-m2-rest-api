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
cat scripts/db.sql.gz | gunzip -c | mysql -u root -h 127.0.0.1 db
