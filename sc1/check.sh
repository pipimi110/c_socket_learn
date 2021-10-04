#!/bin/bash
for((i=1;i<=100;i++));do
    bash -c "nohup ./client1 &" ;
done;
