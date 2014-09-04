
# Check if we have the current directory in the library paths
# and add it if not.

# (I'd be glad if somebody have a solution for the system to look automatically
# for the library given the full path given at linking stage instead of having to do this)

TEST=`echo $LD_LIBRARY_PATH | grep $PWD`
if [[ "$TEST" == "" ]]
then
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD
fi
