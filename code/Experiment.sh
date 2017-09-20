#!/bin/bash
binary="./cachesim"

validate_test() {
    binary=$1
    trace_name=$2
    config=$3

    config_name=`echo $config | tr -d '-' | tr ' ' '_'`

    ${binary} ${config} < traces/${trace_name}.trc > ExperimentOutputs/${trace_name}_${config_name}.out
    echo "${trace_name}_${config_name}"
    
}

 function log2 {
    local x=0
    for (( y=$1-1 ; $y > 0; y >>= 1 )) ; do
        let x=$x+1
    done
    echo $x
}



if [ ! -f "${binary}" ]
then
    echo "Executable ${binary}" not found
    exit 1
fi

rm -rf ExperimentOutputs
mkdir ExperimentOutputs

for trace_name in "astar" "mcf" "perlbench"
do
	for((l=0;l<=12;l++))
	do
		for((m=0;m<=12;m++))
		do
			for((n=0;n<=12;n++))
			do  
				c=$((2 ** l))
				b=$((2 ** m))
				s=$((2 ** n))            
				
				if [ $b -le $c ]
				then
					NumLines=$(($c/$b))
					NumOffset=$(log2 b)                
								
					if [ $s -le $NumLines ]
					then 
						NumSets=$(($NumLines/$s))
						NumIndex=$(log2 NumSets) 
						#totalStore=$(($NumLines*(((66 - $NumLines - $NumOffset)/8) + $b)))
						metaDataPline=$(((66-$NumIndex-$NumOffset)/8))
						metaData=$(($metaDataPline*$NumLines))                  
						dataStorage=$(($NumLines*$b))
						totalStore=$(($dataStorage+$metaData))
						
						if [ $totalStore -le 4096 ]
						then
							validate_test "${binary}" "$trace_name" "-c$c -b$b -s$s"
							#$echo $totalStore $c $b $s
						fi
						
					
					fi  
						
				fi
		 
			done
		done
	done
done
