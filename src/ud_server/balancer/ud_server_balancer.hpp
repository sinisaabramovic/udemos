#ifndef ud_server_balancer_hpp
#define ud_server_balancer_hpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

#ifdef __linux__
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#elif defined(__APPLE__)
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/sysctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach_types.h>
#include <mach/mach_traps.h>
#include <mach/machine.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <iostream>
#endif

class ud_server_balancer
{
private:
    double get_cpu_usage()
    {
        double usage_percent = -1.0;

        // Get CPU usage information
#ifdef __APPLE__
        host_cpu_load_info_data_t cpu_info;
        mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
        kern_return_t status = host_statistics64(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpu_info, &count);

        if (status == KERN_SUCCESS)
        {
            uint64_t total_user_time = cpu_info.cpu_ticks[CPU_STATE_USER];
            uint64_t total_system_time = cpu_info.cpu_ticks[CPU_STATE_SYSTEM];
            uint64_t total_idle_time = cpu_info.cpu_ticks[CPU_STATE_IDLE];
            uint64_t total_time = total_user_time + total_system_time + total_idle_time;
            usage_percent = ((double)(total_user_time + total_system_time) / total_time) * 100.0;
        }
        else
        {
            std::cerr << "Error getting CPU usage: " << status << std::endl;
        }
#elif __linux__
        double user, nice, system, idle;
        std::ifstream stat_file("/proc/stat");
        std::string line;
        std::getline(stat_file, line);
        std::istringstream iss(line);
        iss.ignore(5, ' '); // Skip the "cpu" prefix
        iss >> user >> nice >> system >> idle;
        double total_time = user + nice + system + idle;
        usage_percent = ((total_time - idle) / total_time) * 100.0;
#endif

        return usage_percent;
    }

    double get_memory_usage()
    {
        double usage_mb = -1.0;

        // Get memory usage information
#ifdef __APPLE__
        task_basic_info_data_t info;
        mach_msg_type_number_t count = TASK_BASIC_INFO_COUNT;
        if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS)
        {
            usage_mb = (double)info.resident_size / (1024.0 * 1024.0);
        }
#elif __linux__
        std::ifstream mem_file("/proc/self/status");
        std::string line;
        while (std::getline(mem_file, line))
        {
            if (line.compare(0, 7, "VmRSS: ") == 0)
            {
                std::istringstream iss(line.substr(7));
                iss >> usage_mb;
                break;
            }
        }
#endif

        return usage_mb;
    }

    double get_disk_usage()
    {
        double usage_percent = -1.0;

        // Get disk usage information
#ifdef __APPLE__
        struct statfs sfs;
        if (statfs("/", &sfs) == 0)
        {
            uint64_t block_size = sfs.f_bsize;
            uint64_t total_blocks = sfs.f_blocks;
            uint64_t free_blocks = sfs.f_bfree;
            uint64_t used_blocks = total_blocks - free_blocks;
            usage_percent = (double)(used_blocks * block_size) / (1024.0 * 1024.0);
        }
#elif __linux__
        struct statvfs vfs;
        if (statvfs("/", &vfs) == 0)
        {
            uint64_t block_size = vfs.f_frsize;
            uint64_t total_blocks = vfs.f_blocks;
            uint64_t free_blocks = vfs.f_bfree;
            uint64_t used_blocks = total_blocks - free_blocks;
            usage_percent = (double)(used_blocks * block_size) / (1024.0 * 1024.0);
        }
#endif

        return usage_percent;
    }

public:
    ud_server_balancer(/* args */);
    ~ud_server_balancer();
    std::string get_cpu_usage_description()
    {
        double cpu_usage = get_cpu_usage();
        std::string cpu_usage_str;
        if (cpu_usage >= 0.0)
        {
            cpu_usage_str = "CPU usage: " + std::to_string(cpu_usage) + " %";
        }
        else
        {
            cpu_usage_str = "Error getting cpu usage";
        }

        return cpu_usage_str;
    }

    std::string get_memory_usage_description()
    {
        double memory_usage = get_memory_usage();
        std::string memory_usage_str;
        if (memory_usage >= 0.0)
        {
            memory_usage_str = "Memory usage: " + std::to_string(memory_usage) + " MB";
        }
        else
        {
            memory_usage_str = "Error getting memory usage";
        }

        return memory_usage_str;
    }
    std::string get_disk_usage_description()
    {
        double disk_usage = get_disk_usage();
        std::string disk_usage_str;
        if (disk_usage >= 0.0)
        {
            disk_usage_str = "Disk usage: " + std::to_string(disk_usage) + " %";
        }
        else
        {
            disk_usage_str = "Error getting disk usage";
        }

        return disk_usage_str;
    }
};

ud_server_balancer::ud_server_balancer(/* args */)
{
}

ud_server_balancer::~ud_server_balancer()
{
}

#endif