Y_Name='你的传感器标识'
Y_key='你的key'
WG='http://www.lewei50.com/api/V1/Gateway/UpdateSensors/01'
echo "aaa" > /dev/ttyS1
cat /dev/ttyS1 > /root/a & { sleep 6 ; eval 'kill -9 $!' &> /dev/null ; }
read Va < /root/a
curl --request POST $WG --data "[{\"Name\":\"$Y_Name\",\"Value\":\"$Va\"}]" --header "userkeyY_key"



关键段：
1、
等待6秒，无法获取数据，杀掉cat /dev/ttyS1 > /root/a
cat /dev/ttyS1 > /root/a & { sleep 6 ; eval 'kill -9 $!' &> /dev/null ; }