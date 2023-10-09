// References: https://www.geeksforgeeks.org/convex-hull-using-divide-and-conquer-algorithm/
// https://www.tutorleftHoldlspoint.com/convex-hull-using-divide-and-conquer-algorithm-in-cplusplus


#include <iostream>
#include <vector>
#include <chrono>
#include <utility>
#include <fstream>
#include <random>

using Point = std::pair<double,double>;


// FUNCTION: Generate a random double value that will be used as a point
double randDouble(){
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<double> uD(0.0, 1000.0);
    double num = uD(gen);
    return num;
}

// FUNCTION: Write a random number of point pairs to a file
void writeRandPoints(std::string fileName){
    std::ofstream outFile;
    outFile.open(fileName);

    if (outFile.is_open()){
        std::cout << "Currently writing point pairs to file" << std::endl;
        // Generate a random number of points
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::uniform_int_distribution<int> uD(10, 100);
        int numPoints = uD(gen);

        for (int i = 0; i < numPoints; i++){
            double point1 = randDouble();
            double point2 = randDouble();
            // write point pair to file
            outFile << point1 << point2;
            std::cout << point1 << " " << point2 << std::endl;
        }
        outFile.close();
    }
    else{
        std::cout << "Error writing points to file" << std::endl;
    }
}

// FUNCTION: Read points from file into a vector of point pairs
std::vector<Point> readPointsFromFile(std::string fileName){
    std::vector<Point> points;
    std::ifstream inFile;
    inFile.open(fileName);

    double point1;
    double point2;

    if(inFile.is_open()){
        while (!inFile.eof()){
            inFile >> point1 >> point2;
            std::cout << point1 << " " << point2 << std::endl;
            points.push_back(std::make_pair(point1, point2));
        }
        inFile.close();
    }
    else{
        std::cout << "Error reading points from file" << std::endl;
    }
    return points;
}


// FUNCTION: Find convex hull by brute force
std::vector<Point> convexHullByBruteForce(std::vector<Point> pointsVector){
    std::vector<Point> convexHull;
    
        for (int i = 0; i< pointsVector.size(); i++)
        {
            for (int j = i + 1; j < pointsVector.size(); j++)
            {
                double x1 = pointsVector[i].first, x2 = pointsVector[j].first;
                double y1 = pointsVector[i].second, y2 = pointsVector[j].second;
//
                double a = y1 - y2;
                double b = x2 - x1;
                // line segment
                double c = x1 * y2 - y1 * x2;
                int pos = 0, neg = 0;
                for (int k = 0; k < pointsVector.size(); k++)
                {
                    // ax + by = c... check sign to see if on the same side of the line
                    if (a * pointsVector[k].first + b * pointsVector[k].second + c <= 0)
                        neg++;
                    if (a * pointsVector[k].first + b * pointsVector[k].second + c >= 0)
                        pos++;
                }
                // if all on the same side of the line
                if (pos == pointsVector.size() || neg == pointsVector.size())
                {
                    // Check to see if point is already in convex hull
                    if (std::count(convexHull.begin(), convexHull.end(), pointsVector[i]) || std::count(convexHull.begin(), convexHull.end(), pointsVector[j])) {
                        continue;
                    }
                    // if point is not in convex hull push into convex hull vector
                    else{
                        convexHull.push_back(std::make_pair(pointsVector[i].first, pointsVector[i].second));
                        convexHull.push_back(std::make_pair(pointsVector[j].first, pointsVector[j].second));
                    }
                }
            }
        }
    std::sort(convexHull.begin(), convexHull.end());
    return convexHull;
}

// FUNCTION: Determine if point intersects convex hull
int calc_line(Point a, Point b, Point c){
   int res = (b.second-a.second)*(c.first-b.first) - (c.second-b.second)*(b.first-a.first);
   if (res == 0)
       return 0;
   if (res > 0)
       return 1;
   return -1;
}

