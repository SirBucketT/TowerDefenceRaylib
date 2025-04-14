#ifndef SIMPLE_PROFILER_H
#define SIMPLE_PROFILER_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "raylib.h"  // For GetTime() and GetFrameTime()

#define MAX_PROFILE_SAMPLES 100
#define MAX_FUNCTION_NAME_LENGTH 64

typedef struct {
    char functionName[MAX_FUNCTION_NAME_LENGTH];
    double totalTime;
    double minTime;
    double maxTime;
    int calls;
} ProfileSample;

// Global array of profile samples
ProfileSample gProfileSamples[MAX_PROFILE_SAMPLES];
int gNumProfileSamples = 0;
bool gProfilingEnabled = true;
double gLastReportTime = 0;
double gReportInterval = 5.0; // Report every 5 seconds

// Start profiling a function 
double StartProfiling(const char* functionName) {
    if (!gProfilingEnabled) return 0;
    
    // Simply return the current time
    return GetTime();
}

// End profiling a function
void EndProfiling(const char* functionName, double startTime) {
    if (!gProfilingEnabled) return;
    
    double elapsedTime = GetTime() - startTime;
    
    // Find existing sample or create new one
    int sampleIndex = -1;
    for (int i = 0; i < gNumProfileSamples; i++) {
        if (strcmp(gProfileSamples[i].functionName, functionName) == 0) {
            sampleIndex = i;
            break;
        }
    }
    
    if (sampleIndex == -1) {
        // Create new sample
        if (gNumProfileSamples < MAX_PROFILE_SAMPLES) {
            sampleIndex = gNumProfileSamples++;
            strncpy(gProfileSamples[sampleIndex].functionName, functionName, MAX_FUNCTION_NAME_LENGTH-1);
            gProfileSamples[sampleIndex].totalTime = 0;
            gProfileSamples[sampleIndex].minTime = 999999.0;
            gProfileSamples[sampleIndex].maxTime = 0;
            gProfileSamples[sampleIndex].calls = 0;
        }
    }
    
    if (sampleIndex != -1) {
        gProfileSamples[sampleIndex].totalTime += elapsedTime;
        gProfileSamples[sampleIndex].calls++;
        
        if (elapsedTime < gProfileSamples[sampleIndex].minTime)
            gProfileSamples[sampleIndex].minTime = elapsedTime;
            
        if (elapsedTime > gProfileSamples[sampleIndex].maxTime)
            gProfileSamples[sampleIndex].maxTime = elapsedTime;
    }
    
    // Check if it's time to report
    double currentTime = GetTime();
    if (currentTime - gLastReportTime > gReportInterval) {
        ReportProfilingResults();
        gLastReportTime = currentTime;
    }
}

// Compare function for qsort
int CompareProfileSamples(const void* a, const void* b) {
    const ProfileSample* sampleA = (const ProfileSample*)a;
    const ProfileSample* sampleB = (const ProfileSample*)b;
    
    // Sort by total time, descending
    if (sampleB->totalTime > sampleA->totalTime) return 1;
    if (sampleB->totalTime < sampleA->totalTime) return -1;
    return 0;
}

// Report profiling results
void ReportProfilingResults() {
    if (gNumProfileSamples == 0) return;
    
    // Sort samples by total time
    qsort(gProfileSamples, gNumProfileSamples, sizeof(ProfileSample), CompareProfileSamples);
    
    // Open profile report file
    FILE* file = fopen("profile_report.txt", "w");
    if (!file) return;
    
    // Write header
    fprintf(file, "=== Profiling Report ===\n");
    fprintf(file, "Function                                  | Calls    | Total (ms) | Avg (ms)   | Min (ms)   | Max (ms)   |\n");
    fprintf(file, "------------------------------------------|----------|------------|------------|------------|------------|\n");
    
    // Write data rows
    for (int i = 0; i < gNumProfileSamples; i++) {
        ProfileSample* sample = &gProfileSamples[i];
        double avgTime = sample->totalTime / sample->calls;
        
        fprintf(file, "%-42s | %-8d | %-10.2f | %-10.2f | %-10.2f | %-10.2f |\n",
            sample->functionName,
            sample->calls,
            sample->totalTime * 1000.0,
            avgTime * 1000.0,
            sample->minTime * 1000.0,
            sample->maxTime * 1000.0);
    }
    
    // Write footer
    fprintf(file, "=== End of Report ===\n");
    
    fclose(file);
    
    // Reset for next interval
    for (int i = 0; i < gNumProfileSamples; i++) {
        gProfileSamples[i].totalTime = 0;
        gProfileSamples[i].minTime = 999999.0;
        gProfileSamples[i].maxTime = 0;
        gProfileSamples[i].calls = 0;
    }
}

// Define macros for easy usage
#define PROFILE_START(name) double profilerStart_##name = StartProfiling(#name)
#define PROFILE_END(name) EndProfiling(#name, profilerStart_##name)

#endif // SIMPLE_PROFILER_H