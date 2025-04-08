# Name: Austin Nguyen
# Date: September 24, 2024
# Title: Lab1 – Unit/Linux Commands and Basic Shell Programming
# Description: Same file as the Sample, however is edited to instead find the area of a circle with an inputted radius
#Sample shell programs for Lab assignment
#!/bin/sh
echo Executing $0
echo $(/bin/ls | wc -l) files
wc -l $(/bin/ls)
echo "HOME="$HOME
echo "USER="$USER
echo "PATH="$PATH
echo "PWD="$PWD
echo "\$\$"=$$
user=`whoami`
numusers=`who | wc -l`
echo "Hi $user! There are $numusers users logged on."
if [ $user = "anguyen10" ]
then
    echo "Now you can proceed!"
else
    echo "Check who logged in!"
    exit 1
fi

response="Yes"
while [ $response != "No" ]
do
    echo "Enter Radius of Circle: "
    read radius
    pi=3.14
    area=$(echo "$radius * $radius * $pi" | bc ) 
    echo "The area of the Circle is $area"
    echo "Would you like to repeat for another Circle [Yes/No]?"
    read response
done