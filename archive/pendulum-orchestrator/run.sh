#!/bin/bash
sudo docker-compose down --remove-orphans
sudo docker image prune -f
if [ "$1" == "build" ]; then
    sudo docker-compose build
fi

sudo docker-compose up