// FUNCTION: Helper function to find convex hull from divided points
std::vector<Point> merger(std::vector<Point> left,
                              std::vector<Point> right){
    int size_left = left.size();
    int size_right = right.size();

    int leftHold = 0, rightHold = 0;

    for (int i = 1; i < size_left; i++)
        if (left[i].first > left[leftHold].first)
            leftHold = i;

    for (int i = 1; i < size_right; i++)
        if (right[i].first < right[rightHold].first)
            rightHold = i;

    int lVal = leftHold, rVal = rightHold;
    bool done = 0;
    while (!done) // finding the upper tangent

    {
        done = 1;
        while (calc_line(right[rVal], left[lVal], left[(lVal + 1) % size_left]) >=0)
            lVal = (lVal + 1) % size_left;

        while (calc_line(left[lVal], right[rVal], right[(size_right + rVal - 1) % size_right]) <=0)
        {
            rVal = (size_right + rVal - 1) % size_right;
            done = 0;
        }
    }

    int upperL = lVal, upperR = rVal;
    lVal = leftHold;
    rVal = rVal;
    done = 0;

    while (!done) //finding the lower tangent
    {
        done = 1;
        while (calc_line(left[lVal], right[rVal], right[(rVal + 1) % size_right]) >= 0)
            rVal = (rVal + 1) % size_right;

        while (calc_line(right[rVal], left[lVal], left[(size_left + lVal - 1) % size_left])<=0)
        {
            lVal = (size_left + lVal - 1) % size_left;
            done = 0;
        }
    }

    int lowerL = lVal, lowerR = rVal;

    // Convex Hull that will be returned by function
    std::vector<Point> convexHull;

    int temp = upperL;
    convexHull.push_back(left[upperL]);
    while (temp != lowerL)
    {
        temp = (temp + 1) % size_left;
        convexHull.push_back(left[temp]);
    }

    temp = lowerR;
    convexHull.push_back(right[lowerR]);
    while (temp != upperR)
    {
        temp = (temp + 1) % size_right;
        convexHull.push_back(right[temp]);
    }
    return convexHull;

}




// FUNCTION: Find convex hull by the divide and conquer method
std::vector<Point> convexHullByDandC(std::vector<Point> pointsVector){

    // 2 vectors to partiton points
    std::vector<Point>left, right;

    // First half points
    for (int i = 0; i < pointsVector.size() / 2; i++)
        left.push_back(pointsVector[i]);

    // Second half points
    for (int i = pointsVector.size() / 2; i < pointsVector.size(); i++)
        right.push_back(pointsVector[i]);

    // call divide and conquer method recursively for left and right hulls
    std::vector<Point>left_hull = convexHullByDandC(left);
    std::vector<Point>right_hull = convexHullByDandC(right);

    // merging the convex hulls
    return merger(left_hull, right_hull);
}




int main(int argc, const char * argv[]) {
    // Generate random points file and read as input points into vector
    writeRandPoints("points.txt");
    std::vector<Point> myPoints = readPointsFromFile("points.txt");
    
    // Brute Force
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    std::vector<Point> bf_Convex_Hull = convexHullByBruteForce(myPoints);
    std::chrono::steady_clock::time_point stop_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli>  elapsed_time = stop_time - start_time;
    std::cout << std::endl << "It took me " << elapsed_time.count() << " milliseconds to find convex hull by brute force." << std::endl;
    
    std::cout << "Printing convex hull by brute force" << std::endl;
    for (int i = 0; i < bf_Convex_Hull.size(); i++){
        std::cout << bf_Convex_Hull[i].first << "  " << bf_Convex_Hull[i].second << std::endl;
    }
    
    
    // Divide and Conquer
    start_time = std::chrono::steady_clock::now();
    std::vector<Point> DC_Convex_Hull = convexHullByDandC(myPoints);
    stop_time = std::chrono::steady_clock::now();
    elapsed_time = stop_time - start_time;
    std::cout << std::endl << "It took me " << elapsed_time.count() << " milliseconds to find convex hull by divide and conquer." << std::endl;
    
    std::cout << "Printing convex hull by divide and conquer" << std::endl;
    for (int i = 0; i < bf_Convex_Hull.size(); i++){
        std::cout << bf_Convex_Hull[i].first << "  " << bf_Convex_Hull[i].second << std::endl;
    }
    
    return 0;
}
