# Name: Austin Nguyen
# Date: September 24, 2024
# Title: Lab1 – Unit/Linux Commands and Basic Shell Programming
# Description: This program executes the sample code to verify the user and calculate the area of a rectanble according to inputted width and heights
# 


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
    echo "Enter height of rectangle: "
    read height
    echo "Enter width of rectangle: "
    read width
    area=`expr $height \* $width`
    echo "The area of the rectangle is $area"
    echo "Would you like to repeat for another rectangle [Yes/No]?"
    read response
done