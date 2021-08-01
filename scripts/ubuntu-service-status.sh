#!/bin/bash

export PID=$(cat process.pid)
if ps -p $PID > /dev/null
then
  echo "$PID is running"
else
  echo "$PID is dead"
  exit 1
fi