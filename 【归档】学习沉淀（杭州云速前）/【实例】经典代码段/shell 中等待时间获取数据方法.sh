Y_Name='��Ĵ�������ʶ'
Y_key='���key'
WG='http://www.lewei50.com/api/V1/Gateway/UpdateSensors/01'
echo "aaa" > /dev/ttyS1
cat /dev/ttyS1 > /root/a & { sleep 6 ; eval 'kill -9 $!' &> /dev/null ; }
read Va < /root/a
curl --request POST $WG --data "[{\"Name\":\"$Y_Name\",\"Value\":\"$Va\"}]" --header "userkeyY_key"



�ؼ��Σ�
1��
�ȴ�6�룬�޷���ȡ���ݣ�ɱ��cat /dev/ttyS1 > /root/a
cat /dev/ttyS1 > /root/a & { sleep 6 ; eval 'kill -9 $!' &> /dev/null ; }