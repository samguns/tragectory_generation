//
// Created by Sam on 8/1/2018.
//
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include "polynomial_solver.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;


struct test_case {
  vector<double> start;
  vector<double> end;
  double T;
};

vector< vector<double> > answers = {{0.0, 10.0, 0.0, 0.0, 0.0, 0.0},{0.0,10.0,0.0,0.0,-0.625,0.3125},{5.0,10.0,1.0,-3.0,0.64,-0.0432}};


vector<double> JMT(vector<double> start, vector<double> end, double T) {
  /*
    Calculate the Jerk Minimizing Trajectory that connects the initial state
    to the final state in time T.

    INPUTS

    start - the vehicles start location given as a length three array
            corresponding to initial values of [s, s_dot, s_double_dot]

    end   - the desired end state for vehicle. Like "start" this is a
            length three array.

    T     - The duration, in seconds, over which this maneuver should occur.

    OUTPUT
    an array of length 6, each value corresponding to a coefficent in the polynomial
    s(t) = a_0 + a_1 * t + a_2 * t**2 + a_3 * t**3 + a_4 * t**4 + a_5 * t**5

    EXAMPLE

    > JMT( [0, 10, 0], [10, 10, 0], 1)
    [0.0, 10.0, 0.0, 0.0, 0.0, 0.0]
  */
  const double si = start[0];
  const double s_dot_i = start[1];
  const double s_double_dot_i = start[2];
  const double sf = end[0];
  const double s_dot_f = end[1];
  const double s_double_dot_f = end[2];
  double a0 = si;
  double a1 = s_dot_i;
  double a2 = s_double_dot_i / 2.0;

  const double T2 = pow(T, 2);
  const double T3 = T2 * T;
  const double T4 = T3 * T;
  const double T5 = T4 * T;
  MatrixXd A(3, 3);
  A << T3, T4, T5,
      3 * T2, 4 * T3, 5 * T4,
      6 * T, 12 * T2, 20 * T3;
  VectorXd B(3);
  B << sf - (si + s_dot_i * T + s_double_dot_i * T2 * 0.5),
       s_dot_f - (s_dot_i + s_double_dot_i * T),
       s_double_dot_f - s_double_dot_i;

  VectorXd x = A.inverse() * B;

  return {a0, a1, a2, x.data()[0], x.data()[1], x.data()[2]};
}

bool close_enough(vector< double > poly, vector<double> target_poly, double eps=0.01)
{
  if(poly.size() != target_poly.size())
  {
    cout << "your solution didn't have the correct number of terms" << endl;
    return false;
  }
  for(int i = 0; i < poly.size(); i++)
  {
    cout << poly[i] << " answer:" << target_poly[i] << endl;
    double diff = poly[i]-target_poly[i];
    if(abs(diff) > eps)
    {
      cout << "at least one of your terms differed from target by more than " << eps << endl;
      return false;
    }

  }
  return true;
}

void verify_JMT() {
  // create test cases
  vector< test_case > tc;

  test_case tc1;
  tc1.start = {0,10,0};
  tc1.end = {10,10,0};
  tc1.T = 1;
  tc.push_back(tc1);

  test_case tc2;
  tc2.start = {0,10,0};
  tc2.end = {20,15,20};
  tc2.T = 2;
  tc.push_back(tc2);

  test_case tc3;
  tc3.start = {5,10,2};
  tc3.end = {-30,-20,-4};
  tc3.T = 5;
  tc.push_back(tc3);

  bool total_correct = true;
  for(int i = 0; i < tc.size(); i++)
  {
    vector< double > jmt = JMT(tc[i].start, tc[i].end, tc[i].T);
    bool correct = close_enough(jmt,answers[i]);
    total_correct &= correct;

  }
  if(!total_correct)
  {
    cout << "Try again!" << endl;
  }
  else
  {
    cout << "Nice work!" << endl;
  }
}
