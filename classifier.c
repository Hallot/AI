// Training set : 10 random files but % kept, 5 sound and 5 silent
// Total set : 100 -> testing set : 90 files = 9 * 10
// Caracteristics : E > 24, M > 15, Z < 0.1
// Let's cll them E and ¬E, M and ¬M, Z and ¬Z
// I don't really know how to choose those values so I chose ones that seemed to be good cut of points.
// I don't know either if 10 is enough, or what value I should choose

// Carac           E   |    ¬E    ||   M   |     ¬M    ||    Z   |    ¬Z    | Total
//              ___________________________________________________________________
// Sound       |   5   |     0    ||   5   |     0     ||    1   |     4    |   5  |
// Silence     |   0   |     5    ||   0   |     5     ||    0   |     5    |   5  |
//             ____________________________________________________________________|
// Total       |   5   |     5    ||   5   |     5     ||    1   |     9    |  10  |
//             |___________________________________________________________________|

// Prior probabilities and base rates
// P(Sound) = 0.5
// P(Silence) = 0.5

// Probability of "evidence"
// P(E) = 0.5
// P(M) = 0.5
// P(Z) = 0.1

// Probability of "likelihood"
// P(E/Sound) = 1
// P(M/Sound) = 1
// P(Z/Sound) = 0.2
// P(E/Silence) = 0
// P(M/Silence) = 0
// P(Z/Silence) = 0

#define CUT_E 24.0
#define CUT_M 15.0
#define CUT_Z 0.1

// Variables
int n_total = 10;

// 2-dim array containing number information
// schema (same for Si)
//   _______________________________________________
//  |   n_So   ||   n_E   |    n_M    |     n_Z     |
//  |   n_¬So  ||   n_¬E  |    n_¬M   |     n_¬Z    |
//  |_______________________________________________|

int so[4][2];
int si[4][2];

so[0][0] = 5;
so[0][1] = 5;
so[1][0] = 5;
so[1][1] = 0;
so[2][0] = 5;
so[2][1] = 0;
so[3][0] = 1;
so[3][1] = 4;

si[0][0] = 5;
si[0][1] = 5;
si[1][0] = 0;
si[1][1] = 5;
si[2][0] = 0;
si[2][1] = 5;
si[3][0] = 0;
si[3][1] = 5;


// 2-dim array containing probability information
// schema (same for Si)
//   ________________________________________________
//  |   P(So)   || P(E/So)  | P(M/So)  |   P(Z/So)   |
//  |   P(¬So)  || P(¬E/So) | P(¬M/So) |   P(¬Z/So)  |
//  |________________________________________________|

double P_So[4][2];
double P_Si[4][2];


// Update base values and related conditional probabilities
void update_values()
{
  P_So[0][0] = so[0][0] / n_total;
  P_So[0][1] = so[0][1] / n_total;
  P_So[1][0] = so[1][0] / so[0][0];
  P_So[1][1] = so[1][1] / so[0][0];
  P_So[2][0] = so[2][0] / so[0][0];
  P_So[2][1] = so[2][1] / so[0][0];
  P_So[3][0] = so[3][0] / so[0][0];
  P_So[3][1] = so[3][1] / so[0][0];

  P_Si[0][0] = si[0][0] / n_total;
  P_Si[0][1] = si[0][1] / n_total;
  P_Si[1][0] = si[1][0] / si[0][0];
  P_Si[1][1] = si[1][1] / si[0][0];
  P_Si[2][0] = si[2][0] / si[0][0];
  P_Si[2][1] = si[2][1] / si[0][0];
  P_Si[3][0] = si[3][0] / si[0][0];
  P_Si[3][1] = si[3][1] / si[0][0];
}

// In this function, we don't calculate P(evidence) = P(E).P(M).P(Z) on purpose
// It has no value since both p1 and p2 will be divided by it
// Return 1 if sound and 0 if silence
// Update the base values at the same time
int return_category(double e, double m, double z)
{
  double p1;
  double p2;
  
  // 0 if above (below for z) the chosen cut off rate and 1 otherwise
  int te = e < CUT_E;
  int tm = m < CUT_M;
  int tm = z > CUT_Z;
  
  // Calculate P(Sound/E,M,Z) and P(Silence/E,M,Z)
  p1 = P_So[0][0] * P_So[te][1] * P_So[tm][2] * P_So[tz][3];
  p2 = P_Si[0][0] * P_Si[te][1] * P_Si[tm][2] * P_Si[tz][3];

  //Update the base values
  n_total++;
  
  // The higher probability is the classification for this file
  if (p1 > p2)
  {
    // This is a sound file
    so[0][0]++;
    si[0][1]++; // Useless as this value is not used, but keep consistency
    so[1][te]++;
    so[2][tm]++;
    so[3][tz]++;
  }
  else
  {
    // This is a silent file
    si[0][0]++;
    so[0][1]++; // Useless as this value is not used, but keep consistency
    si[1][te]++;
    si[2][tm]++;
    si[3][tz]++;
  }
  
  return p1 > p2;
}