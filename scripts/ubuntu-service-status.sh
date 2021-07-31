#!/bin/bash

export PID=$(cat process.pid)
then
  echo "$PID is running"
else
  echo "$PID is dead"
  exit 1
fi