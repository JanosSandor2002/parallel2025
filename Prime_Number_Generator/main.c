#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <CL/cl.h>
#include <time.h>

#define N_SIZES     10
#define N_LOCALS    4

const unsigned int Ns[N_SIZES]       = {100000, 250000, 500000, 750000, 1000000, 1250000, 1500000, 1750000, 2000000, 2250000};
const size_t       locals[N_LOCALS]  = {32, 64, 128, 256};

const char *kernel_src =
"__kernel void prime_kernel(const unsigned int N,\n"
"                           __global unsigned int *out,\n"
"                           __global unsigned int *count) {\n"
"    size_t gid = get_global_id(0);\n"
"    unsigned int n = gid + 2;\n"
"    if (n > N) return;\n"
"    bool is_prime = true;\n"
"    for (unsigned int d = 2; d * d <= n; ++d) {\n"
"        if (n % d == 0) { is_prime = false; break; }\n"
"    }\n"
"    if (is_prime) {\n"
"        unsigned int idx = atomic_inc(count);\n"
"        out[idx] = n;\n"
"    }\n"
"}\n";

int main() {
    printf("OpenCL primkereses inditasa...\n");

    FILE *fp = fopen("prime_results.csv", "w");
    if (!fp) {
        perror("fopen");
        return 1;
    }
    fprintf(fp, "Variant,N,LocalSize,KernelTime,Speedup,Efficiency\n");

    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;

    // Profilozás engedélyezése!
    cl_command_queue queue;
    const cl_queue_properties props[] = {
        CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE,
        0
    };

    printf("OpenCL platform lekerdezese...\n");
    err = clGetPlatformIDs(1, &platform, NULL);
    if (err != CL_SUCCESS) { printf("Hiba platform lekerdezeskor: %d\n", err); return 1; }

    printf("OpenCL eszkoz lekerdezese...\n");
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
    if (err != CL_SUCCESS) { printf("Hiba eszkoz lekerdezeskor: %d\n", err); return 1; }

    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    queue = clCreateCommandQueueWithProperties(context, device, props, &err);
    if (err != CL_SUCCESS) { printf("Hiba parancssor letrehozasakor: %d\n", err); return 1; }
    printf("OpenCL kornyezet es parancssor letrehozva.\n");

    cl_program program = clCreateProgramWithSource(context, 1, &kernel_src, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Program forditasi hiba! Hibakod: %d\n", err);
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *log = malloc(log_size);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("Forditasi naplo:\n%s\n", log);
        free(log);
        return 1;
    }
    cl_kernel kernel = clCreateKernel(program, "prime_kernel", &err);
    printf("Kernel sikeresen leforditva es letrehozva.\n");

    unsigned int *out = malloc(sizeof(unsigned int) * Ns[N_SIZES - 1]);
    unsigned int host_count;

    cl_mem buf_out = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                    sizeof(unsigned int) * Ns[N_SIZES - 1], NULL, &err);
    cl_mem buf_count = clCreateBuffer(context, CL_MEM_READ_WRITE,
                                      sizeof(unsigned int), NULL, &err);

    for (int si = 0; si < N_SIZES; ++si) {
        unsigned int N = Ns[si];
        printf("\n--- Primkereses N = %u ertekre ---\n", N);

        clock_t t0 = clock();
        unsigned int seq_count = 0;
        for (unsigned int n = 2; n <= N; ++n) {
            bool is_pr = true;
            for (unsigned int d = 2; d * d <= n; ++d)
                if (n % d == 0) { is_pr = false; break; }
            if (is_pr) ++seq_count;
        }
        double seq_time = (double)(clock() - t0) / CLOCKS_PER_SEC;
        printf("Szekvencialis primkereses kesz. Ido: %.6f s, Talalt primek: %u\n", seq_time, seq_count);

        for (int li = 0; li < N_LOCALS; ++li) {
            size_t local_size = locals[li];
            size_t global_size = (N + local_size - 1) / local_size * local_size;
            printf("OpenCL futtatas (local_size = %zu, global_size = %zu)...\n", local_size, global_size);

            host_count = 0;
            clEnqueueWriteBuffer(queue, buf_count, CL_TRUE, 0, sizeof(unsigned int), &host_count, 0, NULL, NULL);

            err = clSetKernelArg(kernel, 0, sizeof(unsigned int), &N);
            err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &buf_out);
            err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &buf_count);
            if (err != CL_SUCCESS) { printf("Hiba SetKernelArg-nal: %d\n", err); continue; }

            cl_event prof_evt;
            err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
                                         &global_size, &local_size,
                                         0, NULL, &prof_evt);
            if (err != CL_SUCCESS) { printf("Hiba kernel futtatas közben: %d\n", err); continue; }

            clFinish(queue);

            cl_ulong start_ns = 0, end_ns = 0;
            clGetEventProfilingInfo(prof_evt, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start_ns, NULL);
            clGetEventProfilingInfo(prof_evt, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end_ns, NULL);
            double kernel_time = (end_ns - start_ns) * 1e-9;

            clEnqueueReadBuffer(queue, buf_count, CL_TRUE, 0, sizeof(unsigned int), &host_count, 0, NULL, NULL);
            printf("Kernel futas kesz. Ido: %.6f s, Talalt primek: %u\n", kernel_time, host_count);

            double speedup = seq_time / kernel_time;
            double efficiency = speedup / (global_size / local_size);

            fprintf(fp, "opencl,%u,%zu,%.6f,%.6f,%.6f\n",
                    N, local_size, kernel_time, speedup, efficiency);

            printf("Speedup: %.2f, Hatekonysag: %.4f\n", speedup, efficiency);

            clReleaseEvent(prof_evt);
        }
    }

    fclose(fp);
    clReleaseMemObject(buf_out);
    clReleaseMemObject(buf_count);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(out);

    printf("\nMeresek elmentve: prime_results.csv\n");
    printf("Program befejezve.\n");
    return 0;
}
