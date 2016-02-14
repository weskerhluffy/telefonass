casos_uso=40
nums_por_caso=10000
inicio_seq=1000000000
echo $casos_uso > /tmp/mierda_stress.txt
for i in $(seq $casos_uso)
do
	echo $nums_por_caso >> /tmp/mierda_stress.txt
	for ((a=1; a <= $nums_por_caso ; a++))
	do
		valor_act=$(( inicio_seq + a ))
		echo $valor_act >> /tmp/mierda_stress.txt
	done
done
