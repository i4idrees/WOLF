/*#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tinyxml2.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <cctype>
#include <regex>
#include <string>
#include <cstring>

#include "sut.h"
#include "xcsMacros.h"
#include "configuration.h"
#include "env.h"

void displayUpdatedTransactions(Transaction transactions[], int num_transactions)
{
    printf("\n\n======================\n\n");
    printf("Updated transactions:\n");
    printf("\n\n======================\n\n");
    for (int i = 0; i < num_transactions; i++)
    {
        printf("Transaction: %s, Workload: %d\n", transactions[i].name, transactions[i].workLoad);
    }

    printf("\n\n======================================\n\n");
}


void increaseWorkloadOfAll(Transaction transactions[], int num_transactions, int ratio)
{
    for (int i = 0; i < num_transactions; i++)
    {
        // Calculate the increased workload
        int increase = (transactions[i].workLoad * ratio) / 100;
        transactions[i].workLoad += increase;
    }
}

void increaseWorkloadOfParticular(Transaction transactions[], int num_transactions, const char* transaction_name, int ratio)
{
    for (int i = 0; i < num_transactions; i++)
    {
        if (strcmp(transactions[i].name, transaction_name) == 0)
        {
            // Calculate the increased workload
            int increase = (transactions[i].workLoad * ratio) / 100;
            transactions[i].workLoad += increase;
            printf("Updated %s: Workload: %d\n", transactions[i].name, transactions[i].workLoad);
            return;  // Exit the function after updating the specified transaction
        }
    }
    printf("Transaction with name '%s' not found.\n", transaction_name);
}

int calculateRampupTime(Transaction transactions[],int num_transactions, int threadPerSecond)
{
    int totalWorkload = 0;
    for (int i = 0; i < num_transactions; ++i)
    {
        totalWorkload += transactions[i].workLoad;
    }

    int rampUpTime = (int) round((double) totalWorkload / threadPerSecond);

    return rampUpTime;
}

// Function to update thread group num_threads, LoopController.loops, and ThreadGroup.ramp_time based on transactions array
void update_thread_group_num_threads(tinyxml2::XMLElement* element, Transaction transactions[], int num_transactions, int loops, int rampTime)
{
    if (!element) return; // Check for null element

    // Iterate over child elements
    for (tinyxml2::XMLElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
    {
        // Check if the element is a ThreadGroup
        if (strcmp(child->Name(), "ThreadGroup") == 0)
        {
            // Get the testname attribute from the ThreadGroup element
            const char* testname = child->Attribute("testname");
            if (!testname) continue; // Check for null attribute

            // Iterate over transactions to find a match
            for (int i = 0; i < num_transactions; ++i)
            {
                if (strcmp(transactions[i].name, testname) == 0)
                {
                    // Find and update the relevant properties
                    for (tinyxml2::XMLElement* property = child->FirstChildElement(); property; property = property->NextSiblingElement())
                    {
                        const char* name = property->Attribute("name");
                        if (name)
                        {
                            if (strcmp(property->Name(), "stringProp") == 0)
                            {
                                if (strcmp(name, "ThreadGroup.num_threads") == 0)
                                {
                                    // Update the num_threads with the workload value
                                    property->SetText(std::to_string(transactions[i].workLoad).c_str());
                                    std::cout << "Updated " << testname << " num_threads to " << transactions[i].workLoad << std::endl;
                                }
                                else if (strcmp(name, "ThreadGroup.ramp_time") == 0)
                                {
                                    // Update the ramp_time
                                    property->SetText(std::to_string(rampTime).c_str());
                                    //std::cout << "Updated " << testname << " ramp_time to " << rampTime << std::endl;
                                }
                            }
                            else if (strcmp(property->Name(), "elementProp") == 0 && strcmp(name, "ThreadGroup.main_controller") == 0)
                            {
                                // Look for LoopController.loops within the elementProp for main_controller
                                for (tinyxml2::XMLElement* subProperty = property->FirstChildElement(); subProperty; subProperty = subProperty->NextSiblingElement())
                                {
                                    if (strcmp(subProperty->Name(), "stringProp") == 0 && strcmp(subProperty->Attribute("name"), "LoopController.loops") == 0)
                                    {
                                        // Update the loops
                                        subProperty->SetText(std::to_string(loops).c_str());
                                        //std::cout << "Updated " << testname << " loops to " << loops << std::endl;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    break; // Exit the loop once the matching transaction is processed
                }
            }
        }
        // Recursively process child elements
        update_thread_group_num_threads(child, transactions, num_transactions, loops, rampTime);
    }
}

void updateThreadgroups(Transaction transactions[],int num_transactions,int rampUpTime, char* filename)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.LoadFile(filename);
    if (result != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Error: could not parse file " << filename << std::endl;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    update_thread_group_num_threads(root, transactions, num_transactions,1,rampUpTime);

    result = doc.SaveFile(filename);
    if (result != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Error: could not save file " << filename << std::endl;
    }

    std::cout << "Successfully updated ThreadGroup.num_threads based on transactions in " << filename << std::endl;
}


//return avg response time and error rate
std::tuple<double, double> runJMeterAndGetStats(std::string jmeterPath, std::string jmxFilePath) {
    char* resultsFilePath = "D:/apache-jmeter-5.1.1/bin/testing_transactions_rubis/results.jtl";
    std::string command = "\"" + jmeterPath + "\" -n -t \"" + jmxFilePath + "\" -l \"" + resultsFilePath + "\"";

    std::cout << "Executing command: " << command << std::endl;

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    // Create pipes for standard output
    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        std::cerr << "Failed to create pipe." << std::endl;
        return std::make_tuple(-1.0, -1.0);  // Indicate failure
    }

    // Ensure the read handle to the pipe is not inherited.
    if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0)) {
        std::cerr << "Failed to set handle information." << std::endl;
        return std::make_tuple(-1.0, -1.0);  // Indicate failure
    }

    // Set up the STARTUPINFO structure
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;
    si.dwFlags |= STARTF_USESTDHANDLES;

    int re = CreateProcess(NULL, const_cast<LPSTR>(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

    if (re) {
        // Close the write end of the pipe
        CloseHandle(hWritePipe);

        // Read the output from the process
        char buffer[4096];
        DWORD bytesRead;
        std::string output;
        while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            output += buffer;
        }

        // Close handles
        CloseHandle(hReadPipe);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        // Find and print the summary line
        std::regex summaryRegex(R"(summary\s*=\s*\d+\s*in\s*\d{2}:\d{2}:\d{2}\s*=\s*\d+\.\d+/s\s*Avg:\s*(\d+)\s*Min:\s*\d+\s*Max:\s*\d+\s*Err:\s*\d+\s*\((\d+\.\d+)%\))");
        std::smatch match;
        if (std::regex_search(output, match, summaryRegex)) {
            std::string summaryLine = match.str(0);
            std::cout << summaryLine << std::endl;

            double average = std::stod(match.str(1));
            double errorRate = std::stod(match.str(2));
            return std::make_tuple(average, errorRate);
        } else {
            std::cerr << "Failed to parse summary from output." << std::endl;
            return std::make_tuple(-1.0, -1.0);  // Indicate failure
        }
    } else {
        std::cerr << "Failed to create process. Error: " << GetLastError() << std::endl;
        return std::make_tuple(-1.0, -1.0);  // Indicate failure
    }
}
*/
