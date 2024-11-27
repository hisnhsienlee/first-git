#!/bin/bash

# Set the number of repetitions
repeat_count=10

# Set the command to execute
command="./Firehose_arm64 -f /data/SA525M_PVM_AB -l /data/log"

# Loop and execute the command
for ((i=1; i<=$repeat_count; i++))
do
    echo "Running iteration $i..."
    # Check if "QDL" is present in the output of lsusb
    if lsusb | grep -q "QDL"; then
        echo "QDL string found. Continuing..."
        $command
    else
        echo "QDL string not found. Waiting 5 seconds and retrying..."

        # Retry 3 times
        for ((j=1; j<=3; j++))
        do
            sleep 5
            if lsusb | grep -q "QDL"; then
                echo "QDL string found. Continuing..."
                $command
            fi

            if [ $j -eq 3 ]; then
                echo "QDL string not found after 3 retries. Exiting..."
                exit 1
            fi
        done
    fi
done
done