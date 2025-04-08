make:
    for file in file1.bin file2.bin file3.bin file4.bin
    do
    echo "Step2 $file"
    time ./Step2 $file
    echo " "
    done