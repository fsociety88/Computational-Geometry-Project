#include <bits/stdc++.h>
using namespace std;

/** To find Run-time of code **/
class Timer {
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;
    std::chrono::time_point<clock_t> m_beg;
public:
    Timer() : m_beg(clock_t::now()) {}
    void reset() { m_beg = clock_t::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
}T;
/** END **/

struct Point{
    double x, y;
};


struct Polygon{
    vector<Point> points;

    Polygon(int size){
        points.resize(size);
    }
};


Polygon *polygon1, *polygon2;

/*
Compare function for sorting event points
Rule for sorting:
First sort by x co-ordinate, then by which polygon the point belongs to
and then by the y co-ordinate
*/
bool compareFunction(pair<int, int> point1, pair<int, int> point2){

    Polygon *tempPol1, *tempPol2;

    if(point1.second == 1)
        tempPol1 = polygon1;
    else
        tempPol1 = polygon2;
    if(point2.second == 1)
        tempPol2 = polygon1;
    else
        tempPol2 = polygon2;

    if(tempPol1->points[point1.first].x < tempPol2->points[point2.first].x)
        return 1;
    if(tempPol1->points[point1.first].x == tempPol2->points[point2.first].x){
        if(point1.second != point2.second)
            return point1.second < point2.second;
        return tempPol1->points[point1.first].y < tempPol2->points[point2.first].y;
    }
    return 0;
}

//Creating event points and sorting them
void createEventPoints(vector<pair<int, int> > &eventsList){

    for(int i = 0; i < polygon1->points.size(); i++){
        eventsList.push_back(make_pair(i, 1));
    }

    for(int i = 0; i < polygon2->points.size(); i++){
        eventsList.push_back(make_pair(i, 2));
    }

    sort(eventsList.begin(), eventsList.end(), compareFunction);
}

//Rule for current vertex
char determineRule(int vertexNum, int polygon){

    Point nextVertex, prevVertex, curVertex;

    if(polygon == 1){
        int sz = polygon1->points.size();
        nextVertex = polygon1->points[(vertexNum+1)%sz];
        prevVertex = polygon1->points[(vertexNum-1+sz)%sz];
        curVertex = polygon1->points[vertexNum];
    }
    else{
        int sz = polygon2->points.size();
        nextVertex = polygon2->points[(vertexNum+1)%sz];
        prevVertex = polygon2->points[(vertexNum-1+sz)%sz];
        curVertex = polygon2->points[vertexNum];
    }

    if(nextVertex.x > curVertex.x and prevVertex.x > curVertex.x)
        return 'a';
    if(nextVertex.x < curVertex.x and prevVertex.x < curVertex.x)
        return 'b';
    if(nextVertex.x < curVertex.x and prevVertex.x > curVertex.x)//rule c: next neighbor is behind
        return 'c';
    if(nextVertex.x > curVertex.x and prevVertex.x < curVertex.x)
        return 'd';
    return 'v';
}


// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
    if((p.x == q.x and p.y == q.y) or (q.x == r.x and q.y == r.y))
        return 0;
    return (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
            q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y));
}

//Find intersection point of line segments p1-p2 and q1-q2
void doesIntersectHelper(Point p1, Point p2, Point q1, Point q2, vector<Point> &intersectionPointsList){

    // Line p1-p2 represented as a1x + b1y = c1
    double a1 = p2.y - p1.y;
    double b1 = p1.x - p2.x;
    double c1 = a1*(p1.x) + b1*(p1.y);

    // Line q1-q2 represented as a2x + b2y = c2
    double a2 = q2.y - q1.y;
    double b2 = q1.x - q2.x;
    double c2 = a2*(q1.x)+ b2*(q1.y);

    double determinant = a1*b2 - a2*b1;

    if(determinant != 0){//Determinant is zero if both the lines are parallel
        Point intPoint;
        intPoint.x = (b2*c1 - b1*c2)/determinant;
        intPoint.y = (a1*c2 - a2*c1)/determinant;
        if(onSegment(p1, intPoint, p2) and onSegment(q1, intPoint, q2))
            intersectionPointsList.push_back(intPoint);
    }
}

//Find all the intersection points between edge and all the edges in lst
void doesIntersect(list<pair<int, int> > lst, pair<int, int> edge, int polygon, vector<Point> &intersectionPointsList){

    Point curPoints[2];
    if(polygon == 1){
        curPoints[0] = polygon1->points[edge.first];
        curPoints[1] = polygon1->points[edge.second];
    }
    else{
        curPoints[0] = polygon2->points[edge.first];
        curPoints[1] = polygon2->points[edge.second];
    }

    for(auto x:lst){
        Point tempPoints[2];
        if(polygon == 1){
            tempPoints[0] = polygon2->points[x.first];
            tempPoints[1] = polygon2->points[x.second];
        }
        else{
            tempPoints[0] = polygon1->points[x.first];
            tempPoints[1] = polygon1->points[x.second];
        }

        doesIntersectHelper(curPoints[0], curPoints[1], tempPoints[0], tempPoints[1], intersectionPointsList);
    }
}

