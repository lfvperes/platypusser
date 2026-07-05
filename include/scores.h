#ifndef SCORES_H
#define SCORES_H

#define MAX_SCORES 5
#define SCORES_FILE "scores.txt"
#define STATS_FILE  "stats.txt"

typedef struct {
    char name[4];   // 3 letters + '\0'
    float time;     // time in seconds (lower = better)
    char date[11];  // "YYYY-MM-DD" + '\0'
} Score;

typedef struct {
    int wins;
    int losses;
} Stats;

// loads until MAX_SCORES entries from SCORES_FILE
// returns the number of entries read
int loadScores(Score scores[MAX_SCORES]);

// saves ccount entries to SCORES_FILE
void saveScores(Score scores[MAX_SCORES], int count);

// tries to insert a new score into the list sorted by time.// returns 1 if it entered the top MAX_SCORES, 0 otherwise
int insertScore(Score scores[MAX_SCORES], int* count, const char* name, float elapsedTime);

// loads wins/losses from STATS_FILE
void loadStats(Stats* stats);

// saves wins/losses to STATS_FILE
void saveStats(const Stats* stats);

#endif