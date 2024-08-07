#!/bin/bash

usage() {
    echo "Usage: $0 [--build] [--clean] [--test] [--run]"
    echo "Commands:"
    echo "  --build     Build everything."
    echo "  --clean     Clean before building (must be used with --build)."
    echo "  --debug     Run the appliocation with gdb (must be used with --run and cannot be used with more than one target day)."
    echo "  --test      Run all tests."
    echo "  --run       Run all executables."
    echo "  --days      Specify days to build and/or run (must be used with --build or --run)."
    exit 1
}

build=false
clean=false
test=false
run=false

if [[ $# -eq 0 ]]; then
    usage
    exit 1
fi

while [[ $# -gt 0 ]]; do
    case "$1" in
        --build)
            build=true
            shift
            ;;
        --clean)
            clean=true
            shift
            ;;
        --test)
            test=true
            shift
            ;;
        --run)
            run=true
            shift
            ;;
        --days)
            if ! $build && ! $run; then
                echo "Error: --days can only be used with --build or --run"
                usage
            fi
            shift

            while [[ $# -gt 0 && ! $1 == "--"* ]]; do
                if [[ $1 =~ ^[0-9]+$ ]]; then
                    days+=($1)
                else
                    echo "Error: --days requires integer values"
                    usage
                fi
                shift
            done
            ;;
        --debug)
            if ! $run; then
                echo "Error: --debug can only be used with --run"
                usage
            fi
            debug=true
            shift
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done

if $clean && ! $build; then
    echo "Error: --clean can only be used with --build"
    usage
    exit 1
fi

if $build; then
    echo "---- RUNNING BUILD ----"

    cmake_cmd="cmake --build ."
    if [[ ${#days[@]} -gt 0 ]]; then
        targets=()
        for day in ${days[@]}; do
            targets+=("day${day}")
        done
        cmake_cmd="${cmake_cmd} --target ${targets[*]}"
    fi
    
    if $clean; then
        cmake_cmd="${cmake_cmd} --clean-first"
    fi

    cmake_output=$(eval "$cmake_cmd")

    if [[ $? -ne 0 ]]; then
        echo "$cmake_output"
        echo "---- BUILD FAILED ----"
        exit 1
    fi

    if [[ -z $cmake_output ]]; then
        echo "CMake did not produce any output. Did you forget to add your target(s) to CMakeLists.txt?"
        echo "---- BUILD FAILED ----"
        exit 1
    fi
    
    echo "---- BUILD SUCCEEDED ----"
fi

if $test; then
    echo "---- RUNNING TESTS ----"
  
    ctest
    
    if [ $? -ne 0 ]; then
        echo "---- TESTS FAILED ----"
        exit 1
    fi
    
    echo "---- TESTS SUCCEEDED ----"
fi

if $run; then
    if [[ ${#days[@]} -eq 0 ]]; then
        shopt -s extglob            # Enable more advanced pattern matching.
        dirs=$(ls -d1v build/day*[0-9])   # Get all days.
        shopt -u extglob            # Restore default behaviour.
        
        for dir in $dirs; do
            day="${dir//[^0-9]/}"   # Retrieve the numerical component of the directory name.
            days+=($day)
        done
    fi

    echo "---- RUNNING DAYS: ${days[@]} ----"

    for day in ${days[@]}; do
        day_command="build/day${day} data/day${day}.txt"
        #if $debug; then
        #    if [[ ${#days[@]} -eq 1 ]]; then
                #day_command="gdb -ex run --args $day_command"
        #    else
                #echo "--debug cannot be used for more than one target day at a time"
                #usage
        #    fi
        #fi
        echo "Executing ${day_command}..."
        $day_command
        exit_code=$?

        case $exit_code in
            0) # Success
                ;;
            1)
                echo "Day ${day}: Part 1 was incorrect"
                exit 1
                ;;
            2)
                echo "Day ${day}: Part 2 was incorrect"
                exit 2
                ;;
            *)
                echo "Day ${day}: An unexpected exit code was encountered (${exit_code})"
                exit $exit_code
                ;;
        esac
    done
fi
