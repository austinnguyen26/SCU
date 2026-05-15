for file in file1.bin file2.bin file3.bin file4.bin
do
    for buffer in 100 1000 10000 100000
        do
            for thread in 2 8 32 64
            do
            echo "Step5 $file $buffer $thread"
            time ./Step5 $file $buffer $thread
            rm copy*.out
            echo " "
        done
    done
done