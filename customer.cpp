#include <iostream>
#include <string>
#include <curl/curl.h>

// Function to send HTTP GET request to the server
std::string sendHttpRequest(const std::string &url, const std::string &authToken)
{
    std::string response;
    CURL *curl = curl_easy_init();
    if (curl)
    {
        // Set the URL and Authorization header
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: " + authToken).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the callback to receive the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                         [](char *ptr, size_t size, size_t nmemb, std::string *data)
                         {
                             data->append(ptr, size * nmemb);
                             return size * nmemb;
                         });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Send the HTTP GET request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
        {
            std::cerr << "Failed to send HTTP request: " << curl_easy_strerror(res) << std::endl;
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    else
    {
        std::cerr << "Failed to initialize CURL." << std::endl;
    }
    return response;
}

int main()
{
    // Server URL
    std::string serverUrl = "https://api.example.com/customer/details";

    // Authorization token for access control
    std::string authToken = "Bearer YOUR_AUTH_TOKEN";

    // Customer-specific website URL
    std::string customerWebsiteUrl = "https://www.example.com";

    // Check if the customer is accessing from the specific website
    std::string referringWebsite;
    std::cout << "Enter the website URL you are accessing from: ";
    std::cin >> referringWebsite;

    if (referringWebsite == customerWebsiteUrl)
    {
        // Send HTTP GET request to retrieve customer details
        std::string customerDetails = sendHttpRequest(serverUrl, authToken);

        // Display customer details
        std::cout << "Customer Details:\n"
                  << customerDetails << std::endl;
    }
    else
    {
        std::cout << "Access denied. You are not authorized to access customer details from this website." << std::endl;
    }

    return 0;
}