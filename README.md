# Apriori

Syntax to compile the code on MacBook Pro:

g++ -O2 -std=c++11 -o apriori_tanvir apriori_tanvir.cpp

# Syntax to execute the code:
./apriori_tanvir dataT10K500D12L.data.txt 0.001 0.8 a

./apriori_tanvir dataT10K500D12L.data.txt 0.001 0.8 r

./apriori_tanvir dataT10K500D12L.data.txt 0.001 0.8 a

# Execution time

The code takes 15 seconds to produce the output on a 10000 transactions using g++ compiler with O2 optimizer on a MacBook Pro.

Number of association rules = 162069

Number of frequent 1_itemsets: 472

Number of frequent 2_itemsets: 24158

Number of frequent 3_itemsets: 8176

Number of frequent 4_itemsets: 3820

Number of frequent 5_itemsets: 2340

Number of frequent 6_itemsets: 1117

Number of frequent 7_itemsets: 407

Number of frequent 8_itemsets: 111

Number of frequent 9_itemsets: 21

Number of frequent 10_itemsets: 2
