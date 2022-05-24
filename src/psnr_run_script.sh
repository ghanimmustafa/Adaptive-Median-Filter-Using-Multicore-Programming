set -x

echo "PSNR calculations .."
./calculate_psnr 0801.png low_noisy_0801.png;
./calculate_psnr 0801.png local_filtered_low_noisy_0801.png ;
./calculate_psnr 0801.png	seq_adaptive_filtered_low_noisy_0801.png ; 
./calculate_psnr 0802.png low_noisy_0802.png;
./calculate_psnr 0802.png local_filtered_low_noisy_0802.png ;
./calculate_psnr 0802.png	seq_adaptive_filtered_low_noisy_0802.png ; 
./calculate_psnr 0803.png low_noisy_0803.png;
./calculate_psnr 0803.png local_filtered_low_noisy_0803.png ;
./calculate_psnr 0803.png	seq_adaptive_filtered_low_noisy_0803.png ; 
./calculate_psnr 0804.png low_noisy_0804.png;
./calculate_psnr 0804.png local_filtered_low_noisy_0804.png ;
./calculate_psnr 0804.png	seq_adaptive_filtered_low_noisy_0804.png ; 
./calculate_psnr 0805.png low_noisy_0805.png;
./calculate_psnr 0805.png local_filtered_low_noisy_0805.png ;
./calculate_psnr 0805.png	seq_adaptive_filtered_low_noisy_0805.png ; 
./calculate_psnr 0801.png high_noisy_0801.png;
./calculate_psnr 0801.png local_filtered_high_noisy_0801.png ;
./calculate_psnr 0801.png	seq_adaptive_filtered_high_noisy_0801.png ; 
./calculate_psnr 0802.png high_noisy_0802.png;
./calculate_psnr 0802.png local_filtered_high_noisy_0802.png ;
./calculate_psnr 0802.png	seq_adaptive_filtered_high_noisy_0802.png ; 
./calculate_psnr 0803.png high_noisy_0803.png;
./calculate_psnr 0803.png local_filtered_high_noisy_0803.png ;
./calculate_psnr 0803.png	seq_adaptive_filtered_high_noisy_0803.png ; 
./calculate_psnr 0804.png high_noisy_0804.png;
./calculate_psnr 0804.png local_filtered_high_noisy_0804.png ;
./calculate_psnr 0804.png	seq_adaptive_filtered_high_noisy_0804.png ; 
./calculate_psnr 0805.png high_noisy_0805.png;
./calculate_psnr 0805.png local_filtered_high_noisy_0805.png ;
./calculate_psnr 0805.png	seq_adaptive_filtered_high_noisy_0805.png ; 

