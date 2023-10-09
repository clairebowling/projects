#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/config.hpp>
#include <fstream>
#include <vector>
#include <string>


// CHANGE MAKING

// References: https://www.geeksforgeeks.org/coin-change-dp-7/
//https://www.geeksforgeeks.org/make-combinations-size-k/


std::vector<int> COINS = {1, 3, 5};

// FUNCTION: Change Making Algorithm
int makeChange(int change_val, std::vector<int> coins){
    std::vector<int> change(change_val + 1);
    std::vector<int> temp;
    std::vector<std::vector<int>> solutions;

    // Set the first way to 1 because its 0 and
    // there is 1 way to make 0 with 0 coins
    change[0] = 1;

    for(int i = 0; i < coins.size(); i++)
    {
        for(int j = 0; j < change.size(); j++)
        {
            if (coins[i] <= j)
            {

                // Update the change array
                change[j] += change[(j - coins[i])];
                int changeVAL = coins[i];
            }
        }
    }

    // Return the number of possible change combinations
    return change[change_val];
 }





// ARBITRAGE

// References: https://cppcodingzen.com/?p=2333
//https://cppsecrets.com/users/166319711511599105105995564103109971051084699111109/C00-Currency-Arbitrage.php
//https://stackoverflow.com/questions/26855184/floyd-warshall-all-pairs-shortest-paths-on-weighted-undirected-graph-boost-g




// type for weight/distance on each edge
typedef double t_weight;

// define the graph type
typedef boost::property<boost::edge_weight_t, t_weight> EdgeWeightProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, EdgeWeightProperty> Graph;

typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

// Declare a matrix type and its corresponding property map that
// will contain the distances between each pair of vertices.
typedef boost::exterior_vertex_property<Graph, t_weight> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;

std::vector<std::string> countries;
//std::vector<double> currency_rates;

std::vector<double> currency_rates = {
    1.0, 0.94, 1.48, 135.84,
    1.20, 1.0, 1.78, 163.49,
    0.68, 0.56, 1.0, 91.93,
    0.0074, 0.0061, 0.011, 1.0};

const int USA_INDEX = 0;
const int UK_INDEX = 1;
const int AUS_INDEX = 2;
const int JAP_INDEX = 3;

// FUNCTION: Read currency rates from file into vector
void readFile(std::string fileName){
    std::ifstream inFile;
    inFile.open("currency.txt");
    std::vector<double> row;
    double val;
    if(inFile.is_open()){
        while (!inFile.eof())
            {
                inFile >> val;
                currency_rates.push_back(val);
            }
        inFile.close();
    }
    else{
        std::cout << "Error reading currency rates from file" << std::endl;
    }
    return;
}


// FUNCTION: Helper function to find country name
std::string getCountryName(int val){
    std::string name;
    switch(val){
        case 0:
            name = "USA";
            break;
        case 1:
            name = "UK";
            break;
        case 2:
            name = "AUSTRALIA";
            break;
        case 3:
            name = "JAPAN";
            break;
    };
    return name;
}




int main(int argc, const char * argv[]) {
    
    // CHANGE MAKING PROBLEM
    std::vector<int> coin_options = {1, 3, 5};
    int solns = makeChange(9, coin_options);
    std::cout << "There are " << solns << " solutions.\n";


    
    // ARBITRAGE
    
    // read in values from text file containing a table of currency rates
    // Note: All exchange rate data was found using Google Financy on 3/4/2023
    std::string fileName = "currency.txt";
    readFile(fileName);
    
    Graph g;

    const int num_edges = 11;
    
    int edges[] = { 1,     2,
                    1,     3,
                    1,     4,
                    2,     5,
                    3,     4,
                    3,     6,
                    4,     5,
                    4,     6,
                    4,     7,
                    5,     7,
                    6,     7 };
    
    t_weight weight[currency_rates.size()];
    
    for(int i = 0; i < currency_rates.size(); i++){
        weight[i] = currency_rates[i];

    }
    
    
    for(int i = 0; i < currency_rates.size(); i++){
        std::cout << currency_rates[i] << std::endl;
    }

    // iterate over all edges and insert them in the graph
    for (std::size_t k = 0; k < num_edges; ++k)
        boost::add_edge(edges[k*2]-1, edges[k*2+1]-1, weight[k], g);

    WeightMap weight_pmap = boost::get(boost::edge_weight, g);

    // set the distance matrix to receive the floyd warshall output
    DistanceMatrix distances(num_vertices(g));
    DistanceMatrixMap dm(distances, g);

    // find all pairs shortest paths
    bool valid = floyd_warshall_all_pairs_shortest_paths(g, dm,
                    boost::weight_map(weight_pmap));

    // check if there no negative cycles
    if (!valid) {
        std::cerr << "Error - Negative cycle in matrix" << std::endl;
        return -1;
    }

    // print upper triangular part of the distance matrix
    std::cout << "Arbitrge Rates: " << std::endl;

    std::string node1;
    std::string node2;

    for (std::size_t i = 0; i < 4; ++i) {
        node1 = getCountryName(i);
        for (std::size_t j = i; j < 4; ++j) {
            node2 = getCountryName(j);
            std::cout << "From " << i + 1 << " " << " to " << j + 1 << " " << " : ";
            if(distances[i][j] == std::numeric_limits<t_weight>::max())
                std::cout << "inf" << std::endl;
            else
                std::cout << distances[i][j] << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}
