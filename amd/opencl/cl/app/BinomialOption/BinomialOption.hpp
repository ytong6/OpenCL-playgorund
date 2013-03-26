/**********************************************************************
Copyright ?012 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

?Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
?Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/


#ifndef BINOMIAL_OPTION_H_
#define BINOMIAL_OPTION_H_


//Header Files
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <SDKCommon.hpp>
#include <SDKApplication.hpp>
#include <SDKFile.hpp>

#include <malloc.h>

/**
 * \RISKFREE 0.02f
 * \brief risk free interest rate.
 */
#define RISKFREE 0.02f

/**
 * \VOLATILITY 0.30f
 * \brief Volatility factor for Binomial Option Pricing.
 */
#define VOLATILITY 0.30f


/**
 * BinomialOption 
 * Class implements OpenCL  BinomialOption sample
 * Derived from SDKSample base class
 */

class BinomialOption : public SDKSample
{
    cl_double setupTime;            /**< Time taken to setup OpenCL resources and building kernel */
    cl_double kernelTime;           /**< Time taken to run kernel and read result back */
    cl_int numSamples;              /**< No. of time steps */
    cl_int numSteps;                /**< No. of samples */
    cl_float* randArray;            /**< Array of float random numbers */
    cl_float* output;               /**< Output result */
    cl_float* refOutput;            /**< Reference result */
    cl_context context;             /**< CL context */
    cl_device_id *devices;          /**< CL device list */
    cl_mem randBuffer;              /**< CL memory buffer for random numbers */
    cl_mem outBuffer;               /**< CL memory buffer for output*/
    cl_command_queue commandQueue;  /**< CL command queue */
    cl_program program;             /**< CL program  */
    cl_kernel kernel;               /**< CL kernel */
    int iterations;                 /**< Number of iterations for kernel execution */
    streamsdk::SDKDeviceInfo deviceInfo;/**< Structure to store device information*/
    streamsdk::KernelWorkGroupInfo kernelInfo;/**< Structure to store kernel related info */

private:

    float random(float randMax, float randMin);

public:
    /** 
     * Constructor 
     * Initialize member variables
     * @param name name of sample (string)
     */
    explicit BinomialOption(std::string name)
        : SDKSample(name),
          setupTime(0),
          kernelTime(0),
          randArray(NULL),
          output(NULL),
          refOutput(NULL),
          devices(NULL),
          iterations(1)
    {
        numSamples = 64;
        numSteps = 254;
    }

    /** 
     * Constructor 
     * Initialize member variables
     * @param name name of sample (const char*)
     */
    explicit BinomialOption(const char* name)
        : SDKSample(name),
          setupTime(0),
          kernelTime(0),
          randArray(NULL),
          output(NULL),
          refOutput(NULL),
          devices(NULL),
          iterations(1)
    {
        numSamples = 64;
        numSteps = 254;
    }

    ~BinomialOption();

    /**
     * Allocate and initialize host memory array with random values
     * @return SDK_SUCCESS on success and SDK_FAILURE on failure
     */
    int setupBinomialOption();

    /**
     * Override from SDKSample, Generate binary image of given kernel 
     * and exit application
     * @return SDK_SUCCESS on success and SDK_FAILURE on failure
     */
    int genBinaryImage();

    /**
     * OpenCL related initialisations. 
     * Set up Context, Device list, Command Queue, Memory buffers
     * Build CL kernel program executable
     * @return SDK_SUCCESS on success and SDK_FAILURE on failure
     */
    int setupCL();

    /**
     * Set values for kernels' arguments, enqueue calls to the kernels
     * on to the command queue, wait till end of kernel execution.
     * Get kernel start and end time if timing is enabled
     * @return SDK_SUCCESS on success and SDK_FAILURE on failure
     */
    int runCLKernels();

    /**
     * Reference CPU implementation of Binomial Option
     * for performance comparison
     * @return SDK_SUCCESS on success and SDK_FAILURE on failure
     */
    int binomialOptionCPUReference();

    /**
     * Override from SDKSample. Print sample stats.
     */
    void printStats();

    /**
     * Override from SDKSample. Initialize 
     * command line parser, add custom options
     * @return SDK_SUCCESS on success and SDK_FAILURE on failure
     */
    int initialize();

    /**
     * Override from SDKSample, adjust width and height 
     * of execution domain, perform all sample setup
     * @return SDK_SUCCESS on success and SDK_FAILURE on failure
     */
    int setup();

    /**
     * Override from SDKSample
     * Run OpenCL BinomialOption
     * @return SDK_SUCCESS on success and SDK_FAILURE on failure
     */
    int run();

    /**
     * Override from SDKSample
     * Cleanup memory allocations
     * @return SDK_SUCCESS on success and SDK_FAILURE on failure
     */
    int cleanup();

    /**
     * Override from SDKSample
     * Verify against reference implementation
     * @return SDK_SUCCESS on success and SDK_FAILURE on failure
     */
    int verifyResults();
};

#endif // BINOMIAL_OPTION_H_
