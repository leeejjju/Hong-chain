#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>  // Include the JSON library (install via vcpkg or other package manager)

// Your GitHub Personal Access Token
std::string repo_owner = "leeejjju";    // Owner of the repository
std::string repo_name = "Hong-chain";      // Name of the repository

void create_github_issue(const std::string& title, const std::string& body, std::string& repo_owner, std::string repo_name, std::string& github_token) {
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        std::string url = "https://api.github.com/repos/" + repo_owner + "/" + repo_name + "/issues";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        // Set headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: token " + github_token).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Create JSON data
        Json::Value issue;
        issue["title"] = title;
        issue["body"] = body;
        Json::StreamWriterBuilder writer;
        std::string json_data = Json::writeString(writer, issue);

        // Set POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

        // Perform the request
        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

/*
int main() {
	std::string tok;
	std::cout << "token: ";
	std::cin >> tok;
    std::string issue_title = "Wanna go home";
    std::string issue_body = "# this is title <br> this is contents";
    std::string repo_owner = "Chodabeen";
    std::string repo_name = "git-test";
	
    create_github_issue(issue_title, issue_body, repo_owner, repo_name, tok);
    return 0;
}
*/
