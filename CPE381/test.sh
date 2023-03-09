for x in {1..100}
do
    echo "Starting test $x"
    echo "ESTES_J_mod.wav output.wav" | ./test
    echo "ESTES_J_mod.wav output.wav" | ./main
    echo "Ending test $x"
done