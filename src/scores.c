#include <stdio.h>
#include <string.h>
#include <time.h>
#include "scores.h"

int loadScores(Score scores[MAX_SCORES]) {
    FILE* f = fopen(SCORES_FILE, "r");
    if (!f) return 0;

    int count = 0;
    while (count < MAX_SCORES &&
           fscanf(f, "%3s %f %10s", scores[count].name, &scores[count].time, scores[count].date) == 3) {
        count++;
    }

    fclose(f);
    return count;
}

void saveScores(Score scores[MAX_SCORES], int count) {
    FILE* f = fopen(SCORES_FILE, "w");
    if (!f) return;

    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %.2f %s\n", scores[i].name, scores[i].time, scores[i].date);
    }

    fclose(f);
}

int insertScore(Score scores[MAX_SCORES], int* count, const char* name, float elapsedTime) {
    // checks if new time enters top MAX_SCORES
    if (*count == MAX_SCORES && elapsedTime >= scores[*count - 1].time)
        return 0;

    // creates new entry
    Score novo;
    strncpy(novo.name, name, 3);
    novo.name[3] = '\0';
    novo.time = elapsedTime;

    // uses curente date via time.h
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(novo.date, sizeof(novo.date), "%Y-%m-%d", tm_info);

    // finds insertion position (list sorted by ascending time)
    int pos = *count;
    for (int i = 0; i < *count; i++) {
        if (novo.time < scores[i].time) {
            pos = i;
            break;
        }
    }

    // opens space by shifting entries down
    int newCount = (*count < MAX_SCORES) ? *count + 1 : MAX_SCORES;
    for (int i = newCount - 1; i > pos; i--) {
        scores[i] = scores[i - 1];
    }

    scores[pos] = novo;
    *count = newCount;
    return 1;
}

void loadStats(Stats* stats) {
    stats->wins   = 0;
    stats->losses = 0;

    FILE* f = fopen(STATS_FILE, "r");
    if (!f) return;

    fscanf(f, "wins %d losses %d", &stats->wins, &stats->losses);
    fclose(f);
}

void saveStats(const Stats* stats) {
    FILE* f = fopen(STATS_FILE, "w");
    if (!f) return;

    fprintf(f, "wins %d losses %d\n", stats->wins, stats->losses);
    fclose(f);
}