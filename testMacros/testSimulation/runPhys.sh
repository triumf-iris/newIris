for i in `seq 0 0` #specific runs

do

echo "Mon Jan  1	${i}	00:00:00	01:00:00	100.0k	1000.00k	Words	More Words" > ./runlog_lastrun.txt

/home/iris/curtis/NewIris/newIRIS/bin/physIris ./output -o=./physOut${i}.root -c=/home/iris/curtis/NewIris/newIRIS/Configurationfiles/phys_config_12C_2H_CATIMA.txt
done
