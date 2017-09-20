#!/bin/sh
binary="./cachesim"

validate_test() {
    binary=$1
    trace_name=$2
    config=$3

    config_name=`echo $config | tr -d '-' | tr ' ' '_'`

    ${binary} ${config} < test-traces/${trace_name}.trc > myoutput/${trace_name}_${config_name}.out
    diff output/${trace_name}_${config_name}.out myoutput/${trace_name}_${config_name}.out
}

if [ ! -f "${binary}" ]
then
    echo "Executable ${binary}" not found
    exit 1
fi

rm -rf myoutput
mkdir myoutput

validate_test "${binary}" "loop-8" " -c32 -b8 -s4"
validate_test "${binary}" "loop-8" " -c64 -b8 -s4"
validate_test "${binary}" "loop-high" "-c1024 -b32 -s1"
validate_test "${binary}" "stream" "-c16384 -b64 -s1"
validate_test "${binary}" "stream" "-c16384 -b64 -s256"
validate_test "${binary}" "bzip2" ""
