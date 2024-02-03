#!/bin/bash
     	# run tests
     	correct=0
     	total=0
     	i=1
     	flag=()
     	while [[ -f ./tests/${i}.in ]]; do
     		total=$((total+1))
    		./a.out `cat ./tests/${i}.args` < ./tests/${i}.in > /autograder/results/${i}.out 2> /autograder/results/${i}.err
    		./a.out `cat ./tests/${i}.args` < ./tests/${i}.in &> /autograder/results/${i}.all
    		if [[ $(diff /autograder/results/${i}.out ./tests/${i}.out) == "" && $(diff /autograder/results/${i}.err ./tests/${i}.err) == "" && $(diff /autograder/results/${i}.all ./tests/${i}.all) == "" ]]; then
    			correct=$((correct+1))
    			flag+=("passed")
    	
    			echo
    			echo "===================="
    			echo "CASE #${i} PASSED"
    			echo "===================="
    		else
    			flag+=("failed")
    	
     			echo "===================="
     			echo "CASE #${i} FAILED"
    	     		echo "--------------------"
     			echo "Description:"
     			cat ./tests/${i}.desc
    	     		if (( $i < 5 )); then
     				echo "--------------------"
     				echo "Input:"
     				cat ./tests/${i}.in
     				echo "--------------------"
     				echo "Output:"
     				cat -n /autograder/results/${i}.all
     				echo
     				echo "--------------------"
     				echo "Expected output:"
     				cat -n ./tests/${i}.all
     				echo
     			fi
    			echo "===================="
     		fi
    	
    	     	i=$((i+1))
     	done
    	
     	# compute score
     	if (( ${total} > 0 )); then
     		score=$(echo 100.0*${correct}/${total} |bc -l |cut -c 1-5)
     	else
     		score=0.0
     	fi
     
          # dump result
     	echo "{" > /autograder/results/results.json
     	echo "  \"score\": ${score}," >> /autograder/results/results.json
     	echo "  \"visibility\": \"visible\"," >> /autograder/results/results.json
     	echo "  \"stdout_visibility\": \"visible\"," >> /autograder/results/results.json
     	echo "  \"tests\":" >> /autograder/results/results.json
     	echo "  [" >> /autograder/results/results.json
     
     	# test cases result
     	i=1
     	j=0
     	while [[ -f ./tests/${i}.in ]]; do
     		echo "    {" >> /autograder/results/results.json
     		echo "      \"name\": \"Case #${i}\"," >> /autograder/results/results.json
     		echo "      \"status\": \"${flag[$j]}\"" >> /autograder/results/results.json
     		echo "    }," >> /autograder/results/results.json
     		i=$((i+1))
     		j=$((j+1))
     	done
     
     	# compilation result
     	echo "    {" >> /autograder/results/results.json
     	echo "      \"name\": \"Compilation check\"," >> /autograder/results/results.json
     	echo "      \"status\": \"passed\"" >> /autograder/results/results.json
     	echo "    }" >> /autograder/results/results.json
     
     	echo "  ]" >> /autograder/results/results.json
     	echo "}" >> /autograder/results/results.json
    	