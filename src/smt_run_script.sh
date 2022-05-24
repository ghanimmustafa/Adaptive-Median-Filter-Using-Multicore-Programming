set -x
max_threads=32
for (( thread_count=1; thread_count <= $max_threads; thread_count = thread_count*2 ))
do
	echo "thread count = $thread_count"
	./smt_parallel low_noisy_0801.png $thread_count ; 
	./smt_parallel  low_noisy_0802.png $thread_count ; 
	./smt_parallel  low_noisy_0803.png $thread_count  ; 
	./smt_parallel  low_noisy_0804.png $thread_count  ;  
	./smt_parallel  low_noisy_0805.png $thread_count  ;  
	./smt_parallel  high_noisy_0801.png $thread_count  ; 
	./smt_parallel  high_noisy_0802.png $thread_count  ;  
	./smt_parallel  high_noisy_0803.png $thread_count  ;  
	./smt_parallel  high_noisy_0804.png $thread_count  ; 
	./smt_parallel  high_noisy_0805.png $thread_count  ;
	
done  
