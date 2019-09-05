sourcefile="main.cpp"
difffile="difffile.tmp"

if [ -f "$difffile" ]; then
    echo "" > $difffile
fi


for ((i=1;i<=3;i++)); do
    
    inputdir="test cases/$i/"
    inputfile="${inputdir}scene.txt"
    
    g++ $sourcefile -o a.out && ./a.out "$inputfile" && rm a.out

    for ((j=1;j<=3;j++)); do
        echo " -------------------------------------------- scene: $i stage: $j -------------------------------------------- " >> $difffile
        echo -e >> $difffile
        diff -y -w --suppress-common-lines -b -s "stage$j.txt" "${inputdir}stage$j.txt" >> $difffile
        # colordiff -y -w "stage$j.txt" "${inputdir}stage$j.txt"
        echo -e -e >> $difffile
    done    
done

for ((i=1;i<=3;i++)); do
    rm "stage$i.txt"
done