//Process current event point
void findIntersectionsHelper(int vertexNum, int polygon, list<pair<int, int> > &list1,
                             list<pair<int, int> > &list2, vector<Point> &intersectionPointsList){

    char rule = determineRule(vertexNum, polygon);

    int nextVertexNum, prevVertexNum;

    if(polygon == 1){
        int sz = polygon1->points.size();
        nextVertexNum = (vertexNum+1)%sz;
        prevVertexNum = (vertexNum-1+sz)%sz;
    }
    else{
        int sz = polygon2->points.size();
        nextVertexNum = (vertexNum+1)%sz;
        prevVertexNum = (vertexNum-1+sz)%sz;
    }

    switch(rule){
        case 'a':{//both next and prev are ahead wrt x co-ord
            list1.push_back({vertexNum, nextVertexNum});
            list1.push_back({vertexNum, prevVertexNum});
            doesIntersect(list2, make_pair(vertexNum, nextVertexNum), polygon, intersectionPointsList);
            doesIntersect(list2, make_pair(vertexNum, prevVertexNum), polygon, intersectionPointsList);
            break;
        }
        case 'b':{//both next and prev are behind wrt x co-ord
            list1.remove({prevVertexNum, vertexNum});
            list1.remove({nextVertexNum, vertexNum});
            break;
        }
        case 'c':{//next is behind and prev is ahead wrt x co-ord
            list1.remove({nextVertexNum, vertexNum});
            list1.push_back({vertexNum, prevVertexNum});
            doesIntersect(list2, make_pair(vertexNum, prevVertexNum), polygon, intersectionPointsList);
            break;
        }
        case 'd':{//next is ahead and prev is behind wrt x co-ord
            list1.remove({prevVertexNum, vertexNum});
            list1.push_back({vertexNum, nextVertexNum});
            doesIntersect(list2, make_pair(vertexNum, nextVertexNum), polygon, intersectionPointsList);
            break;
        }
        case 'v':{//one of next or prev of same polygon has same x co-ord
            //First find intersections for current and next
            doesIntersect(list2, make_pair(vertexNum, nextVertexNum), polygon, intersectionPointsList);
            Polygon *curPolygon;
            if(polygon == 1)
                curPolygon = polygon1;
            else
                curPolygon = polygon2;
            //If prev is not in same vertical line as current
            if(curPolygon->points[prevVertexNum].x != curPolygon->points[vertexNum].x){
                if(curPolygon->points[prevVertexNum].x < curPolygon->points[vertexNum].x)
                    list1.remove({prevVertexNum, vertexNum});
                else{
                    list1.push_back({vertexNum, prevVertexNum});
                    doesIntersect(list2, make_pair(vertexNum, prevVertexNum), polygon, intersectionPointsList);
                }
            }
            //If next is not in same vertical line as current
            if(curPolygon->points[nextVertexNum].x != curPolygon->points[vertexNum].x){
                if(curPolygon->points[nextVertexNum].x < curPolygon->points[vertexNum].x)
                    list1.remove({nextVertexNum, vertexNum});
                else{
                    list1.push_back({vertexNum, nextVertexNum});
                    //Do not find intersections here because it has already been done above.
                }
            }
            break;
        }
    }
}

//Process all the event points one by one
void findIntersections(vector<pair<int, int> > &eventsList,
                       vector<Point> &intersectionPointsList){

    list<pair<int, int> > poly1List, poly2List;

    for(int i = 0; i < eventsList.size(); i++){
        if(eventsList[i].second == 1)
            findIntersectionsHelper(eventsList[i].first, eventsList[i].second, poly1List, poly2List, intersectionPointsList);
        else
            findIntersectionsHelper(eventsList[i].first, eventsList[i].second, poly2List, poly1List, intersectionPointsList);
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cout << fixed << setprecision(6);

    int poly1Size, poly2Size;
    cin >> poly1Size;

    polygon1 = new Polygon(poly1Size);

    for (int i = 0; i < poly1Size; i++) {
        cin >> polygon1->points[i].x >> polygon1->points[i].y;
    }

    cin >> poly2Size;

    T.reset();

    polygon2 = new Polygon(poly2Size);
    for (int i = 0; i < poly2Size; i++) {
        cin >> polygon2->points[i].x >> polygon2->points[i].y;
    }

    vector<pair<int, int> > eventsList;
    createEventPoints(eventsList);

    vector<Point> intersectionPointsList;
    findIntersections(eventsList, intersectionPointsList);

    cout << "Intersection Points:\n";
    for (int i = 0; i < intersectionPointsList.size(); i++) {
        cout << intersectionPointsList[i].x << " " << intersectionPointsList[i].y << endl;
    }

    cerr << "\nDone in " << 1000 * T.elapsed() << " ms." << "\n";

    return 0;
}
