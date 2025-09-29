# weak_experiments_for_MSO

執行方式依序為:
make
./main (可以看到參數設定: ./main ell problem detect_eg detect_weak show_fitness)

補充說明:
detect_eg
detect_weak
show_fitness
這三個變數設定1為開啟、設定0為關閉

執行完的結果:
在 order = 1 的情況下
-> 代表 solid
->>> 代表 dashed

## 20250927

./main problem ell detect_eg detect_epi detect_weak show_fitness
problem: 
     ONEMAX         :  0
     LEADINGONES    :  1
     CTRAP          :  2
     CYCTRAP        :  3
     CNIAH          :  4
     LEADINGONES    :  5
     LEADINGTRAPS   :  6
     MAX3SAT_TEST   :  7
     MAX3SAT_RANDOM :  8
     1-0 CYCTRAP    :  9

time ./collect_unique_max3sat_parallel.py --bin ./main2 --args 7 18 1 0 0 1 --jobs 8 --target 100 --outdir max3sat_ell_18

python3 sat.py example.cnf 