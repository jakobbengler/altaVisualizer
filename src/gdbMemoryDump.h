//
//  gdbMemoryDump.h
//  altaVisualizer
//
//  Created by Jakob Bak on 16/03/16.
//
//

#ifndef gdbMemoryDump_h
#define gdbMemoryDump_h

#include <stdio.h>
#include "ofUtils.h"
#include "ofFileUtils.h"

class gdbMemoryDump {
    
    public:
        
        gdbMemoryDump();
        ~gdbMemoryDump(){}
        
        char * setSignature(char * byte_ptr);
        char * setPhasesPerRevolution(char * byte_ptr);
        char * setArmHomePosition(char * byte_ptr);
        char * setArmClockwiseExtent(char * byte_ptr);
        char * setArmAntiClockwiseExtent(char * byte_ptr);
        char * setConfigWithSignature(char * byte_ptr);
        void printConfigWithSignature();
        void printSignature();
        
        char * loadPhaserPermastoreData(char * byte_ptr);
        char * loadEcmPermastoreData(char * byte_ptr);
    
        void readNewDataFromFileDump(string file_name);
        void printDataBytesAsFloats(void * mem_ptr, int num_bytes);
        float getFloatFromBytes(void * byte_ptr, uint32_t pos);
        float convertLittleEndianFloatAsBytesToBigEndianFloat(char * byte_ptr, uint32_t pos);
    
    
        std::vector<ofVec2f> points;
        ofVec2f point_zero;
        
        char * mem_dump_ptr;

        
        // MAKE SURE THESE DEFINES AND STRUCTS ARE IDENTICAL TO THE FIRMWARE!!!
        
    #define NUMBER_OF_ANGULAR_AXES 4
    #define AXIS_ARM 0
    #define AXIS_PLATTER 1
        
    #define NUMBER_OF_ECM_DRIVEN_MOTORS 2
    #define MAX_REFERENCE_POINTS_PER_REVOLUTION 400
    #define BUCKETS_PER_ROTATION 256
        
    #define SAMPLE_BUCKETS 8

    #define NUMBER_OF_GRAPH_AXES 1
    #define ITERATIONS 1
    #define SAMPLES_PER_ITERATION 4000


    
        
        // FOR RUNTIME
        typedef struct {
            uint16_t phases_per_revolution[NUMBER_OF_ECM_DRIVEN_MOTORS];
            float arm_home_position;
            float arm_clockwise_extent, arm_anti_clockwise_extent;
        } runtime_config_t;
        
        // FOR PHASER
        typedef struct {
            float nominals[BUCKETS_PER_ROTATION];
        } axis_table_t;
        
        // FOR ECM
        typedef struct {
            float offset, acceleration, velocity;
        } ecm_sample_t;
        
        typedef struct {
            float offset_max;
            float offset_min;
            float acceleration_min;
            float acceleration_max;
            uint8_t count;
        } raw_t;
        
        typedef struct {
            ecm_sample_t samples[SAMPLE_BUCKETS];
            raw_t raw;
            uint8_t raw_sample_counter;
            float alpha, beta, bias;
            bool stable;
        } ecm_model_t;
    
        // FOR ALTERNATIVE ECM ROUTINE
        typedef struct {
            // float offset, acceleration;
            float x, y, z;
        } sample_t;
        
        typedef struct {
            sample_t samples[ITERATIONS][SAMPLES_PER_ITERATION];
            int sample_counter[ITERATIONS];
            float alpha[ITERATIONS];
            float beta[ITERATIONS];
            float gamma[ITERATIONS];
            bool sampling[ITERATIONS];
            float offset[ITERATIONS];
            int current_iteration;
        } grapher_t;
            
        // FOR USE IN THIS APPLICATION
        typedef struct {
            uint32_t permastore_signature; // 4 bytes
            runtime_config_t config; // 16 bytes
            axis_table_t axis_table[NUMBER_OF_ANGULAR_AXES];
            float phase_reference[NUMBER_OF_ANGULAR_AXES][MAX_REFERENCE_POINTS_PER_REVOLUTION];
            float phase_reference_bias[NUMBER_OF_ANGULAR_AXES];
            ecm_model_t models[NUMBER_OF_ANGULAR_AXES];
            grapher_t grapher[NUMBER_OF_GRAPH_AXES];
        } memory_dump_t;
        
        memory_dump_t memory;
    
};



#endif /* gdbMemoryDump_h */
