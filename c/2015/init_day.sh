#!/bin/bash

DAY_NUMBER=$1
TEMPLATE_DIR="day_template"

echo -e "Attempting to initialise day $DAY_NUMBER of Advent of Code..."

echo "Validating day number..."
if [ -z $DAY_NUMBER ]; then
    echo "Day number is mandatory"
    exit 1
fi

if ! [[ $DAY_NUMBER =~ ^[0-9]+$ ]]; then
    echo "Day number must be numeric"
    exit 1
fi

DIR="day${DAY_NUMBER}"

echo "Checking for existing directory..."
if [ -d "$DIR" ]; then
    echo "A directory for day number $DAY_NUMBER already exists"
    exit 1
fi

echo "Checking for template directory..."
if ! [ -d "$TEMPLATE_DIR" ]; then
    echo "The template directory $TEMPLATE_DIR was not found"
    exit 1
fi

echo "Creating directory $DIR based on template directory $TEMPLATE_DIR..."
cp -R day_template $DIR
if [ $? -ne 0 ]; then
    echo "Copy failed with exit code $?\nUnable to initialise day $DAY_NUMBER"
    exit 1
fi

echo -e "Setting day number $DAY_NUMBER in $DIR/CMakeLists.txt...\n"
sed -i "s/X/$DAY_NUMBER/g" $DIR/CMakeLists.txt
if [ $? -ne 0 ]; then
    echo "sed failed with exit code $?\nEdit file manually"
    exit 1
fi

echo "Printing $DIR/CMakeLists.txt..."
echo "-------------------------------"
cat $DIR/CMakeLists.txt
echo -e "----------------------------\n"
echo "Day $DAY_NUMBER was successfully initialised."
